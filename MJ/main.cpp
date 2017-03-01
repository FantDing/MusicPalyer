#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
     Widget w;
    //w.setAttribute(Qt::WA_TranslucentBackground);
    w.show();

    return a.exec();
}
