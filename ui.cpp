#include "ui.h"


//Ui类的构造函数
Ui::Ui(QWidget *parent) : QWidget(parent)
{

	initTopMenu();
	initCentreView();
	initListView();
	initBottom();
	initLayout();
	initIcon();
	initButtonSize();
	initToolTip();
	initStyle();
	initOther();
}



void Ui::initTopMenu() {
	//打开一个文件
	openOneFile = new QAction();
	openOneFile->setText("导入歌曲");
	openOneFile->setShortcut(QKeySequence::Open);

	//打开一个文件夹
	openOneFolder = new QAction();
	openOneFolder->setText("导入文件夹");

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
}



void Ui::initCentreView() {
	//画面中心ui
	songname = new QLabel;
	songimage = new QLabel;

	songname->setText("请打开歌曲...");
	songname->setFixedSize(700,30);
	songname->setFont(QFont("微软雅黑",15,1));
	songname->setAlignment(Qt::AlignHCenter);

	pix.load(":/img/fm.png");
	songimage->setPixmap(pix);
	songimage->setFixedSize(354,354);
	songimage->setAlignment(Qt::AlignCenter);
}



void Ui::initListView() {
    viewlist = new QListWidget();
    viewlist->setStyleSheet("background:transparent"); //继承主窗口背景图片	
}



//播放控制
void Ui::initBottom() {
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
	//进度条时间显示
	time_progress = new QLabel;
	progress_ = new QLabel;
	//音量控制
	volume1 = new QToolButton;
	volume2 = new QSlider(this); //volume2放到主界面，如果没有this,音量滑块就会另开窗口
	//定位键
	locate = new QToolButton;	
}



void Ui::initLayout() {
	QVBoxLayout *root = new QVBoxLayout(this);

	QHBoxLayout *h1 = new QHBoxLayout();
	QHBoxLayout *h2 = new QHBoxLayout();
	QHBoxLayout *h3 = new QHBoxLayout();

	QVBoxLayout *v1 = new QVBoxLayout();

	h1->addWidget(songname);
	h2->addWidget(songimage);
	h2->addWidget(viewlist);

	root->addLayout(h1);
	root->addLayout(h2);

	h3->addWidget(before);
	h3->addWidget(playpause);
	h3->addWidget(next);
	h3->addWidget(playmode);
	h3->addWidget(volume1);
	//h3->addWidget(volume2);
	h3->addWidget(locate);
	v1->addWidget(time_progress);
	v1->addWidget(load);
	v1->addWidget(progress_);
	h3->addLayout(v1);
	root->addLayout(h3);
}



void Ui::initIcon() {
	//设置按键图标
	playpause->setIcon(QIcon(":/img/play.png"));
	next->setIcon(QIcon(":/img/next.png"));
	before->setIcon(QIcon(":/img/before.png"));
	playmode->setIcon(QIcon(":/img/顺序播放.png"));
	volume1->setIcon(QIcon(":/img/声音开.png"));
	locate->setIcon(QIcon(":/img/定位.png"));
}



void Ui::initButtonSize() {
	//设置按键大小
	playpause->setFixedSize(50,50);
	next->setFixedSize(50,50);
	before->setFixedSize(50,50);
	playmode->setFixedSize(50,50);
	volume1->setFixedSize(30,30);
	locate->setFixedSize(50,50);
}



void Ui::initToolTip() {
	//提示
	playpause->setToolTip("播放");
	next->setToolTip("下一曲");
	before->setToolTip("上一曲");
	playmode->setToolTip("顺序播放");
	volume1->setToolTip("音量");
	volume2->setToolTip("音量控制");
	locate->setToolTip("定位到当前播放歌曲");
	load->setToolTip("歌曲播放进度");	
}



void Ui::initStyle() {
	before->setStyleSheet("border:none");
	playpause->setStyleSheet("border:none");
	next->setStyleSheet("border:none");
	playmode->setStyleSheet("border:none");
	volume1->setStyleSheet("border:none");
	volume2->setStyleSheet("border:none");
	load->setStyleSheet("border:none");
	locate->setStyleSheet("border:none");
}



void Ui::initOther() {
	time_progress->setText("00:00 / 00:00");
	time_progress->setAlignment(Qt::AlignHCenter);
	time_progress->setFixedHeight(20);	
	progress_->setFixedHeight(10);

	volume2->setGeometry(QRect(223,375,100,100));
	volume2->hide();
	//控制音量大小
	volume2->setMaximum(100);
	volume2->setMinimum(0);
	volume2->setFixedSize(50,50);
	//初始化音量
    volume2->setValue(35);

	//设置水平进度条
	load->setOrientation(Qt::Horizontal);
	load->setFixedHeight(10);
	load->setMinimum(0);
	load->setMaximum(100);
}