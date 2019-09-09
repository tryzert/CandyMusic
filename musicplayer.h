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
#include <QDir>
#include <QFileInfo>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMouseEvent>
#include <QMediaMetaData>
#include <QMessageBox>
#include <QMediaContent>
#include "ui.h"

class MusicPlayer : public QMainWindow
{
	Q_OBJECT

public:
	//构造函数
    MusicPlayer(QWidget *parent = nullptr);
	//析构函数
	~MusicPlayer();

	void createMenuBar();
	void createToolBar();

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
	
	//歌曲数据列表，这个暂时是用不到的
	//QList<QString> songlist;

	//视图
	//QStringListModel *model;

	//歌曲数据
	//QStringList sl;

public slots:
	//打开文件
	void openOneFileFunction();
	//打开文件夹
	void openOneFolderFunction();
	//从歌单删除歌曲
	void delMusicFile();
	//清理播放列表
	void clearMusicList();
	//查看。（待补充的内容）

	//关于
	void about();

	//播放或暂停音乐
	void playerState();
	//上一曲
	void beforeMusic();
	//下一曲
	void nextMusic();
	//设置歌曲循环模式
	void setPlayMode();
	//改变音量大小
	void changeVolume();
	//设置进度条
	void changeLoad();  //改变进度条
	void setLoadMaxValue(); //设置进度条最大值
	void setMusicPosition(); //设置音乐播放进度

	//播放状态发生变化
	void indexChanged();

	//对歌曲列表某一首歌双击
	void songDoubleClicked(QListWidgetItem *);
	//void musicStateChanged(QMediaPlayer::State _state);

protected:


signals:

};


#endif 
