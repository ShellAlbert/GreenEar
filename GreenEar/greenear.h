#ifndef GREEN_EAR_H
#define GREEN_EAR_H

#include "ui/zmainui.h"
#include "thread/zcommunicatethread.h"
#include "thread/ztimeoutscanthread.h"
#include "zgblparam.h"
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
class GreenEar:public QObject
{
    Q_OBJECT
public:
    GreenEar();
    ~GreenEar();

    bool ZStart();

private slots:
    void ZSlotCloseEvent();
private:
    ZMainUI *m_mainUI;
    ZCommunicateThread *m_commThread;
    ZTimeoutScanThread *m_timeoutThread;

    //Free FIFO.
    QMutex m_muxFreeFIFO;
    QWaitCondition m_waitFreeFIFONotEmpty;
    QWaitCondition m_waitFreeFIFONotFull;
    QQueue<ZElement*> m_freeFIFO;

    //Ack FIFO.
    QMutex m_muxAckFIFO;
    QWaitCondition m_waitAckFIFONotEmpty;
    QWaitCondition m_waitAckFIFONotFull;
    QQueue<ZElement*> m_ackFIFO;

    //MainUI thread to Communicate Thread queue.
    //TxFIFO.
    QMutex m_muxTxFIFO;
    QWaitCondition m_waitTxFIFONotEmpty;
    QWaitCondition m_waitTxFIFONotFull;
    QQueue<ZElement*> m_txFIFO;

    //Communicate Thread to Timeout Thread.
    //WaitAckFIFO.
    QMutex m_muxWaitAckFIFO;
    QWaitCondition m_waitWaitAckFIFONotEmpty;
    QWaitCondition m_waitWaitAckFIFONotFull;
    QQueue<ZElement*> m_waitAckFIFO;
};

#endif // GREEN_EAR_H
