#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPointer>
#include <QFileDialog>
#include <QList>
#include <QUrl>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMouseEvent>
#include <QMediaMetaData>
#include <QMessageBox>
#include <QMediaContent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "ui.h"

class MusicPlayer : public QMainWindow
{
	Q_OBJECT

public:
	//构造函数
    MusicPlayer(QWidget *parent = nullptr);
	//析构函数
	~MusicPlayer();

	void initWindow();
	void createMenuBar();
	void createToolBar();
	void initPlayer();
	void initDataBase();
	void initList();
	
	//UI
	Ui *ui;

	//菜单
	QMenu *file;
	QMenu *edit; 
	QMenu *view;
	QMenu *help;

	//Qt内置播放器
	QMediaPlayer *qtplayer;

	//播放器内含播放列表
	QMediaPlaylist *qtplaylist;
	
	//数据库存放歌曲信息
	QSqlDatabase db;
	void bindSignal();


public slots:
	//打开文件
	void openOneFileFunction();
	//打开文件夹
	void openOneFolderFunction();
	//从歌单删除歌曲
	void delMusicFile();
	//清理播放列表
	void clearMusicList();
	//关于
	void about();
	//播放或暂停音乐
	void reversePlayerState();
	//上一曲
	void preMusic();
	//下一曲
	void nextMusic();
	//设置歌曲循环模式
	void setPlayMode();
	//改变音量大小
	void changeVolume();
	void volume1_clicked();
	//定位键
	void locate_clicked();
	//设置进度条
	void changeLoad();  //改变进度条
	void setLoadMaxValue(); //设置进度条最大值
	void setMusicPosition(); //设置音乐播放进度

	//播放状态发生变化
	void indexChanged();
	//对歌曲列表某一首歌双击
	void songDoubleClicked(QListWidgetItem *);
	//void musicStateChanged(QMediaPlayer::State _state);
	//鼠标点击事件
	void mousePressEvent(QMouseEvent *event);

protected:


signals:

};


#endif 
