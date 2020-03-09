#include "ztimeoutscanthread.h"
#include "zgblparam.h"
#include <QDebug>
#include <QApplication>
#include <QKeyEvent>
ZTimeoutScanThread::ZTimeoutScanThread()
{

}
void ZTimeoutScanThread::ZBindFIFOWaitAck(QMutex *mux,///<
                                     QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                                     QQueue<ZElement*> *fifo)
{
    this->m_muxFIFOIn=mux;
    this->m_waitFIFOInNotEmpty=waitFIFONotEmpty;
    this->m_waitFIFOInNotFull=waitFIFONotFull;
    this->m_FIFOIn=fifo;
}
void ZTimeoutScanThread::ZBindFIFOAck(QMutex *mux,///<
                                      QWaitCondition *waitFIFONotEmpty,QWaitCondition *waitFIFONotFull,///<
                                      QQueue<ZElement*> *fifo)
{
    this->m_muxFIFOOut=mux;
    this->m_waitFIFOOutNotEmpty=waitFIFONotEmpty;
    this->m_waitFIFOOutNotFull=waitFIFONotFull;
    this->m_FIFOOut=fifo;
}
void ZTimeoutScanThread::run()
{
    while(!g_GblPara.bExitFlag)
    {
        QList<ZElement*> list;
        list.clear();
        //1.Get element from WaitAckFIFO.
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

        for(qint32 i=0;i<this->m_FIFOIn->size();i++)
        {
            ZElement *ele=this->m_FIFOIn->at(i);
            if(ele->nTimeoutTick>0)
            {
                ele->nTimeoutTick--;
                qDebug()<<"timeout -1";
            }else{
                this->m_FIFOIn->takeAt(i);
                list.append(ele);
                qDebug()<<"list size:"<<list.size();
            }
        }
        this->m_waitFIFOInNotFull->wakeAll();
        this->m_muxFIFOIn->unlock();

        //3.Move it into AckFIFO.
        for(qint32 i=0;i<list.size();i++)
        {
            ZElement *ele=list.at(i);

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

            if(i==list.size()-1)
            {
                QKeyEvent *event=new QKeyEvent(QEvent::KeyPress,Qt::Key_A,Qt::NoModifier);
                QApplication::postEvent(ele->widget,event);
            }
        }

        //to release cpu heavy load.
        this->usleep(1000*200);//200ms.
    }
}
