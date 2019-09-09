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
#include <QDial>
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

	//打开一个文件
	QAction *openOneFile;

	//打开一个文件夹
	QAction *openOneFolder;

	//删除一个文件
	QAction *delOneFile;

	//清理播放列表
	QAction *clearPlayList;
	
	//关于
	QAction *about;

	
	QLabel *songname;
	QLabel *songimage;
	QPixmap pix;
	//QLabel *list;
    //QListView *list;
    //QMediaPlaylist *list;
    QListWidget *viewlist;

	//播放或暂停按钮
	QToolButton *playpause;
	//上一曲
	QToolButton *before;
	//下一曲
	QToolButton *next;

	//歌曲循环模式：单曲循环，全部循环，随机播放
	QToolButton *playmode;

	//播放进度条
	QSlider *load;
	//音量控制
	QDial *volume;


signals:

public slots:


};

#endif // UI_H
