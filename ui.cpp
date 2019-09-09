#include "ui.h"


//Ui类的构造函数
Ui::Ui(QWidget *parent) : QWidget(parent)
{
	//打开一个文件
	openOneFile = new QAction();
	openOneFile->setText("选择歌曲");
	openOneFile->setShortcut(QKeySequence::Open);

	//打开一个文件夹
	openOneFolder = new QAction();
	openOneFolder->setText("选择文件夹");

	//删除一个文件
	delOneFile = new QAction();
	delOneFile->setText("移除歌曲");

	//清理播放列表
	clearPlayList = new QAction();
	clearPlayList->setText("清空歌单");

	//关于
	about = new QAction();
	about->setText("关于");
	about->setIcon(QIcon(":/img/bix.png"));


	//画面中心ui
	songname = new QLabel;
	songimage = new QLabel;
    //list = new QListView();
	//list = new QLabel;
    //list = new QMediaPlaylist(this);
    viewlist = new QListWidget();

	QVBoxLayout *root = new QVBoxLayout(this);

	QHBoxLayout *h1 = new QHBoxLayout();
	QHBoxLayout *h2 = new QHBoxLayout();


	songname->setText("请打开歌曲...");
	songname->setFixedSize(700,30);
	songname->setFont(QFont("微软雅黑",15,1));
	songname->setAlignment(Qt::AlignHCenter);


	pix.load(":/img/fm.png");
	songimage->setPixmap(pix);
	songimage->setFixedSize(354,354);
	songimage->setAlignment(Qt::AlignCenter);


	//list->setFixedSize(300,380);

	h1->addWidget(songname);
	h2->addWidget(songimage);
	h2->addWidget(viewlist);

	root->addLayout(h1);
	root->addLayout(h2);


	//播放控制
	//播放按钮
	playpause = new QToolButton;
	//上一曲
	before = new QToolButton;
	//下一曲
	next = new QToolButton;

	//循环模式
	playmode = new QToolButton;

	//进度条
	load = new QSlider;
	//音量控制
	volume = new QDial;

	//设置按键图标
	playpause->setIcon(QIcon(":/img/play.png"));
	//pause->setIcon(QIcon(":/img/pause.png"));
	next->setIcon(QIcon(":/img/next.png"));
	before->setIcon(QIcon(":/img/before.png"));

	playmode->setIcon(QIcon(":/img/顺序播放.png"));

	//设置按键大小
	playpause->setFixedSize(50,50);
	next->setFixedSize(50,50);
	before->setFixedSize(50,50);
	playmode->setFixedSize(50,50);

	//提示
	playpause->setStatusTip("播放");
	//pause->setStatusTip("暂停");
	next->setStatusTip("下一曲");
	before->setStatusTip("上一曲");

	//控制音量大小
	volume->setMaximum(100);
	volume->setMinimum(0);
	volume->setFixedSize(50,50);
	volume->setStatusTip("音量");
	//初始化音量
    volume->setValue(35);


	//设置水平进度条
	load->setOrientation(Qt::Horizontal);
	load->setFixedHeight(20);
	load->setMinimum(0);
	load->setMaximum(100);
	load->setStatusTip("歌曲播放进度");


	//状态栏
	status = new QStatusBar;
	status->setStatusTip("Ready...");
	status->showMessage("Ready");
	status->activateWindow();
}

