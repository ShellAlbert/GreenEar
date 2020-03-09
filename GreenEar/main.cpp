#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QDebug>
#include "greenear.h"
#include "zgblparam.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //load qss skin.
    QFile fileQss(":/skinfile/skin/default.qss");
    if(fileQss.open(QIODevice::ReadOnly))
    {
        QByteArray baQss=fileQss.readAll();
        app.setStyleSheet(baQss);
        fileQss.close();
    }

    //load font file.
    int fontId=QFontDatabase::addApplicationFont(":/fontfile/font/fontawesome-webfont.ttf");
    QStringList fontFamilies=QFontDatabase::applicationFontFamilies(fontId);
    qDebug()<<"fontFamilies.size()"<<fontFamilies.size();
    g_GblPara.fontFamily=fontFamilies.at(0);


    GreenEar ear;
    ear.ZStart();

    return app.exec();
}
