#include "zcommunicatethread.h"
#include <QDebug>
#include <QTcpSocket>
#include "zgblparam.h"
ZCommunicateThread::ZCommunicateThread()
{
}
ZCommunicateThread::~ZCommunicateThread()
{
}
void ZCommunicateThread::ZBindFIFOTx(QMutex *mux,///<
                QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                QQueue<ZElement*> *fifo)
{
    this->m_muxFIFOIn=mux;
    this->m_waitFIFOInNotEmpty=waitFIFONotEmpty;
    this->m_waitFIFOInNotFull=waitFIFONotFull;
    this->m_FIFOIn=fifo;
}
void ZCommunicateThread::ZBindFIFOWaitAck(QMutex *mux,///<
                QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                QQueue<ZElement*> *fifo)
{
    this->m_muxFIFOOut=mux;
    this->m_waitFIFOOutNotEmpty=waitFIFONotEmpty;
    this->m_waitFIFOOutNotFull=waitFIFONotFull;
    this->m_FIFOOut=fifo;
}
void ZCommunicateThread::run()
{
    QTcpSocket *tcpSocket=new QTcpSocket;
    while(!g_GblPara.bExitFlag)
    {
        if(tcpSocket->state()==QAbstractSocket::UnconnectedState)
        {
            tcpSocket->connectToHost("127.0.0.1",6500);
        }
        //1.Get element from TxFIFO.
        this->m_muxFIFOIn->lock();
        while(this->m_FIFOIn->isEmpty())
        {
            //timeout 5s to check exit flag.
            this->m_waitFIFOInNotEmpty->wait(this->m_muxFIFOIn,5000);

            //if timeout and exit flag was set,then exit.
            if(g_GblPara.bExitFlag)
            {
                break;
            }
        }
        if(g_GblPara.bExitFlag)
        {
            this->m_muxFIFOIn->unlock();
            break;
        }

        ZElement *ele=this->m_FIFOIn->dequeue();
        this->m_waitFIFOInNotFull->wakeAll();
        this->m_muxFIFOIn->unlock();

        //2.Tx element out.
        qDebug()<<"get a element from txfifo.";
        tcpSocket->write(ele->baTxData->data(),ele->nTxLen);
        tcpSocket->waitForBytesWritten();

        //3.Move it into WaitAckFIFO.
        this->m_muxFIFOOut->lock();
        while(this->m_FIFOOut->size()==FIFO_DEPTH)
        {
            //timeout 5s to check exit flag.
            this->m_waitFIFOOutNotFull->wait(this->m_muxFIFOOut,5000);

            //if timeout and exit flag was set,then exit.
            if(g_GblPara.bExitFlag)
            {
                break;
            }
        }
        if(g_GblPara.bExitFlag)
        {
            this->m_muxFIFOOut->unlock();
            break;
        }
        this->m_FIFOOut->enqueue(ele);
        this->m_waitFIFOOutNotEmpty->wakeAll();
        this->m_muxFIFOOut->unlock();

        //to release cpu heavy load.
        this->usleep(1000*100);
    }
    tcpSocket->close();
    delete tcpSocket;
}
