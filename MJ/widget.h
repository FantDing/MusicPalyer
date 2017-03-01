#ifndef WIDGET_H
#define WIDGET_H
#include <QMediaPlayer>
#include <QWidget>
#include <QMediaPlaylist>
#include<QTableWidget>
#include<QMenu>
#include<QTimer>
#include<QMap>
#include<QEvent>
#include<QMouseEvent>
#include"search.h"

#include<QtNetwork>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void connections();
    void initTable();
    void initLooking();
    void initPrograme();
    QString qintToTime(QString);
    void setTabListTransparent();
    void ParseStringList(QStringList filelist_new);
    void ParseStringList_fromNet(QStringList filelistNet);

    //处理歌词


private slots:
    void handleOpenfiles();
    void handlePrev();
    void handleNext();
    void handlePlay();
    void handleSliToPlayer();
    void handlePlayerToSli();
    void handleDialvToPlayer();
    void handleSongInfo();

    void handleChangeBackground();//换背景
    void saveBackgroundToFile();

    void handlePlayMode();
    void handleRandomMode();
    void handleRepeatMode();
    void handleListLoopMode();

    void handlePlayIcon(QMediaPlayer::State state);

    void deletesong();
    void deleteall();
    void on_tab_playlist_cellDoubleClicked(int row, int column);
    void on_tab_playlist_customContextMenuRequested(const QPoint &pos);
    //处理歌词
    void handleLrcToLabel(qint64 songPos);
    void fromTextToLrcmap();//将时间-歌词对保存至map
    void handleFontColor(int value);
    //处理delete new
    void handlePlaylistUpdate();
    //表单保存
    void handleSaveListToFile();
    void closeEvent(QCloseEvent* e);
    //处理网络
    void handleShowSeaWid();
    void handleParseOver();
    void handlePlayList_2DoubleClicked(int row, int column);
   //download lyrics void handleCurrentIndexChanged(int pos);
    void moveEvent(QMoveEvent*);
    void hideEvent(QHideEvent*);
    void showEvent(QShowEvent*);
    //frameless
    void mousePressEvent(QMouseEvent*e);
    void mouseMoveEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent*e);




private:
    Ui::Widget *ui;

    QMediaPlayer* player;

    QMediaPlaylist* playlist;
    QMediaPlaylist* pre_playlist;
    QMediaPlaylist* now_playlist;

    int curIndex;
    QMenu*menu;
    QTimer* timer;
    QStringList filelist;
    QString backgroundPath;
    //处理歌词
    QMap<qint64,QString> mapLrc;
    //处理网络
    Search* searchWidget;
    QMediaPlaylist* netPlayList;//new
    QPoint movePos;
    bool leftPress;
    bool preSearchwidState;
    QString currentMusicID;
    //frameless
    bool pressed;
    QPoint windowToMouse;





};

#endif // WIDGET_H
