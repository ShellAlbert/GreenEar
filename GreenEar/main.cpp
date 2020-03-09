#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QDebug>
#include "greenear.h"
#include "zgblparam.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //generate example data file.
    //create it if it isnot exist.
    //d2e:distance to encoder.
    QFile fileExample("example.d2e");
    if(!fileExample.exists())
    {
        if(fileExample.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            char *exampleData="31,10439\n76,12681\n152,16656\n240,30951\n300,32032\n400,64203\n";
            fileExample.write(exampleData,strlen(exampleData));
            fileExample.close();
        }
    }

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
