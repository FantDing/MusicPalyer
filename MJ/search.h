#ifndef SEARCH_H
#define SEARCH_H
#include<QString>
#include <QWidget>
#include<QtNetwork>
#include<QJsonDocument>
#include<QKeyEvent>
const QString urlFirstPart("https://route.showapi.com/213-1?");
const QString urlFirstPartUrl("https://route.showapi.com/213-2?");
const QString secret("979fabc9da9d42f8b17d920673f2f4b6");
namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT

public:
    Ui::Search *ui;
    explicit Search(QWidget *parent = 0);
    ~Search();
    void connection();
    void getSecondPart(int mark,QString);
    void getSign(int mark);
    void getLyrics();
    QJsonDocument jsonDoc;
    QStringList strNetList;
    QStringList strNetSongName;
    QStringList strLyrics;
    QString lyrics;
    QNetworkAccessManager*manager;
    int mark;//区分处理歌词，还是搜索
    QString urlSecondPart;
    void keyPressEvent(QKeyEvent*);

public slots:
    void handleSubmitInfo();
    void replyFinished(QNetworkReply*reply);
private:
    //网络部分

    QNetworkReply* reply;
    //QString urlSecondPart_withID;

   QString sendedUrl;
    QString strSign;
    QString currentMusicID;

//当解析结束，发送信号给widget
signals:
    void finishParseJson();

};

#endif // SEARCH_H
