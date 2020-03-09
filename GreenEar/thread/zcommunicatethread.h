#ifndef ZCOMMUNICATETHREAD_H
#define ZCOMMUNICATETHREAD_H

#include <QThread>
#include "zgblparam.h"
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
class ZCommunicateThread : public QThread
{
    Q_OBJECT
public:
    ZCommunicateThread();
    ~ZCommunicateThread();

    void ZBindFIFOTx(QMutex *mux,///<
                    QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                    QQueue<ZElement*> *fifo);

    void ZBindFIFOWaitAck(QMutex *mux,///<
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

#endif // ZCOMMUNICATETHREAD_H
