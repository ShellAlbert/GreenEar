#include "zmainui.h"
#include <QApplication>
#include <QThread>
#include <QToolTip>
#include <QAndroidJniObject>
#include <QDebug>
#include "zgblparam.h"
#include "protocol/zmodbusprotocol.h"
#include "ui/zcurvedialog.h"
ZMainUI::ZMainUI(QWidget *parent):QWidget(parent)
{
    this->setWindowTitle(tr("BypassAPP"));
    this->setObjectName("ZMainUI");

    QFont font;
    font.setFamily(g_GblPara.fontFamily);
    font.setPointSize(40);

    this->m_barRSSI=new QProgressBar;
    this->m_barRSSI->setOrientation(Qt::Vertical);
    this->m_barRSSI->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    this->m_barRSSI->setRange(0,100);
    this->m_barRSSI->setValue(10);
    this->m_barRSSI->setFormat("RSSI");
    //QObject::connect(this->m_rssiUI,SIGNAL(ZSigClicked()),this,SLOT(ZSlotRSSI()));

    //auto focus.
    this->m_tbAutoFocus=new QToolButton;
    this->m_tbAutoFocus->setFont(font);
    this->m_tbAutoFocus->setText(QChar(0xf030));
    this->m_tbAutoFocus->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QObject::connect(this->m_tbAutoFocus,SIGNAL(clicked()),this,SLOT(ZSlotAutoFocus()));

    this->m_tbDistance=new QToolButton;
    this->m_tbDistance->setText(tr("---米"));
    this->m_tbDistance->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QObject::connect(this->m_tbDistance,SIGNAL(clicked()),this,SLOT(ZSlotDistance()));

    //+.
    this->m_tbPlus=new QToolButton;
    this->m_tbPlus->setFont(font);
    this->m_tbPlus->setText(QChar(0xf067));
    this->m_tbPlus->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QObject::connect(this->m_tbPlus,SIGNAL(clicked()),this,SLOT(ZSlotPlus()));

    //-.
    this->m_tbMinus=new QToolButton;
    this->m_tbMinus->setFont(font);
    this->m_tbMinus->setText(QChar(0xf068));
    this->m_tbMinus->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QObject::connect(this->m_tbMinus,SIGNAL(clicked()),this,SLOT(ZSlotMinus()));

    //video.
    this->m_tbRecord=new QToolButton;
    this->m_tbRecord->setFont(font);
    this->m_tbRecord->setText(QChar(0xf03d));
    this->m_tbRecord->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QObject::connect(this->m_tbRecord,SIGNAL(clicked()),this,SLOT(ZSlotRecord()));

    this->m_tbPlayback=new QToolButton;
    this->m_tbPlayback->setFont(font);
    this->m_tbPlayback->setText(QChar(0xf1c8));
    this->m_tbPlayback->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QObject::connect(this->m_tbPlayback,SIGNAL(clicked()),this,SLOT(ZSlotPlayback()));

    this->m_gridLayLeft=new QGridLayout;
    this->m_gridLayLeft->setContentsMargins(0,0,0,0);
    this->m_gridLayLeft->addWidget(this->m_barRSSI,1,0,1,2);
    this->m_gridLayLeft->addWidget(this->m_tbAutoFocus,2,0,1,2);
    this->m_gridLayLeft->addWidget(this->m_tbDistance,3,0,1,2);
    this->m_gridLayLeft->addWidget(this->m_tbPlus,4,0,1,1);
    this->m_gridLayLeft->addWidget(this->m_tbMinus,4,1,1,1);
    this->m_gridLayLeft->addWidget(this->m_tbRecord,5,0,1,2);
    this->m_gridLayLeft->addWidget(this->m_tbPlayback,6,0,1,2);

    this->m_videoUI=new ZVideoUI;
    QObject::connect(this->m_videoUI,SIGNAL(ZSigArrowClicked(qint32)),this,SLOT(ZSlotArrow(qint32)));
    QObject::connect(this->m_videoUI,SIGNAL(ZSigVolumeCtl(qint32,bool)),this,SLOT(ZSlotVolumeCtl(qint32,bool)));
    QObject::connect(this->m_videoUI,SIGNAL(ZSigVolumeSet()),this,SLOT(ZSlotVolumeSet()));
    QObject::connect(this->m_videoUI,SIGNAL(ZSigCurveUpdate()),this,SLOT(ZSlotCurveUpdate()));

    this->m_hLayMain=new QHBoxLayout;
    this->m_hLayMain->setContentsMargins(0,0,0,0);
    this->m_hLayMain->addLayout(this->m_gridLayLeft);
    this->m_hLayMain->addWidget(this->m_videoUI);
    this->m_hLayMain->setStretchFactor(this->m_gridLayLeft,0);
    this->m_hLayMain->setStretchFactor(this->m_videoUI,1);

    this->setLayout(this->m_hLayMain);

    //default device volume=0%.
    this->m_nDeviceVolume=0;
}

ZMainUI::~ZMainUI()
{
    delete this->m_barRSSI;
    delete this->m_tbAutoFocus;
    delete this->m_tbDistance;
    delete this->m_tbPlus;
    delete this->m_tbMinus;
    delete this->m_tbRecord;
    delete this->m_tbPlayback;
    delete this->m_gridLayLeft;

    delete this->m_videoUI;
    delete this->m_hLayMain;
}
void ZMainUI::ZBindFIFOFree(QMutex *mux,///<
                            QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                            QQueue<ZElement*> *fifo)
{
    this->m_muxFIFOFree=mux;
    this->m_waitFIFOFreeNotEmpty=waitFIFONotEmpty;
    this->m_waitFIFOFreeNotFull=waitFIFONotFull;
    this->m_FIFOFree=fifo;
}
void ZMainUI::ZBindFIFOTx(QMutex *mux,///<
                          QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                          QQueue<ZElement*> *fifo)
{
    this->m_muxFIFOTx=mux;
    this->m_waitFIFOTxNotEmpty=waitFIFONotEmpty;
    this->m_waitFIFOTxNotFull=waitFIFONotFull;
    this->m_FIFOTx=fifo;
}
void ZMainUI::ZBindFIFOAck(QMutex *mux,///<
                           QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                           QQueue<ZElement*> *fifo)
{
    this->m_muxFIFOAck=mux;
    this->m_waitFIFOAckNotEmpty=waitFIFONotEmpty;
    this->m_waitFIFOAckNotFull=waitFIFONotFull;
    this->m_FIFOAck=fifo;
}
void ZMainUI::closeEvent(QCloseEvent *e)
{
    e->ignore();
    g_GblPara.bExitFlag=true;
    emit this->ZSigCloseEvent();
}
void ZMainUI::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_A)
    {
        qDebug()<<"keyPressedEvent:timeout thread notify ui thread";

        //fetch Ack from AckFIFO,parse out,then move it to FreeFIFO.
        //make it reuseable.
        QList<ZElement*> list;
        this->m_muxFIFOAck->lock();
        while(this->m_FIFOAck->size()>0)
        {
            ZElement *ele=this->m_FIFOAck->dequeue();
            list.append(ele);
        }
        this->m_waitFIFOAckNotFull->wakeAll();
        this->m_muxFIFOAck->unlock();

        //process all Ack Element.
        qDebug()<<"process all Ack Element.";
        for(qint32 i=0;i<list.size();i++)
        {
            ZElement *ele=list.at(i);
            this->ZParseAckFrame(ele);
        }

        //move it to FreeFIFO to make it reusable.
        this->m_muxFIFOFree->lock();
        for(qint32 i=0;i<list.size();i++)
        {
            ZElement *elec=list.at(i);
            //caution here: we do not judge if FIFOFree is full or not.
            this->m_FIFOFree->enqueue(elec);
        }
        this->m_waitFIFOFreeNotEmpty->wakeAll();
        this->m_muxFIFOFree->unlock();

        return;
    }
    QWidget::keyPressEvent(e);
}
bool ZMainUI::ZPutEle2FIFO(QByteArray baData,qint32 nRegAddr)
{
    if(baData.size()>TX_RX_MAX_SIZE)
    {
        QToolTip::showText(this->rect().center(),tr("错误:发送命令长度过大!"));
        return false;
    }
    //we only move pointer(address) from FIFO to FIFO.

    //1.get a free element from free fifo.
    this->m_muxFIFOFree->lock();
    if(this->m_FIFOFree->isEmpty())
    {
        if(!this->m_waitFIFOFreeNotEmpty->wait(this->m_muxFIFOFree,5000))
        {
            this->m_muxFIFOFree->unlock();
            QToolTip::showText(this->rect().center(),tr("错误:发送超时,请重试!"));
            return false;
        }
    }

    ZElement *ele=this->m_FIFOFree->dequeue();
    this->m_waitFIFOFreeNotFull->wakeAll();
    this->m_muxFIFOFree->unlock();

    //2.copy data into fifo element.
    ele->widget=this;
    ele->nRegAddr=nRegAddr;
    ele->nTimeoutTick=5;
    memcpy(ele->baTxData->data(),baData.data(),baData.size());
    ele->nTxLen=baData.size();

    //3.put element into TxFIFO.
    if(this->m_muxFIFOTx->tryLock())
    {
        if(this->m_FIFOTx->size()==FIFO_DEPTH)
        {
            if(!this->m_waitFIFOTxNotFull->wait(this->m_muxFIFOTx,5000))
            {
                this->m_muxFIFOTx->unlock();
                QToolTip::showText(this->rect().center(),tr("错误:发送超时,请重试!"));
                return false;
            }
        }
        this->m_FIFOTx->enqueue(ele);
        this->m_waitFIFOTxNotEmpty->wakeAll();
        this->m_muxFIFOTx->unlock();
        return true;
    }else{
        QRect rectTips;
        rectTips.setX(this->width()/2);
        rectTips.setY(this->height()/2);
        rectTips.setWidth(200);
        rectTips.setHeight(50);

        QToolTip::showText(this->rect().center(),tr("错误:发送超时,请重试!"),this,rectTips,2000);
        return false;
    }
}
void ZMainUI::ZSlotBattery()
{
    ZModBusProtocol protocol;
    QByteArray cmd=protocol.ZGetCmd_Rd_Battery();
    this->ZPutEle2FIFO(cmd,ZModBusProtocol::nReg_Battery_R);
}
void ZMainUI::ZSlotRSSI()
{
    ZModBusProtocol protocol;
    QByteArray cmd=protocol.ZGetCmd_Rd_RSSI();
    this->ZPutEle2FIFO(cmd,ZModBusProtocol::nReg_RSSI_R);
}
void ZMainUI::ZSlotAutoFocus()
{
    ZModBusProtocol protocol;
    QByteArray cmd=protocol.ZGetCmd_Wr_AutoFocus();
    this->ZPutEle2FIFO(cmd,ZModBusProtocol::nReg_LenMotorCtl_W);
}
void ZMainUI::ZSlotDistance()
{
    ZModBusProtocol protocol;
    QByteArray cmd=protocol.ZGetCmd_Rd_Distance();
    this->ZPutEle2FIFO(cmd,ZModBusProtocol::nReg_Distance_R);
}
void ZMainUI::ZSlotPlus()
{
    ZModBusProtocol protocol;
    //true:clockwise=plus.
    QByteArray cmd=protocol.ZGetCmd_Wr_LenMotorCtl(true);
    this->ZPutEle2FIFO(cmd,ZModBusProtocol::nReg_LenMotorCtl_W);
}
void ZMainUI::ZSlotMinus()
{
    ZModBusProtocol protocol;
    //false:anti-clockwise=minus.
    QByteArray cmd=protocol.ZGetCmd_Wr_LenMotorCtl(false);
    this->ZPutEle2FIFO(cmd,ZModBusProtocol::nReg_LenMotorCtl_W);
}
void ZMainUI::ZSlotRecord()
{

}
void ZMainUI::ZSlotPlayback()
{

}
void ZMainUI::ZSlotArrow(qint32 direction)
{
    ZModBusProtocol protocol;
    QByteArray cmd;

    switch(direction)
    {
    case 0://up.
        cmd=protocol.ZGetCmd_Wr_2DBracketCtl(0);
        break;
    case 1://left.
        cmd=protocol.ZGetCmd_Wr_2DBracketCtl(1);
        break;
    case 2://right.
        cmd=protocol.ZGetCmd_Wr_2DBracketCtl(2);
        break;
    case 3://down.
        cmd=protocol.ZGetCmd_Wr_2DBracketCtl(3);
        break;
    default:
        break;
    }
    this->ZPutEle2FIFO(cmd,ZModBusProtocol::nReg_2DBracketCtl_W);
    return;
}
bool ZMainUI::ZParseAckFrame(ZElement *ele)
{
    QRect rectTips;
    rectTips.setX(this->width()/2);
    rectTips.setY(this->height()/2);
    rectTips.setWidth(200);
    rectTips.setHeight(50);
    if(ele->nTimeoutTick==0)
    {
        qDebug()<<"timeout Ack";
        switch(ele->nRegAddr)
        {
        case ZModBusProtocol::nReg_Battery_R:
            QToolTip::showText(this->rect().center(),tr("错误:读Battery失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_RSSI_R:
            QToolTip::showText(this->rect().center(),tr("错误:读RSSI失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_Distance_R:
            QToolTip::showText(this->rect().center(),tr("错误:读Distance失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_LenMotorCtl_W:
            QToolTip::showText(this->rect().center(),tr("错误:写LenMotorCtl失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_2DBracketCtl_W:
            QToolTip::showText(this->rect().center(),tr("错误:写2DBracketCtl失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_OutVolume_R:
            QToolTip::showText(this->rect().center(),tr("错误:读OutVolume失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_OutVolume_W:
            QToolTip::showText(this->rect().center(),tr("错误:写OutVolume失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_VideoCtl_R:
            QToolTip::showText(this->rect().center(),tr("错误:读VideoCtl失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_VideoCtl_W:
            QToolTip::showText(this->rect().center(),tr("错误:写VideoCtl失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_FPGA_R:
            QToolTip::showText(this->rect().center(),tr("错误:读FPGA失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_FPGA_W:
            QToolTip::showText(this->rect().center(),tr("错误:写FPGA失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_CrossXY_R:
            QToolTip::showText(this->rect().center(),tr("错误:读CrossXY失败!"),this,rectTips,2000);
            break;
        case ZModBusProtocol::nReg_CrossXY_W:
            QToolTip::showText(this->rect().center(),tr("错误:写CrossXY失败!"),this,rectTips,2000);
            break;
        default:
            QToolTip::showText(this->rect().center(),tr("错误:未知寄存器操作!"),this,rectTips,2000);
            break;
        }
    }else{
        qDebug()<<"normal Ack";
    }
    return true;
}
void ZMainUI::ZSlotCurveUpdate()
{
    ZCurveDialog dia(this);
    dia.setGeometry(0,0,this->width(),this->height());
    dia.exec();
}
void ZMainUI::ZSlotVolumeCtl(qint32 which,bool inc)
{
    qint32 nLocalMax=QAndroidJniObject::callStaticMethod<jint>("app/of/domain/OutVolume", "maxStreamVolume");
    qint32 nCurrent=QAndroidJniObject::callStaticMethod<jint>("app/of/domain/OutVolume", "streamVolume");
    switch(which)
    {
    case 0://local volume.
        if(inc)
        {
            if(nCurrent<nLocalMax)
            {
                nCurrent++;

            }
        }else{
            if(nCurrent>0)
            {
                nCurrent--;
            }
        }
        QAndroidJniObject::callStaticMethod<void>("app/of/domain/OutVolume","setStreamVolume","(I)V",nCurrent);
        break;
    case 1://device volume.
        if(inc)
        {
            if(this->m_nDeviceVolume<100)
            {
                this->m_nDeviceVolume++;

                ZModBusProtocol protocol;
                QByteArray cmd;
                cmd=protocol.ZGetCmd_Wr_OutVolume(this->m_nDeviceVolume);
                this->ZPutEle2FIFO(cmd,ZModBusProtocol::nReg_OutVolume_W);
            }
        }else{
            if(this->m_nDeviceVolume>0)
            {
                this->m_nDeviceVolume--;

                ZModBusProtocol protocol;
                QByteArray cmd;
                cmd=protocol.ZGetCmd_Wr_OutVolume(this->m_nDeviceVolume);
                this->ZPutEle2FIFO(cmd,ZModBusProtocol::nReg_OutVolume_W);
            }
        }
        break;
    default:
        break;
    }
}
void ZMainUI::ZSlotVolumeSet()
{
    QAndroidJniObject::callStaticMethod<void>("app/of/domain/OutVolume", "show");
}
