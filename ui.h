#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QLabel>
#include <QPicture>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QGridLayout>
#include <QListView>
#include <QMatrix>
#include <QAction>
#include <QPushButton>
#include <QToolButton>
#include <QSlider>
#include <QFileDialog>
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QStatusBar>
#include <QPointer>
#include <QMediaPlaylist>
#include <QListWidget>
#include <QListWidgetItem>

class Ui : public QWidget
{
	Q_OBJECT
public:
	explicit Ui(QWidget *parent = nullptr);

	QAction *openOneFile;//打开一个文件
	QAction *openOneFolder;//打开一个文件夹
	QAction *delOneFile; //删除一个文件
	QAction *clearPlayList;  //清理播放列表
	QAction *about;			 //关于

	QLabel *songname;
	QLabel *songimage;
	QPixmap pix;

	QListWidget *viewlist;

	QToolButton *playpause; //播放或暂停按钮
	QToolButton *before;	//上一曲
	QToolButton *next;		//下一曲
	QToolButton *playmode;  //歌曲循环模式：单曲循环，全部循环，随机播放

	QSlider *load;		   //播放进度条
	QLabel *time_progress; //进度条时间显示
	QLabel *progress_;

	QToolButton *volume1; //音量控制
	QSlider *volume2;

	QToolButton *locate; //定位键

	void initTopMenu(); //初始化顶部菜单
	void initCentreView();
	void initListView();
	void initBottom();
	void initLayout();
	void initIcon();
	void initButtonSize();
	void initToolTip();
	void initStyle();
	void initOther();

signals:

public slots:
};

#endif // UI_H
