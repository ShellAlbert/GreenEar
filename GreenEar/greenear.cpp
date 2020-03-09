#include "greenear.h"
#include "zgblparam.h"
#include <QApplication>
GreenEar::GreenEar()
{

}
GreenEar::~GreenEar()
{
    delete this->m_mainUI;
    delete this->m_commThread;
    delete this->m_timeoutThread;
}
bool GreenEar::ZStart()
{
    //free FIFO.
    for(qint32 i=0;i<FIFO_DEPTH;i++)
    {
        ZElement *ele=new ZElement;
        this->m_freeFIFO.enqueue(ele);
    }
    //AckFIFO.
    this->m_ackFIFO.clear();

    //TxFIFO.
    this->m_txFIFO.clear();

    //WaitAckFIFO.
    this->m_waitAckFIFO.clear();


    this->m_mainUI=new ZMainUI;
    this->m_mainUI->ZBindFIFOFree(&this->m_muxFreeFIFO,&this->m_waitFreeFIFONotEmpty,&this->m_waitFreeFIFONotFull,&this->m_freeFIFO);
    this->m_mainUI->ZBindFIFOTx(&this->m_muxTxFIFO,&this->m_waitTxFIFONotEmpty,&this->m_waitTxFIFONotFull,&this->m_txFIFO);
    this->m_mainUI->ZBindFIFOAck(&this->m_muxAckFIFO,&this->m_waitAckFIFONotEmpty,&this->m_waitAckFIFONotFull,&this->m_ackFIFO);
    QObject::connect(this->m_mainUI,SIGNAL(ZSigCloseEvent()),this,SLOT(ZSlotCloseEvent()));

    this->m_commThread=new ZCommunicateThread;
    this->m_commThread->ZBindFIFOTx(&this->m_muxTxFIFO,&this->m_waitTxFIFONotEmpty,&this->m_waitTxFIFONotFull,&this->m_txFIFO);
    this->m_commThread->ZBindFIFOWaitAck(&this->m_muxWaitAckFIFO,&this->m_waitWaitAckFIFONotEmpty,&this->m_waitWaitAckFIFONotFull,&this->m_waitAckFIFO);

    this->m_timeoutThread=new ZTimeoutScanThread;
    this->m_timeoutThread->ZBindFIFOWaitAck(&this->m_muxWaitAckFIFO,&this->m_waitWaitAckFIFONotEmpty,&this->m_waitWaitAckFIFONotFull,&this->m_waitAckFIFO);
    this->m_timeoutThread->ZBindFIFOAck(&this->m_muxAckFIFO,&this->m_waitAckFIFONotEmpty,&this->m_waitAckFIFONotFull,&this->m_ackFIFO);

    this->m_commThread->start();
    this->m_timeoutThread->start();
    //this->m_mainUI->show();
    this->m_mainUI->showFullScreen();

    return true;
}
void GreenEar::ZSlotCloseEvent()
{
    this->m_commThread->wait();
    this->m_timeoutThread->wait();
    this->m_mainUI->close();
    qApp->exit(0);
}
