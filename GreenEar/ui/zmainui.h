#ifndef ZMAINUI_H
#define ZMAINUI_H

#include <QWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include "zgblparam.h"
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QCloseEvent>
#include <QTimer>
#include <QProgressBar>
#include "ui/zvideoui.h"
class ZMainUI : public QWidget
{
    Q_OBJECT

public:
    ZMainUI(QWidget *parent = 0);
    ~ZMainUI();

    void ZBindFIFOFree(QMutex *mux,///<
                     QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                     QQueue<ZElement*> *fifo);

    void ZBindFIFOTx(QMutex *mux,///<
                      QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                      QQueue<ZElement*> *fifo);

    void ZBindFIFOAck(QMutex *mux,///<
                    QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                    QQueue<ZElement*> *fifo);
private:
    bool ZPutEle2FIFO(QByteArray baData,qint32 nRegAddr);
    bool ZParseAckFrame(ZElement *ele);
signals:
    void ZSigCloseEvent();
protected:
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);
private slots:
    void ZSlotBattery();
    void ZSlotRSSI();
    void ZSlotAutoFocus();
    void ZSlotDistance();
    void ZSlotPlus();
    void ZSlotMinus();
    void ZSlotRecord();
    void ZSlotPlayback();
    void ZSlotArrow(qint32 direction);
    void ZSlotCurveUpdate();
    void ZSlotVolumeCtl(qint32 which,bool inc);
    void ZSlotVolumeSet();
private:
    QProgressBar *m_barRSSI;
    QToolButton *m_tbAutoFocus;
    QToolButton *m_tbDistance;
    QToolButton *m_tbPlus;
    QToolButton *m_tbMinus;
    QToolButton *m_tbRecord;
    QToolButton *m_tbPlayback;
    QGridLayout *m_gridLayLeft;

    ZVideoUI *m_videoUI;
    QHBoxLayout *m_hLayMain;

private:
    //FIFO Free.
    QMutex *m_muxFIFOFree;
    QWaitCondition *m_waitFIFOFreeNotEmpty;
    QWaitCondition *m_waitFIFOFreeNotFull;
    QQueue<ZElement*> *m_FIFOFree;

    //FIFO Tx.
    QMutex *m_muxFIFOTx;
    QWaitCondition *m_waitFIFOTxNotEmpty;
    QWaitCondition *m_waitFIFOTxNotFull;
    QQueue<ZElement*> *m_FIFOTx;


    //FIFO Ack.
    QMutex *m_muxFIFOAck;
    QWaitCondition *m_waitFIFOAckNotEmpty;
    QWaitCondition *m_waitFIFOAckNotFull;
    QQueue<ZElement*> *m_FIFOAck;

private:
    qint32 m_nDeviceVolume;//0%~100%.
};

#endif // ZMAINUI_H
