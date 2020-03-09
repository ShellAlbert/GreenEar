#include "zvideoui.h"
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>
#include <QToolTip>
#include "zgblparam.h"
ZVideoUI::ZVideoUI(QWidget *parent) : QWidget(parent)
{
    QFont font;
    font.setFamily(g_GblPara.fontFamily);
    font.setPointSize(40);

    //battery.
    this->m_tbBattery=new QToolButton;
    this->m_tbBattery->setFont(font);
    this->m_tbBattery->setObjectName("ZVideoUIButton");
    this->ZSetBatteryState(Battery_Quarter);
    QObject::connect(this->m_tbBattery,SIGNAL(clicked()),this,SLOT(ZSlotButtonClicked()));

    //autofocus curve verify.
    this->m_tbVerify=new QToolButton;
    this->m_tbVerify->setFont(font);
    this->m_tbVerify->setText(QChar(0xf201));
    this->m_tbVerify->setObjectName("ZVideoUIButton");
    QObject::connect(this->m_tbVerify,SIGNAL(clicked()),this,SLOT(ZSlotButtonClicked()));

    //volume.
    this->m_tbVolume=new QToolButton;
    this->m_tbVolume->setFont(font);
    this->m_tbVolume->setText(QChar(0xf028));
    this->m_tbVolume->setObjectName("ZVideoUIButton");
    QObject::connect(this->m_tbVolume,SIGNAL(clicked()),this,SLOT(ZSlotButtonClicked()));


    this->m_hLayTop=new QHBoxLayout;
    this->m_hLayTop->addWidget(this->m_tbBattery);
    this->m_hLayTop->addStretch(1);
    this->m_hLayTop->addWidget(this->m_tbVerify);
    this->m_hLayTop->addWidget(this->m_tbVolume);

    //up.
    this->m_tbArrow[0]=new QToolButton;
    this->m_tbArrow[0]->setFont(font);
    this->m_tbArrow[0]->setText(QChar(0xf102));
    this->m_tbArrow[0]->setObjectName("ZVideoUIButton");
    QObject::connect(this->m_tbArrow[0],SIGNAL(clicked()),this,SLOT(ZSlotButtonClicked()));
    //left.
    this->m_tbArrow[1]=new QToolButton;
    this->m_tbArrow[1]->setFont(font);
    this->m_tbArrow[1]->setText(QChar(0xf100));
    this->m_tbArrow[1]->setObjectName("ZVideoUIButton");
    QObject::connect(this->m_tbArrow[1],SIGNAL(clicked()),this,SLOT(ZSlotButtonClicked()));
    //right.
    this->m_tbArrow[2]=new QToolButton;
    this->m_tbArrow[2]->setFont(font);
    this->m_tbArrow[2]->setText(QChar(0xf101));
    this->m_tbArrow[2]->setObjectName("ZVideoUIButton");
    QObject::connect(this->m_tbArrow[2],SIGNAL(clicked()),this,SLOT(ZSlotButtonClicked()));
    //down.
    this->m_tbArrow[3]=new QToolButton;
    this->m_tbArrow[3]->setFont(font);
    this->m_tbArrow[3]->setText(QChar(0xf103));
    this->m_tbArrow[3]->setObjectName("ZVideoUIButton");
    QObject::connect(this->m_tbArrow[3],SIGNAL(clicked()),this,SLOT(ZSlotButtonClicked()));

    this->m_gridLayArrow=new QGridLayout;
    this->m_gridLayArrow->addWidget(this->m_tbArrow[0],0,1,1,1);
    this->m_gridLayArrow->addWidget(this->m_tbArrow[1],1,0,1,1);
    this->m_gridLayArrow->addWidget(this->m_tbArrow[2],1,2,1,1);
    this->m_gridLayArrow->addWidget(this->m_tbArrow[3],2,1,1,1);

    this->m_hLayBtm=new QHBoxLayout;
    this->m_hLayBtm->addStretch(1);
    this->m_hLayBtm->addLayout(this->m_gridLayArrow);

    this->m_vLayMain=new QVBoxLayout;
    this->m_vLayMain->addLayout(this->m_hLayTop);
    this->m_vLayMain->addStretch(1);
    this->m_vLayMain->addLayout(this->m_hLayBtm);
    this->setLayout(this->m_vLayMain);


    //start timer to hide operating buttons.
    this->m_timer=new QTimer;
    QObject::connect(this->m_timer,SIGNAL(timeout()),this,SLOT(ZSlotTimeout()));
    this->m_timer->start(5000);//1000ms*10.
}
ZVideoUI::~ZVideoUI()
{
    for(qint32 i=0;i<4;i++)
    {
        delete this->m_tbArrow[i];
    }
    delete this->m_gridLayArrow;
    delete this->m_hLayBtm;

    delete this->m_tbBattery;
    delete this->m_tbVerify;
    delete this->m_tbVolume;
    delete this->m_hLayTop;

    delete this->m_vLayMain;
}
QSize ZVideoUI::sizeHint() const
{
    return QSize(400,300);
}
void ZVideoUI::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(this->rect(),Qt::white);
}
void ZVideoUI::mousePressEvent(QMouseEvent *e)
{
    if(!this->m_timer->isActive())
    {
        for(qint32 i=0;i<4;i++)
        {
            this->m_tbArrow[i]->setVisible(true);
        }
        this->m_tbVerify->setVisible(true);
        this->m_tbVolume->setVisible(true);

        this->m_timer->start(5000);//1000ms*10.
    }

    //remember the start point.
    this->m_bMousePressed=true;
    this->m_PosOld=e->pos();

    QWidget::mousePressEvent(e);
}
void ZVideoUI::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
}
void ZVideoUI::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->m_bMousePressed)
    {
        this->m_PosNew=e->pos();
        qDebug()<<this->m_PosOld<<this->m_PosNew;

        //left part screen.
        if((this->m_PosOld.x()<this->width()/2)&&(this->m_PosNew.x()<this->width()/2))
        {
            if(this->m_PosNew.y()<this->m_PosOld.y())
            {
                //from top to down.
                qDebug()<<"local volume increase";
                emit this->ZSigVolumeCtl(0,true);
            }else if(this->m_PosNew.y()>this->m_PosOld.y())
            {
                //from down to top.
                qDebug()<<"local volume decrease";
                emit this->ZSigVolumeCtl(0,false);
            }
        }
        //right part.
        if((this->m_PosOld.x()>this->width()/2)&&(this->m_PosNew.x()>this->width()/2))
        {
            if(this->m_PosNew.y()<this->m_PosOld.y())
            {
                //from top to down.
                qDebug()<<"device volume increase";
                emit this->ZSigVolumeCtl(1,true);
            }else if(this->m_PosNew.y()>this->m_PosOld.y())
            {
                //from down to top.
                qDebug()<<"device volume decrease";
                emit this->ZSigVolumeCtl(1,false);
            }
        }
        //reset flag.
        this->m_bMousePressed=false;
    }
    QWidget::mouseReleaseEvent(e);
}
void ZVideoUI::ZSetBatteryState(Battery_State state)
{
    switch(state)
    {
    case Battery_Empty:
        this->m_tbBattery->setText(QChar(0xf244));
        break;
    case Battery_Quarter:
        this->m_tbBattery->setText(QChar(0xf243));
        break;
    case Battery_Half:
        this->m_tbBattery->setText(QChar(0xf242));
        break;
    case Battery_Three_Quarter:
        this->m_tbBattery->setText(QChar(0xf241));
        break;
    case Battery_Full:
        this->m_tbBattery->setText(QChar(0xf240));
        break;
    default:
        break;
    }
}
void ZVideoUI::ZSlotButtonClicked()
{
    QToolButton *tb=qobject_cast<QToolButton*>(this->sender());
    if(tb==this->m_tbArrow[0])
    {
        emit this->ZSigArrowClicked(0);
    }else if(tb==this->m_tbArrow[1])
    {
        emit this->ZSigArrowClicked(1);
    }else if(tb==this->m_tbArrow[2])
    {
        emit this->ZSigArrowClicked(2);
    }else if(tb==this->m_tbArrow[3])
    {
        emit this->ZSigArrowClicked(3);
    }else if(tb==this->m_tbBattery)
    {

    }else if(tb==this->m_tbVerify)
    {

    }else if(tb==this->m_tbVolume)
    {
        emit this->ZSigVolumeSet();
    }
    this->m_timer->setInterval(5000);
}
void ZVideoUI::ZSlotTimeout()
{
    this->m_timer->stop();
    for(qint32 i=0;i<4;i++)
    {
        this->m_tbArrow[i]->setVisible(false);
    }
    this->m_tbVerify->setVisible(false);
    this->m_tbVolume->setVisible(false);
}
