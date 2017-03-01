#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QStringList>
#include<QDebug>
#include<QMediaContent>
#include<QString>
#include<QIcon>
#include <QFileInfo>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);

    player=new QMediaPlayer(this);
    playlist=new QMediaPlaylist(this);
    player->setPlaylist(playlist);


    player->setVolume(50);
    ui->dial_volumn->setValue(50);


    timer=new QTimer;
    timer->setInterval(30);

    this->setMouseTracking(true);

    //读入表单
    QFile file("D:\\list.rmj");
   file.open(QIODevice::ReadOnly|QIODevice::Text);

    QTextStream stream(&file);
    while(!stream.atEnd())
    {

        filelist<<stream.readLine();
    }
    ParseStringList(filelist);
    now_playlist=playlist;





   setTabListTransparent();
    connections();
    initPrograme();
    initTable();
    initLooking();

    //background file
    QFile Bfile("D:\\background.rmj");
    bool isSuccess=Bfile.open(QIODevice::ReadOnly|QIODevice::Text);

    QTextStream Bstream(&Bfile);
    backgroundPath=Bstream.readLine();
    if(isSuccess&&backgroundPath.contains("."))
    {

        QImage image=QImage(backgroundPath);
        QBrush brush=QBrush(image.scaled(this->size()));
        QPalette palette;
        palette.setBrush(QPalette::Background,brush);
        this->setPalette(palette);

    }
    else
    {
        QBrush brush(QImage(":/img/background1.jpg").scaled(this->size()));
        QPalette palette;
        palette.setBrush(QPalette::Window,brush);
        this->setPalette(palette);

    }
    //网络

    searchWidget=new Search;//没有释放
    netPlayList=new QMediaPlaylist(this);
    connect(searchWidget,SIGNAL(finishParseJson()),this,SLOT(handleParseOver()));
    connect(ui->tab_playlist_2,SIGNAL(cellDoubleClicked(int,int)),
            this,SLOT(handlePlayList_2DoubleClicked(int,int)));
    connect(this,SIGNAL(destroyed(QObject*)),searchWidget,SLOT(deleteLater()));
//    connect(this->netPlayList,SIGNAL(currentIndexChanged(int)),
//this,SLOT(handleCurrentIndexChanged(int)));

    //搜索按钮美化
    ui->btn_search->setIcon(QIcon(":/img/bs1"));
    ui->btn_search->setIconSize(ui->btn_search->size());
    ui->btn_search->setFlat(1);
    this->preSearchwidState=false;
    //字体调整
    ui->sli_fontColor->setMaximum(280);
    //connect(this->playlist,SIGNAL())
    connect(ui->sli_fontColor,SIGNAL(valueChanged(int)),this,SLOT(handleFontColor(int)));
    //frameless

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->pressed=false;
}

Widget::~Widget()
{
    delete ui;
    delete player;
    delete playlist;
    delete menu;
    //网络部分

    //delete searchWidget;
    delete netPlayList;
}



/*
void Widget::handleOpenfiles()
{
     static QString path="D:\\";
    filelist=QFileDialog::getOpenFileNames(this,"选择音乐",
                                           QString(path),QString("MP3 WMA(*.mp3 *.wma)"));

    if(!filelist.isEmpty())
    {
        foreach (QString filename, filelist)
        {

           //测试 this->fromTextToLrcmap(filename);

            QMediaContent media=QMediaContent(QUrl::fromLocalFile(filename));
            this->playlist->addMedia(media);



            //插入一行
            ui->tab_playlist->insertRow(ui->tab_playlist->rowCount());

            //插入行项
            for(int i=0;i<ui->tab_playlist->columnCount();i++)
               ui->tab_playlist->setItem(ui->tab_playlist->rowCount()-1,i,new QTableWidgetItem);
            //在项中添加内容
            ui->tab_playlist->item(ui->tab_playlist->rowCount()-1,0)
                   ->setText(QFileInfo(filename).baseName());
        }

    }
    path="";

}
*/

void Widget::handleOpenfiles()
{
     static QString path="D:\\";
    QStringList filelist_new=QFileDialog::getOpenFileNames(this,"选择音乐",
                                           QString(path),QString("MP3 WMA(*.mp3 *.wma)"));
    ParseStringList(filelist_new);
    filelist=filelist+filelist_new;
    now_playlist=playlist;//new
    path="";

}

void Widget::handlePrev()
{
    curIndex--;
    if(curIndex<0)
        return;
    //暂停的时候
    if(player->state()==QMediaPlayer::PausedState)
        ui->sld_process->setValue(0);
    player->playlist()->previous();
    //playlist->setCurrentIndex(playlist->currentIndex()+1);
   // curIndex--;
    ui->tab_playlist->selectRow(playlist->currentIndex());


}

void Widget::handleNext()
{
    //if(filelist.)
    //暂停的时候
    if(player->state()==QMediaPlayer::PausedState)
        ui->sld_process->setValue(0);
    player->playlist()->next();
    ui->tab_playlist->selectRow(playlist->currentIndex());
}

void Widget::handlePlay()
{
    QMediaPlaylist* tempList=player->playlist();
    if(tempList->isEmpty())
        return;
    if(player->state()==QMediaPlayer::PlayingState)
    {
        player->pause();
        timer->stop();

        ui->btn_play->setIcon(QIcon(":/img/play2"));
        ui->btn_play->setIconSize(ui->btn_play->size());
        ui->btn_play->setFlat(true);

    }
    else
    {
        player->play();
        timer->start();
        ui->btn_play->setIcon(QIcon(":/img/pause2"));
        ui->btn_play->setIconSize(ui->btn_play->size());
        ui->btn_play->setFlat(true);

    }
}

void Widget::handleSliToPlayer()
{
     qint64 value=((1.0)*ui->sld_process->value()/ui->sld_process->maximum())
             *this->player->duration();
     this->player->setPosition(value);
}

void Widget::handlePlayerToSli()
{
    qint64 value=((1.0)*this->player->position()/
            this->player->duration())*ui->sld_process->maximum();
    ui->sld_process->setValue(value);

    //时间
    QString currentTime;
    currentTime=currentTime.setNum( long long(this->player->position()));
    ui->lab_currenttime->setText(qintToTime(currentTime));
}

void Widget::handleDialvToPlayer()
{
    int value=ui->dial_volumn->value();
    player->setVolume(value);
}

void Widget::handleSongInfo()
{


    int pos=playlist->currentIndex();
    playlist=now_playlist;
   // playlist->setCurrentIndex(pos);

    //歌词相关
    ui->lab_lrc1->clear();
    ui->lab_lrc2->clear();
    ui->lab_lrc0->clear();
if(player->playlist()==playlist)
    this->fromTextToLrcmap();
    //字体相关
   QString song=player->metaData("Title").toString();
    QString singer=player->metaData("Author").toString();
    QString album=player->metaData("AlbumTitle").toString();
    ui->lab_song->setText("歌曲: "+song);
    ui->lab_singer->setText("歌手: "+singer);
    ui->lab_album->setText("专辑: "+album);


   // 时间显示
    QString duration=this->qintToTime(player->metaData("Duration").toString());
    ui->lab_duration->setText(duration);

    QString currentTime;
    currentTime=currentTime.setNum( this->player->position());
    ui->lab_currenttime->setText(qintToTime(currentTime));


    //专辑图片显示


}


void Widget::handleChangeBackground()
{
    static QString path="D:\\";
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),path,tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QFileInfo fInfo=QFileInfo(filename);
    QString name=fInfo.absoluteFilePath();
    QImage image=QImage(name);
    QBrush brush=QBrush(image.scaled(this->size()));
    QPalette palette;
    palette.setBrush(QPalette::Background,brush);
    this->setPalette(palette);
     path="";

     //写入文件
     backgroundPath=name;


}

void Widget::saveBackgroundToFile()
{
    QFile file("D:\\background.rmj");
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    if(!file.exists())
        file.close();
    else
    {
    QTextStream stream(&file);
    stream<<this->backgroundPath;
    }
}

void Widget::handlePlayMode()
{
    QMenu menu;

    menu.addAction(QIcon(":/img/random_menu.png"),"随机播放",this,SLOT(handleRandomMode()));
    menu.addAction(QIcon(":/img/singleloop_menu.png"),"单曲循环",this,SLOT(handleRepeatMode()));
    menu.addAction(QIcon(":/img/listrange_menu.png"),"顺序播放",this,SLOT(handleListLoopMode()));


    menu.move(cursor().pos());
    menu.exec();


}

void Widget::handleRandomMode()
{

   player->playlist()->setPlaybackMode(QMediaPlaylist::Random);
   ui->btn_PlayMode->setStyleSheet("QPushButton#btn_PlayMode{border-image: url(:/img/random4.png)}");
}

void Widget::handleRepeatMode()
{
    player->playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    ui->btn_PlayMode->setStyleSheet("QPushButton#btn_PlayMode{border-image: url(:/img/singleloop.png)}");
}

void Widget::handleListLoopMode()
{
    player->playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
    ui->btn_PlayMode->setStyleSheet("QPushButton#btn_PlayMode{border-image: url(:/img/listrange.png)}");
}



void Widget::handlePlayIcon(QMediaPlayer::State state)
{
    QMediaPlayer::State state1=player->state();
    if(state1==QMediaPlayer::PlayingState)
       ui->btn_play->setStyleSheet("QPushButton#btn_play:hover{border-image: url(:/img/pause_hover2.png);}");
    if(state1==QMediaPlayer::PausedState)
       ui->btn_play->setStyleSheet("QPushButton#btn_play:hover{border-image: url(:/img/play_hover2.png);}");
}

void Widget::connections()
{
    //title bar event
    connect(ui->btn_close,SIGNAL(clicked(bool)),this,
            SLOT(close()));
    connect(ui->btn_minimum,SIGNAL(clicked(bool)),this,
            SLOT(showMinimized()));
    //connect(this,SIGNAL(destroyed()),this,SLOT(handleSaveListToFile()));

    connect(ui->btn_openfiles,SIGNAL(clicked()),this,SLOT(handleOpenfiles()));
    connect(ui->btn_next,SIGNAL(clicked()),this,SLOT(handleNext()));
    connect(ui->btn_play,SIGNAL(clicked()),this,SLOT(handlePlay()));

    connect(this->player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(handlePlayIcon(QMediaPlayer::State)));
    //handle playmode
    connect(this->ui->btn_PlayMode,SIGNAL(clicked()),this,SLOT(handlePlayMode()));
    connect(ui->btn_skin,SIGNAL(clicked()),this,SLOT(handleChangeBackground()));

    connect(ui->btn_prev,SIGNAL(clicked()),this,SLOT(handlePrev()));
    connect(ui->sld_process,SIGNAL(sliderMoved(int)),this,SLOT(handleSliToPlayer()));
    connect(this->timer,SIGNAL(timeout()),this,SLOT(handlePlayerToSli()));
    connect(ui->dial_volumn,SIGNAL(valueChanged(int)),this,SLOT(handleDialvToPlayer()));
    connect(this->player,SIGNAL(metaDataChanged()),this,SLOT(handleSongInfo()));
//处理删除歌曲导致的问题new


    //处理网络搜索按钮
    connect(ui->btn_search,SIGNAL(clicked(bool)),this,SLOT(handleShowSeaWid()));

    //处理歌词
   connect(this->player,SIGNAL(positionChanged(qint64)),this,SLOT(handleLrcToLabel(qint64)));
}

void Widget::initTable()
{
    ui->tab_playlist->setContextMenuPolicy(Qt::CustomContextMenu);
    menu=new QMenu;

    menu->addAction("添加歌曲",this,SLOT(handleOpenfiles()));
    menu->addAction("删除歌曲",this,SLOT(deletesong()));


    menu->addAction("清空列表",this,SLOT(deleteall()));
}

void Widget::initLooking()
{
    this->setAutoFillBackground(true);


    ui->tabWidget->setCurrentIndex(1);
    ui->btn_PlayMode->setStyleSheet("QPushButton#btn_PlayMode{border-image:url(:/img/listrange.png)}");


    ui->btn_next->setIcon(QIcon(":/img/next2"));
    ui->btn_next->setIconSize(ui->btn_next->size());
    ui->btn_next->setFlat(true);

    ui->btn_prev->setIcon(QIcon(":/img/prev2"));
    ui->btn_prev->setIconSize(ui->btn_prev->size());
    ui->btn_prev->setFlat(true);

    ui->btn_play->setIcon(QIcon(":/img/play2"));
    ui->btn_play->setIconSize(ui->btn_play->size());
    ui->btn_play->setFlat(true);

    ui->btn_close->setIcon(QIcon(":/img/btn_close"));
    ui->btn_close->setIconSize(ui->btn_close->size());
    ui->btn_close->setFlat(true);

    ui->btn_minimum->setIcon(QIcon(":/img/btn_minimum"));
    ui->btn_minimum->setIconSize(ui->btn_close->size());
    ui->btn_minimum->setFlat(true);


    //打开文件
    ui->btn_openfiles->setStyleSheet("QPushButton#btn_openfiles{border-image:url(:/img/open.png)}");
    //换背景
    ui->btn_skin->setStyleSheet("QPushButton#btn_skin{border-image:url(:/img/skin.png)}");

#if 0
    QPixmap pixmap(":/img/cover1.jpg");
    this->ui->lab_cover->setPixmap(pixmap.scaled(ui->lab_cover->size()));
#endif
   // ui->tab_playlist->setStyleSheet("color:white");
    //当前歌曲信息显示居中
    ui->lab_album->setAlignment(Qt::AlignCenter);
    ui->lab_singer->setAlignment(Qt::AlignCenter);
    ui->lab_song->setAlignment(Qt::AlignCenter);

    //歌词居中显示
    ui->lab_lrc1->setAlignment(Qt::AlignCenter);
    ui->lab_lrc2->setAlignment(Qt::AlignCenter);
    ui->lab_lrc0->setAlignment(Qt::AlignCenter);

}

void Widget::initPrograme()
{
    this->setWindowIcon(QIcon(":/img/music.ico"));
    this->setWindowTitle("MJ player");
}

QString Widget::qintToTime(QString duratio)
{
    long long duration=duratio.toLongLong();

    /*
    int total=int(duration/1000);
    int seconds=0;
    total=int(total/60);
    int minutes=total;


    double temp =1.0*duration/1000/60-int(duration/1000/60);
    seconds=int(temp*60);
    */
    int minutes=int(duration/1000);
    minutes=int(minutes/60);

    double temp =1.0*duration/1000/60-int(duration/1000/60);
    int seconds=int(temp*60);


    QString minute;
    if(minutes<10)  minute='0'+minute.setNum(minutes);
        else    minute=minute.setNum(minutes);

    QString second;
    if(seconds<10) second='0'+second.setNum(seconds);
        else    second=second.setNum(seconds);


    QString time=minute+':'+second;
    return time;
}

void Widget::setTabListTransparent()
{
   ui->tab_playlist->setAttribute(Qt::WA_TranslucentBackground, true);
   ui->tab_playlist->setStyleSheet ("background-color:transparent");
   ui->tab_playlist->setShowGrid(false);
   ui->tab_playlist->verticalHeader()->setVisible(0);
   ui->tab_playlist->horizontalHeader()->setVisible(0);
   ui->tab_playlist->setFrameShape(QFrame::NoFrame);

   ui->tab_playlist_2->setAttribute(Qt::WA_TranslucentBackground, true);
   ui->tab_playlist_2->setStyleSheet ("background-color:transparent");
   ui->tab_playlist_2->setShowGrid(false);
   ui->tab_playlist_2->verticalHeader()->setVisible(false);
   ui->tab_playlist_2->horizontalHeader()->setVisible(false);
   ui->tab_playlist_2->setFrameShape(QFrame::NoFrame);

   ui->tabWidget->setStyleSheet("QTabWidget:pane{border-top:0px solid #e8f3f9;background:transparent;}");

}



void Widget::ParseStringList(QStringList filelist_new)
{
    if(!filelist_new.isEmpty())
    {
        foreach (QString filename, filelist_new)
        {

            QMediaContent media=QMediaContent(QUrl::fromLocalFile(filename));
            this->playlist->addMedia(media);
            //插入一行
            ui->tab_playlist->insertRow(ui->tab_playlist->rowCount());
            //插入行项
            for(int i=0;i<ui->tab_playlist->columnCount();i++)
               ui->tab_playlist->setItem(ui->tab_playlist->rowCount()-1,i,new QTableWidgetItem);
            //在项中添加内容
            ui->tab_playlist->item(ui->tab_playlist->rowCount()-1,0)
                   ->setText(QFileInfo(filename).baseName());
        }

    }
}

void Widget::ParseStringList_fromNet(QStringList filelistNet)
{



    ui->btn_play->setIcon(QIcon(":/img/play2"));
    ui->btn_play->setIconSize(ui->btn_play->size());
    ui->btn_play->setFlat(true);
    this->netPlayList->clear();
    for(int i=0;i<ui->tab_playlist_2->rowCount();)
        ui->tab_playlist_2->removeRow(0);


    if(!filelistNet.isEmpty())
    {
        foreach (QString filename, filelistNet)
        {

            //插入一行
            ui->tab_playlist_2->insertRow(ui->tab_playlist_2->rowCount());
            //插入行项
            for(int i=0;i<ui->tab_playlist_2->columnCount();i++)
               ui->tab_playlist_2->setItem(ui->tab_playlist_2->rowCount()-1,i,new QTableWidgetItem);
            //在项中添加内容
            ui->tab_playlist_2->item(ui->tab_playlist_2->rowCount()-1,0)
                   ->setText(filename);
        }

        foreach (QString url, searchWidget->strNetList)
        {
            QMediaContent media=QMediaContent(QUrl(url));
            this->netPlayList->addMedia(media);
        }

    }


}

void Widget::fromTextToLrcmap()
{
    //正则表达式匹配
    QRegExp time("\\[(\\d{2}):(\\d{2})\\.(\\d{2})\\]");
    mapLrc.clear();
    //打开文件
    QString path;
if(player->playlist()==this->playlist)
{
    int num=this->playlist->currentIndex();
    QString name=this->filelist.at(num);
    path=name.left(name.length()-4)+".lrc";
}
else if(player->playlist()==netPlayList)
{
    path="D:\\"+currentMusicID+".txt";
}
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    //直接从文件读取会出现乱码，从文本流中读取不会
    QTextStream in(&file);
    QString filetext=in.readAll();

    foreach (QString line,filetext.split("\n"))
    {
          int pos=time.indexIn(line);
          int songPos=time.cap(1).toInt()*60000 + time.cap(2).toInt()*1000 + time.cap(3).toInt()*10;
          while(pos!=-1)
          {
              mapLrc.insert(songPos,line.replace(time,""));
              pos=time.indexIn(line,pos+1);
          }
    }

}

void Widget::handleFontColor(int value)
{

    int r=((value)*4/3)%255;
    int g=(2*value/3)%255;
    int b=(value*3/4)%255;
    QColor fontColor(r,g,b);


    for(int i=0;i<ui->tab_playlist->rowCount();i++)
    {
        ui->tab_playlist->item(i,0)->setTextColor(fontColor);
    }
    for(int i=0;i<ui->tab_playlist_2->rowCount();i++)
    {
        ui->tab_playlist_2->item(i,0)->setTextColor(fontColor);
    }
    QPalette pa;
    pa.setColor(QPalette::WindowText,fontColor);
    ui->lab_album->setPalette(pa);
    ui->lab_singer->setPalette(pa);
    ui->lab_song->setPalette(pa);
    ui->lab_lrc0->setPalette(pa);
    ui->lab_lrc1->setPalette(pa);
    ui->lab_lrc2->setPalette(pa);

}

void Widget::handlePlaylistUpdate()
{

}

void Widget::handleSaveListToFile()
{
    QFile file("D:\\list.rmj");
    file.open(QIODevice::Text|QIODevice::WriteOnly);
    if(!file.exists())
        file.close();

    if(!file.isOpen())
         qDebug()<<"false";
    else
    {
        QTextStream stream(&file);
        foreach (QString line,this->filelist ) {

            stream<<line;
            stream<<"\n";
        }
    }
}

void Widget::closeEvent(QCloseEvent *e)
{
    handleSaveListToFile();
    this->saveBackgroundToFile();
    this->destroyed();

    //网络

}

void Widget::handleShowSeaWid()
{
    if(searchWidget->isHidden())
    {
        ui->btn_search->setIcon(QIcon(":/img/bs8"));
        ui->btn_search->setIconSize(ui->btn_search->size());
    this->searchWidget->show();
   searchWidget->move(this->geometry().x(),this->geometry().y()+this->height());
    }
    else
    {
        ui->btn_search->setIcon(QIcon(":/img/bs1"));
        ui->btn_search->setIconSize(ui->btn_search->size());
        this->searchWidget->hide();
        this->preSearchwidState=false;
    }

}

void Widget::handleParseOver()
{
    ParseStringList_fromNet(searchWidget->strNetSongName);

    ui->tabWidget->setCurrentIndex(2);
}

void Widget::handlePlayList_2DoubleClicked(int row, int)
{

    this->player->setPlaylist(netPlayList);
    this->netPlayList->setCurrentIndex(row);
    curIndex=row;
    qDebug()<<curIndex;

    timer->start();
    player->play();
    ui->btn_play->setIcon(QIcon(":/img/pause2"));
    ui->btn_play->setIconSize(ui->btn_play->size());
    ui->btn_play->setFlat(true);
}
/*
void Widget::handleCurrentIndexChanged(int pos)
{
    //set tableWidget's current index.(highlight current row)

    //download Lyrics
    ui->tab_playlist_2->setCurrentCell(pos,0);
    static int arr[2]={0};
    static int i=0;
    arr[i]=pos;
    i=(i+1)%2;
    if((pos!=0)||(arr[0]==arr[1]&&arr[1]==0))
    {
        //获取到歌词
        currentMusicID=this->searchWidget->strLyrics[pos];
        this->searchWidget->getSecondPart(2,currentMusicID);
        searchWidget->mark=2;
        searchWidget->getSign(2);
        searchWidget->getLyrics();
    }


}
*/
void Widget::moveEvent(QMoveEvent *)
{
    searchWidget->move(this->geometry().x(),this->geometry().y()+this->height());
}

void Widget::hideEvent(QHideEvent *e)
{

    if(!searchWidget->isHidden())
    {
        searchWidget->hide();
        this->preSearchwidState=true;
    }

}

void Widget::showEvent(QShowEvent *)
{
    if(this->preSearchwidState)
    {
        searchWidget->show();
    }
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        this->pressed=true;
         this->windowToMouse=e->globalPos()-this->pos();

    }
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if(pressed)
    {
        this->move(e->globalPos()-this->windowToMouse);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    this->pressed=false;
}





void Widget::on_tab_playlist_cellDoubleClicked(int row, int)
{
    player->setPlaylist(playlist);
    curIndex=row;
    playlist->setCurrentIndex(row);
    timer->start();
    player->play();
    ui->btn_play->setIcon(QIcon(":/img/pause2"));
    ui->btn_play->setIconSize(ui->btn_play->size());
    ui->btn_play->setFlat(true);
}

void Widget::deletesong()
{
    if(this->playlist->currentIndex()==curIndex)
    {
         ui->tab_playlist->removeRow(curIndex);
         playlist->removeMedia(curIndex);
         filelist.removeAt(curIndex);//new


    }
    else
    {
        ui->tab_playlist->removeRow(curIndex);//new
        now_playlist=playlist;
        now_playlist->removeMedia(curIndex);
        filelist.removeAt(curIndex);//new

    }

}

void Widget::deleteall()
{
    player->pause();
    ui->btn_play->setIcon(QIcon(":/img/play2"));
    ui->btn_play->setIconSize(ui->btn_play->size());
    ui->btn_play->setFlat(true);
    playlist->clear();
    now_playlist->clear();
    filelist.clear();
    for(int i=0;i<ui->tab_playlist->rowCount();)
        ui->tab_playlist->removeRow(0);
}

void Widget::on_tab_playlist_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem* item=ui->tab_playlist->itemAt(pos);
    curIndex=ui->tab_playlist->row(item);
// curIndex = ui->tab_playlist->rowAt(pos.y());
 menu->exec(QCursor::pos());
}

void Widget::handleLrcToLabel(qint64 songPos)
{
#if 0
    qint64 targetTime=0;
    foreach (qint64 gettedTime, mapLrc.keys())
    {
        QString time1;
        time1=time1.number(songPos);
        time1=this->qintToTime(time1);

        QString time2;
        time2=time2.number(gettedTime);
        time2=this->qintToTime(time2);

        qDebug()<<time1<<time2;


        //mapLrc.value(gettedTime);
        if(songPos>gettedTime)
        {
            targetTime=gettedTime;
            break;
        }
    }
    /*
    QString time;
    time=time.number(targetTime);
    time=this->qintToTime(time);

    qDebug()<<time<<lrc;
    */
    QString lrc=mapLrc.value(targetTime);
    ui->lab_lrc->setText(lrc);

#endif

    qint64 prev = 0,next = 0;
    foreach(qint64 value,mapLrc.keys())
    {
        if(songPos >= value)
            prev = value;
        else
        {
            next = value;
            break;
        }
    }



    QString currentLrc = mapLrc.value(prev);
    ui->lab_lrc1->setText(currentLrc);
     //new
     QMap<qint64,QString>::iterator p=mapLrc.find(prev);

    // QMap<qint64,QString>::iterator up=p-1;
     if(p!=mapLrc.begin())
         ui->lab_lrc0->setText( *(p-1));



        if(p!=mapLrc.end())
         {


           if((++p)!=mapLrc.end())
          {
                QString nextLrc=*(p);
                ui->lab_lrc2->setText(nextLrc);
          }
        }
     //ui->lab_lrc2->setText(nextLrc);
}
