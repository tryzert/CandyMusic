#include "musicplayer.h"


//构造函数
MusicPlayer::MusicPlayer(QWidget *parent)
	: QMainWindow(parent)
{
	//设置窗体图标
	this->setWindowIcon(QIcon(":/img/mini底部窗口（2）.png"));
	
	//禁用右键
	this->setContextMenuPolicy(Qt::NoContextMenu);
	
	//窗体大小
	this->setFixedSize(700,530);
	
	//控件UI组件
	ui = new Ui();
	this->setCentralWidget(ui);

	createMenuBar();
	createToolBar();


	//构造音乐播放器，并设置一个默认播放对象
	qtplayer = new QMediaPlayer(this);
	//qtplayer->setMedia(QUrl::fromLocalFile("/home/maple/music_test/The Chainsmokers,Aash Mehta - Clsr (Aash Mehta Flip).mp3"));
	qtplayer->setVolume(35);

	//QPointer<> pPlayer = new QMediaPlayer(this);

	//内部播放器列表
	qtplaylist = new QMediaPlaylist(this);

	//将列表关联到播放器
	qtplayer->setPlaylist(qtplaylist);
	
	//歌曲数据列表，暂时用不到
	//QList<QString> songlist;

	//打开文件
	connect(ui->openOneFile,SIGNAL(triggered(bool)),this,SLOT(openOneFileFunction()));

	//打开文件夹
	connect(ui->openOneFolder,SIGNAL(triggered(bool)),this,SLOT(openOneFolderFunction()));

	//关于
	connect(ui->about,SIGNAL(triggered(bool)),this,SLOT(about()));

	//播放或暂停
	connect(ui->playpause,SIGNAL(clicked(bool)),this,SLOT(playerState()));

	//上一曲
	connect(ui->before,SIGNAL(clicked(bool)),this,SLOT(beforeMusic()));

	//下一曲
	connect(ui->next,SIGNAL(clicked(bool)),this,SLOT(nextMusic()));

	//歌曲播放循环模式
	connect(ui->playmode,SIGNAL(clicked(bool)),this,SLOT(setPlayMode()));

	//改变音量大小
	connect(ui->volume,SIGNAL(valueChanged(int)),this,SLOT(changeVolume()));

	//进度条移动，durationChanged是当前歌曲播放进度发生变化
	connect(qtplayer,SIGNAL(positionChanged(qint64)),this,SLOT(changeLoad()));
	
	//设置进度条最大值，durationChanged是切换歌曲时，歌曲时间长度发生变化。
	connect(qtplayer,SIGNAL(durationChanged(qint64)),this,SLOT(setLoadMaxValue()));

	//设置音乐播放进度
	connect(ui->load,SIGNAL(sliderReleased()),this,SLOT(setMusicPosition()));

	//设置歌曲标题为当前正在播放的歌曲
	connect(qtplaylist,SIGNAL(currentIndexChanged(int)),this,SLOT(indexChanged()));

	//对歌曲列表某一首歌双击
	connect(ui->viewlist,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(songDoubleClicked(QListWidgetItem *)));
}

//析构函数
MusicPlayer::~MusicPlayer()
{
	delete ui;
}


//创建菜单栏
void MusicPlayer::createMenuBar()
{
	file = this->menuBar()->addMenu(tr("&文件"));
	edit = this->menuBar()->addMenu(tr("&修改"));
	view = this->menuBar()->addMenu(tr("&查看"));
	help = this->menuBar()->addMenu(tr("&帮助"));

	//打开文件
	file->addAction(ui->openOneFile);
	//打开一个文件夹
	file->addAction(ui->openOneFolder);

	//删除一个文件
	edit->addAction(ui->delOneFile);
	//清空播放列表
	edit->addAction(ui->clearPlayList);

	//关于
	help->addAction(ui->about);
}


//打开单个文件
void MusicPlayer::openOneFileFunction()
{
	//获得包含绝对路径的文件名
	QString filePathName = QFileDialog::getOpenFileName(this,QString("选择歌曲"),"/home/maple/software/tank-3.0.5/matter/tryzert/root","*.mp3");
	
    if (filePathName!=nullptr){
		//将读取的歌曲路径数据放进三个列表
		//１．添加到QMediaPlayList
		qtplaylist->addMedia(QUrl::fromLocalFile(filePathName));
		//２．添加到QList。暂时用不到，先注释掉。
		//songlist << filePathName;
		//３．添加到视图QListWidget。获得不含路径的文件名，且不含后缀名
		QFileInfo fileinfo(filePathName);
		QString songname = fileinfo.baseName();
		ui->viewlist->addItem(songname);
		//qDebug() << qtplaylist;//songlist;
	}
}


//打开多个文件
void MusicPlayer::openOneFolderFunction()
{

	QString OneFolderName = QFileDialog::getExistingDirectory(this,"选择一个文件夹", "/home/maple/software/tank-3.0.5/matter/tryzert/root");
	QDir dir(OneFolderName);
	if (OneFolderName!=nullptr) {
		foreach (QFileInfo info, dir.entryInfoList()) {
			if (info.isFile()) {
				//１．添加到QMediaPlayList
				qtplaylist->addMedia(QMediaContent(QUrl::fromLocalFile(info.filePath())));
				//２．添加到QList。暂时用不到，先注释掉
				//songlist << info.filePath();
				//３．添加到视图QListWidget
				ui->viewlist->addItem(info.baseName());
			}
		}
	}
	//qDebug() << songlist;
}


//将歌曲从歌单移除
void MusicPlayer::delMusicFile()
{

}


//清理播放列表
void MusicPlayer::clearMusicList()
{

}


//关于
void MusicPlayer::about()
{
	QMessageBox::about(this,"关于","巴拉巴拉巴拉。。。");
}


//创建工具栏
void MusicPlayer::createToolBar()
{
	QToolBar *playctrl = new QToolBar(this);
	playctrl->addWidget(ui->before);
	playctrl->addWidget(ui->playpause);
	playctrl->addWidget(ui->next);
	playctrl->addWidget(ui->playmode);
	this->addToolBar(Qt::BottomToolBarArea,playctrl);


	QToolBar *volumectrl = new QToolBar(this);
	volumectrl->addWidget(ui->volume);
	this->addToolBar(Qt::BottomToolBarArea,volumectrl);

	QToolBar *loadctrl = new QToolBar(this);
	loadctrl->addWidget(ui->load);
	this->addToolBar(Qt::BottomToolBarArea,loadctrl);
}


//播放音乐按钮点击事件
void MusicPlayer::playerState()
{
	//如果歌曲列表非空，进行操作。否则点击不起作用
	if (!qtplaylist->isEmpty()) {
		if (qtplayer->state() == QMediaPlayer::PlayingState) {
			qtplayer->pause();
			ui->songname->setText("播放已停止");
			ui->playpause->setIcon(QIcon(":/img/play.png"));
		} else {
			qtplayer->play();
			int currentMusicIndex = qtplaylist->currentIndex();
			ui->songname->setText("正在播放："+ui->viewlist->item(currentMusicIndex)->text());
			ui->playpause->setIcon(QIcon(":/img/pause.png"));
		}
	}
}


//上一曲
void MusicPlayer::beforeMusic()
{
	//随机播放时，上一曲会跳跃播放。其他模式时，上一曲是紧挨着当前播放曲目的
	if (qtplaylist->playbackMode()==QMediaPlaylist::Random) {
		qtplaylist->previous();
	} else {
		if (qtplaylist->currentIndex() > 0) {
			//qtplaylist->previous();
			//当循环模式为单曲循环,用previous方法，始终一首歌，点击上一曲也不会切歌。下面这个方法才能切歌。
			qtplaylist->setCurrentIndex(qtplaylist->currentIndex()-1);
		} else {
			qtplaylist->setCurrentIndex(qtplaylist->mediaCount()-1);
		}
	}
	qtplayer->play();
	if (!qtplaylist->isEmpty()) {
		ui->playpause->setIcon(QIcon(":/img/pause.png"));
	}
}


//下一曲
void MusicPlayer::nextMusic()
{
	//随机播放时，下一曲会跳跃播放。其他模式时，下一曲是紧挨着当前曲目的
	if (qtplaylist->playbackMode()==QMediaPlaylist::Random) {
		qtplaylist->next();
	} else {
		if (qtplaylist->currentIndex() < qtplaylist->mediaCount()-1) {
			//qtplaylist->next();
			//当循环模式为单曲循环,用next方法，始终一首歌，点击下一曲也不会切歌。下面这个方法才能切歌。
			qtplaylist->setCurrentIndex(qtplaylist->currentIndex()+1);
		} else {
			qtplaylist->setCurrentIndex(0);
		}
	}
	qtplayer->play();
	if (!qtplaylist->isEmpty()) {
		ui->playpause->setIcon(QIcon(":/img/pause.png"));
	}
}


//循环模式
void MusicPlayer::setPlayMode()
{
	//Sequential为顺序播放。Loop为列表循环。CurrentItemInLoop为单曲循环。Random为随机。
	if (qtplaylist->playbackMode()==QMediaPlaylist::Sequential) {
		qtplaylist->setPlaybackMode(QMediaPlaylist::Loop);
		ui->playmode->setIcon(QIcon(":/img/列表循环.png"));
	} else if (qtplaylist->playbackMode()==QMediaPlaylist::Loop) {
		qtplaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); 
		ui->playmode->setIcon(QIcon(":/img/单曲循环.png"));
	} else if (qtplaylist->playbackMode()==QMediaPlaylist::CurrentItemInLoop) {
		qtplaylist->setPlaybackMode(QMediaPlaylist::Random);
		ui->playmode->setIcon(QIcon(":/img/随机播放.png"));
	} else if (qtplaylist->playbackMode()==QMediaPlaylist::Random) {
		qtplaylist->setPlaybackMode(QMediaPlaylist::Sequential);
		ui->playmode->setIcon(QIcon(":/img/顺序播放.png"));
	}
}


//改变音量大小
void MusicPlayer::changeVolume()
{
	qtplayer->setVolume(ui->volume->value());
}


//改变进度条
void MusicPlayer::changeLoad() 
{
	long l = qtplayer->position()/1000;
	int temp = (int) l;
	ui->load->setValue(temp);
	if (ui->load->value()==((int)(qtplayer->duration()/1000))) {
		ui->load->setValue(0);
	}
}  


//设置进度条最大值
void MusicPlayer::setLoadMaxValue() 
{
	if (qtplayer->isAudioAvailable()) {
		long a = qtplayer->duration()/1000;
        int temp = (int) a;
		ui->load->setMaximum(temp);
	}
}


//设置音乐播放进度
void MusicPlayer::setMusicPosition() 
{
	if (qtplayer->isAudioAvailable()) {
		qint64 temp = (long) (ui->load->value())*1000;
		qtplayer->setPosition(temp);
	}
}


//切换歌曲(自动切换，手动切换)
void MusicPlayer::indexChanged()
{
	//获得当前播放歌曲索引位置
	int currentMusicIndex = qtplaylist->currentIndex();
	//索引为-1时表示列表最后一首歌曲播放完毕，停止播放。
	if (currentMusicIndex!=-1) {
		//标题显示正在播放的歌曲名
		ui->songname->setText("正在播放："+ui->viewlist->item(currentMusicIndex)->text());
	} else {
		ui->playpause->setIcon(QIcon(":/img/play.png"));
		ui->songname->setText("播放已停止");
	}
}


//对歌曲列表某一首歌双击
void MusicPlayer::songDoubleClicked(QListWidgetItem *item)
{
	//int currentMusicIndex = item;
	ui->songname->setText("正在播放："+item->text());
	//获取双击索引，及所在行号。并设置当前播放歌曲为双击选中行
	qtplaylist->setCurrentIndex(ui->viewlist->currentRow());
	qtplayer->play();
	ui->playpause->setIcon(QIcon(":/img/pause.png"));
}

