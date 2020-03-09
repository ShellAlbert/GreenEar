#ifndef ZTIMEOUTSCANTHREAD_H
#define ZTIMEOUTSCANTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include "zgblparam.h"
class ZTimeoutScanThread : public QThread
{
    Q_OBJECT
public:
    ZTimeoutScanThread();
    void ZBindFIFOWaitAck(QMutex *mux,///<
                    QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                    QQueue<ZElement*> *fifo);

    void ZBindFIFOAck(QMutex *mux,///<
                    QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                    QQueue<ZElement*> *fifo);
protected:
    void run();
private:
    //FIFO In.
    QMutex *m_muxFIFOIn;
    QWaitCondition *m_waitFIFOInNotEmpty;
    QWaitCondition *m_waitFIFOInNotFull;
    QQueue<ZElement*> *m_FIFOIn;

    //FIFO Out.
    QMutex *m_muxFIFOOut;
    QWaitCondition *m_waitFIFOOutNotEmpty;
    QWaitCondition *m_waitFIFOOutNotFull;
    QQueue<ZElement*> *m_FIFOOut;
};

#endif // ZTIMEOUTSCANTHREAD_H
