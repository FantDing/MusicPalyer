#include "search.h"
#include "ui_search.h"
#include<QDateTime>
#include"widget.h"
//extern Widget w;
Search::Search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
   // this->setWindowFlags(Qt::Tool| Qt::X11BypassWindowManagerHint);
    ui->setupUi(this);
    mark=1;
    connection();
    this->setFocusPolicy(Qt::ClickFocus);
    //美化
    //按钮
    ui->BtnSearch_submit->setIcon(QIcon(":/img/bg8.png"));
    ui->BtnSearch_submit->setIconSize(ui->BtnSearch_submit->size());
    ui->BtnSearch_submit->setFlat(1);

   // connect(&w,SIGNAL(destroyed(QObject*)),this,SLOT(close()));

}

Search::~Search()
{
    delete ui;

}

void Search::connection()
{
    connect(ui->BtnSearch_submit,SIGNAL(clicked(bool)),
            this,SLOT(handleSubmitInfo()));

}

void Search::getSecondPart(int mark, QString temp)
{
    QString key=ui->LeSearch_keyword->text();

    QString pageNum=ui->LeSearch_page->text();
    QDateTime time=QDateTime::currentDateTime();
    QString strTime=time.toString("yyyyMMddhhmmss");
    if(mark==1)//处理歌词
        urlSecondPart="keyword="+key+
            "&page="+pageNum+
            "&showapi_appid=9656&showapi_timestamp="+strTime;
    else if(mark==2)//处理搜索
    {
        currentMusicID=temp;
        urlSecondPart="musicid="+temp+
                "&showapi_appid=9656&showapi_timestamp="+strTime;
    }



}

void Search::getSign(int mark)
{
    QString tempSecondStr=urlSecondPart;
    QString source=tempSecondStr.remove('=');
    source.remove('&');
    source=source+secret;

    //生成MD5
    QByteArray ba,bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(source);
    md.addData(ba);
    bb=md.result();
    strSign.clear();
    strSign.append(bb.toHex());
    if(mark==1)//处理搜索
        sendedUrl=urlFirstPart+urlSecondPart+
            "&showapi_sign="+strSign;
    else if(mark==2)//处理歌词
        sendedUrl=urlFirstPartUrl+urlSecondPart+
            "&showapi_sign="+strSign;

}

void Search::getLyrics()
{
    manager->get(QNetworkRequest(QUrl(sendedUrl)));
}

void Search::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Enter||e->key()==Qt::Key_Return)
        handleSubmitInfo();

}

void Search::replyFinished(QNetworkReply *reply)
{
    if(mark==1)//处理搜索
    {

        strNetList.clear();
        strNetSongName.clear();
        if(reply->error()==QNetworkReply::NoError)
        {
           // qDebug()<<"wei";
            QByteArray json=reply->readAll();
            QJsonParseError error;
            jsonDoc=QJsonDocument::fromJson(json,&error);
            //逐层解析Json
            QJsonObject Jobj=jsonDoc.object();
            QJsonObject secondLayer= Jobj.take("showapi_res_body").toObject();
            QJsonObject thirdLayer= secondLayer.take("pagebean").toObject();
            QJsonArray contentList=thirdLayer.take("contentlist").toArray();
            for(int i=0;i<contentList.size();i++)
            {
                QJsonObject obj=contentList.at(i).toObject();

                this->strNetSongName<<(obj.take("singername").toString()+"-"+
                                          obj.take("songname").toString()+"-"+
                                      obj.take("albumname").toString());
                this->strNetList<<obj.take("downUrl").toString();
                this->strLyrics<<QString::number(obj.take("songid").toInt());
            }
            //发送信号给widget

            emit finishParseJson();
        }
    }
    else if(mark==2)
    {
        QByteArray json=reply->readAll();
        QJsonDocument jsonDoc=QJsonDocument::fromJson(json);
        QJsonObject obj1=jsonDoc.object();
        QJsonObject obj2=obj1.take("showapi_res_body").toObject();
        lyrics=obj2.take("lyric").toString();
        qDebug()<<lyrics;

        //保存到文件
        QString path="D:\\"+currentMusicID+".txt";
        QFile file(path);
            file.open(QIODevice::WriteOnly|QIODevice::Text);
            if(!file.exists())
                file.close();
            else
            {
            QTextStream stream(&file);
            stream<<lyrics;

            }

    }
}





void Search::handleSubmitInfo()
{

    getSecondPart(1,"1");
    getSign(1);
    if(mark==1)
    {
    manager=new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(sendedUrl)));
    }
}
