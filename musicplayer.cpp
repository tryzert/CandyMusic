#include "musicplayer.h"


//构造函数
MusicPlayer::MusicPlayer(QWidget *parent)
	: QMainWindow(parent)
{
	initWindow();
	createMenuBar();
	//createToolBar();
	initPlayer();
	
	//QFile file("./musiclist.db");
	initDataBase();
	
	initList();
	bindSignal();
}



//析构函数
MusicPlayer::~MusicPlayer()
{
	delete ui;
}



void MusicPlayer::initWindow() {
	//设置窗体图标
	this->setWindowIcon(QIcon(":/img/mini.png"));
	
	//禁用右键
	this->setContextMenuPolicy(Qt::NoContextMenu);
	
	//窗体大小
	this->setFixedSize(700,500);
	
	//控件UI组件
	ui = new Ui();
	this->setCentralWidget(ui);

	//设置背景图片
	QPixmap pix = QPixmap("/home/maple/图片/Wallpapers/155222-1529481142eabe.jpg").scaled(this->size());
    QPalette pal(this->palette());
    pal.setBrush(QPalette::Background, QBrush(pix));
    this->setPalette(pal);
    this->setAutoFillBackground(true);
}



//创建菜单栏
void MusicPlayer::createMenuBar() {
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


void MusicPlayer::initPlayer() {
	//构造音乐播放器，并设置一个默认播放对象
	qtplayer = new QMediaPlayer(this);
	qtplayer->setVolume(35);

	//QPointer<> pPlayer = new QMediaPlayer(this);
	//内部播放器列表
	qtplaylist = new QMediaPlaylist(this);

	//将列表关联到播放器
	qtplayer->setPlaylist(qtplaylist);
}



void MusicPlayer::initDataBase() {
	//数据库存放歌曲信息。进行初始化
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("musiclist.db");
	if (db.open()) {
		QSqlQuery query;
		//filePathName:包含绝对路径的文件名称。songname:不含路径和后缀名的文件名称
	    query.exec("create table musiclist (filePathName varchar, songname varchar)"); 
		db.close();
	}
}



void MusicPlayer::initList() {
	if (db.open()) {
		QSqlQuery query;
		query.exec("select * from musiclist");
		while (query.next()) {
			qtplaylist->addMedia(QMediaContent(QUrl::fromLocalFile(query.value(0).toString())));
			ui->viewlist->addItem(query.value(1).toString());
		}
	}
	if (qtplaylist->isEmpty()) {
		ui->songname->setText("请打开歌曲...");
	} else {
		ui->songname->setText("播放已停止");
	}
}


//打开单个文件
void MusicPlayer::openOneFileFunction()
{
	//获得包含绝对路径的文件名
	QString filePathName = QFileDialog::getOpenFileName(this,QString("选择一首歌曲"),\
		"/home/maple/software/tank-3.0.5/matter/tryzert/root","*.mp3");
	
    if (filePathName!=nullptr){
    	initDataBase();
		//将读取的歌曲路径数据放进三个列表
		//１．添加到QMediaPlayList
		qtplaylist->addMedia(QUrl::fromLocalFile(filePathName));
		//２．添加到QList。暂时用不到，先注释掉。
		//songlist << filePathName;
		//３．添加到视图QListWidget。获得不含路径的文件名，且不含后缀名
		QFileInfo fileinfo(filePathName);
		QString songname = fileinfo.baseName();
		ui->viewlist->addItem(songname);
		if (db.open()) {
			QSqlQuery query;
			query.exec(QString("INSERT INTO musiclist VALUES('%1','%2')").arg(filePathName).arg(songname));
			db.close();
		}

		if (qtplayer->state() != QMediaPlayer::PlayingState) {
			ui->songname->setText("播放已停止");
		}
	}
}


//打开多个文件
void MusicPlayer::openOneFolderFunction()
{

	QString OneFolderName = QFileDialog::getExistingDirectory(this,"选择一个文件夹", \
		"/home/maple/software/tank-3.0.5/matter/tryzert/root");
	QDir dir(OneFolderName);
	if (OneFolderName!=nullptr) {
		initDataBase();
		if (db.open()) {
			QSqlQuery query;
			foreach (QFileInfo info, dir.entryInfoList()) {
				if (info.isFile()) {
					//１．添加到QMediaPlayList
					qtplaylist->addMedia(QMediaContent(QUrl::fromLocalFile(info.filePath())));
					//２．添加到QList。暂时用不到，先注释掉
					//songlist << info.filePath();
					//３．添加到视图QListWidget
					ui->viewlist->addItem(info.baseName());										
					query.exec(QString("INSERT INTO musiclist VALUES('%1','%2')").arg(info.filePath()).arg(info.baseName()));											
				}
			}
			db.close();
		}
		if (qtplayer->state() != QMediaPlayer::PlayingState) {
			ui->songname->setText("播放已停止");
		}
	}
}


//将歌曲从歌单移除
void MusicPlayer::delMusicFile() //ui->viewlist->currentRow()
{	
	if (qtplaylist->mediaCount()>0) {
		int values=QMessageBox::information(this,("移除选中歌曲"),("确定从歌单中移除下面歌曲吗?(不会删除原文件)\n"\
			+ui->viewlist->currentItem()->text()),\
			QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
		if (values==QMessageBox::Yes) {
			if (qtplaylist->mediaCount()>1) {
				//qtplayer->stop();
				int delRow = ui->viewlist->currentRow();	
				if (db.open()) {
					QSqlQuery query;
					query.exec(QString("delete from musiclist where songname=('%1')").arg(ui->viewlist->item(delRow)->text()));
					db.close();
				}		
				ui->viewlist->takeItem(delRow);
				qtplaylist->removeMedia(delRow);
				if (qtplayer->state() == QMediaPlayer::PlayingState) {
					ui->songname->setText("正在播放："+ui->viewlist->item(qtplaylist->currentIndex())->text());
					ui->playpause->setIcon(QIcon(":/img/pause.png"));
					ui->playpause->setToolTip("暂停");
				} else {
					ui->songname->setText("播放已停止");
					ui->playpause->setIcon(QIcon(":/img/play.png"));
					ui->playpause->setToolTip("播放");
				}

			} else {
				qtplayer->stop();
				ui->viewlist->clear();
				ui->songname->setText("请打开歌曲...");
				qtplaylist->clear();
				ui->playpause->setIcon(QIcon(":/img/play.png"));
				ui->playpause->setToolTip("播放");
				QFile file("./musiclist.db");
				if (file.exists()) {
					file.remove();
				}
			}			
		}
	}	
}


//清空播放列表
void MusicPlayer::clearMusicList()
{
	int values=QMessageBox::information(this,("清空歌单"),("确定清空歌单?\n(不会删除原文件)"),\
		QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
	if (values==QMessageBox::Yes) {
		ui->viewlist->clear();
		ui->songname->setText("请打开歌曲...");
		qtplaylist->clear();
		ui->playpause->setIcon(QIcon(":/img/play.png"));
		ui->playpause->setToolTip("播放");
		QFile file("./musiclist.db");
		if (file.exists()) {
			file.remove();
		}
	}
}


//关于
void MusicPlayer::about()
{
	QMessageBox::about(this,"关于","目前功能还非常简陋，后续会逐步完善。欢迎提出更多建议。");
}


//创建工具栏
void MusicPlayer::createToolBar()
{
	QToolBar *playctrl = new QToolBar(this);
	playctrl->setStyleSheet("backgroud:transparent;");
	playctrl->addWidget(ui->before);
	playctrl->addWidget(ui->playpause);
	playctrl->addWidget(ui->next);
	playctrl->addWidget(ui->playmode);
	playctrl->addWidget(ui->volume1);
	playctrl->addWidget(ui->volume2);
	playctrl->addWidget(ui->load);

	//ui->playpause->setStyleSheet("backgroud: transparent;");
	this->addToolBar(Qt::BottomToolBarArea,playctrl);

	/*
	QToolBar *volumectrl = new QToolBar(this);
	volumectrl->addWidget(ui->volume);
	this->addToolBar(Qt::BottomToolBarArea,volumectrl);

	QToolBar *loadctrl = new QToolBar(this);
	loadctrl->addWidget(ui->load);
	this->addToolBar(Qt::BottomToolBarArea,loadctrl);
	*/
}


//播放音乐按钮点击事件
void MusicPlayer::reversePlayerState()
{
	//如果歌曲列表非空，进行操作。否则点击不起作用
	if (!qtplaylist->isEmpty()) {
		if (qtplayer->state() == QMediaPlayer::PlayingState) {
			qtplayer->pause();
			ui->songname->setText("播放已停止");
			ui->playpause->setIcon(QIcon(":/img/play.png"));
			ui->playpause->setToolTip("播放");
		} else {
			qtplayer->play();
			int currentMusicIndex = qtplaylist->currentIndex();
			ui->songname->setText("正在播放："+ui->viewlist->item(currentMusicIndex)->text());
			ui->playpause->setIcon(QIcon(":/img/pause.png"));
			ui->playpause->setToolTip("暂停");
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
		ui->playpause->setToolTip("暂停");
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
		ui->playpause->setToolTip("暂停");
	}
}


//循环模式
void MusicPlayer::setPlayMode()
{
	//Sequential为顺序播放。Loop为列表循环。CurrentItemInLoop为单曲循环。Random为随机。
	auto mode = qtplaylist->playbackMode();
	if (mode == QMediaPlaylist::Sequential) {
		qtplaylist->setPlaybackMode(QMediaPlaylist::Loop);
		ui->playmode->setIcon(QIcon(":/img/列表循环.png"));
		ui->playmode->setToolTip("列表循环");
	} else if (mode == QMediaPlaylist::Loop) {
		qtplaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); 
		ui->playmode->setIcon(QIcon(":/img/单曲循环.png"));
		ui->playmode->setToolTip("单曲循环");
	} else if (mode == QMediaPlaylist::CurrentItemInLoop) {
		qtplaylist->setPlaybackMode(QMediaPlaylist::Random);
		ui->playmode->setIcon(QIcon(":/img/随机播放.png"));
		ui->playmode->setToolTip("随机播放");
	} else if (mode == QMediaPlaylist::Random) {
		qtplaylist->setPlaybackMode(QMediaPlaylist::Sequential);
		ui->playmode->setIcon(QIcon(":/img/顺序播放.png"));
		ui->playmode->setToolTip("顺序播放");
	}
}


//改变音量大小
void MusicPlayer::changeVolume()
{
	qtplayer->setVolume(ui->volume2->value());
	if (ui->volume2->value()==0) {
		ui->volume1->setIcon(QIcon(":/img/声音关.png"));
	}
	else {
		ui->volume1->setIcon(QIcon(":/img/声音开.png"));
	}
}


void MusicPlayer::volume1_clicked()
{
	
	if (ui->volume2->isHidden()) {
		ui->volume2->show();
	} else {
		ui->volume2->hide();
	}
	
}


//定位到当前播放歌曲
void MusicPlayer::locate_clicked()
{	
	//ui->viewlist->setFocus();
	ui->viewlist->setCurrentRow(qtplaylist->currentIndex());
	ui->viewlist->scrollToItem(ui->viewlist->item(qtplaylist->currentIndex()));
}


//改变进度条
void MusicPlayer::changeLoad() 
{
	int dur_time = int(qtplayer->duration()/1000);  //歌曲时间长度
	int cur_time = int(qtplayer->position()/1000);  //进度条当前位置
	ui->load->setValue(cur_time);
	if (ui->load->value()>dur_time) {
		ui->load->setValue(0);
		return;
	}
	QTime durationTime = QTime(0,(dur_time/60)%60,dur_time%60);
	QTime currentTime = QTime(0,(cur_time/60)%60,cur_time%60);
	ui->time_progress->setText(currentTime.toString("mm:ss")+" / "+durationTime.toString("mm:ss"));
}  


//设置进度条最大值
void MusicPlayer::setLoadMaxValue() 
{
	if (qtplayer->isAudioAvailable()) {
		long a = qtplayer->duration()/1000;
        int temp = int(a);
		ui->load->setMaximum(temp);
	}
}


//设置音乐播放进度
void MusicPlayer::setMusicPosition() 
{
	if (qtplayer->isAudioAvailable()) {
        qint64 temp = long((ui->load->value())*1000);
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
		ui->playpause->setIcon(QIcon(":/img/pause.png"));
		ui->songname->setText("正在播放："+ui->viewlist->item(currentMusicIndex)->text());
		ui->playpause->setToolTip("暂停");
	} else {
		ui->playpause->setIcon(QIcon(":/img/play.png"));
		ui->songname->setText("播放已停止");
		ui->playpause->setToolTip("播放");
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
	ui->playpause->setToolTip("暂停");
}


//响应鼠标点击事件
void MusicPlayer::mousePressEvent(QMouseEvent *event)
{
	ui->volume2->hide(); //隐藏音量滑块
    (void)event;
}



//绑定信号
void MusicPlayer::bindSignal()
{
	//打开文件
	connect(ui->openOneFile,SIGNAL(triggered(bool)),this,SLOT(openOneFileFunction()));

	//打开文件夹
	connect(ui->openOneFolder,SIGNAL(triggered(bool)),this,SLOT(openOneFolderFunction()));

	//移除歌曲
	connect(ui->delOneFile,SIGNAL(triggered(bool)),this,SLOT(delMusicFile()));

	//清空歌单
	connect(ui->clearPlayList,SIGNAL(triggered(bool)),this,SLOT(clearMusicList()));

	//关于
	connect(ui->about,SIGNAL(triggered(bool)),this,SLOT(about()));

	//播放或暂停
	connect(ui->playpause,SIGNAL(clicked(bool)),this,SLOT(reversePlayerState()));

	//上一曲
	connect(ui->before,SIGNAL(clicked(bool)),this,SLOT(beforeMusic()));

	//下一曲
	connect(ui->next,SIGNAL(clicked(bool)),this,SLOT(nextMusic()));

	//歌曲播放循环模式
	connect(ui->playmode,SIGNAL(clicked(bool)),this,SLOT(setPlayMode()));

	//改变音量大小
	connect(ui->volume1,SIGNAL(clicked(bool)),this,SLOT(volume1_clicked()));
	connect(ui->volume2,SIGNAL(valueChanged(int)),this,SLOT(changeVolume()));

	//定位到当前播放歌曲
	connect(ui->locate,SIGNAL(clicked(bool)),this,SLOT(locate_clicked()));

	//进度条移动，durationChanged是当前歌曲播放进度发生变化
	connect(qtplayer,SIGNAL(positionChanged(qint64)),this,SLOT(changeLoad()));
	
	//设置进度条最大值，durationChanged是切换歌曲时，歌曲时间长度发生变化。
	connect(qtplayer,SIGNAL(durationChanged(qint64)),this,SLOT(setLoadMaxValue()));

	//设置音乐播放进度
	connect(ui->load,SIGNAL(sliderReleased()),this,SLOT(setMusicPosition()));

	//设置歌曲标题为当前正在播放的歌曲
	connect(qtplaylist,SIGNAL(currentIndexChanged(int)),this,SLOT(indexChanged()));

	//对歌曲列表某一首歌双击
	connect(ui->viewlist,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,\
		SLOT(songDoubleClicked(QListWidgetItem *)));

}
