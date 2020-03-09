#include "zgblparam.h"

ZGblParam::ZGblParam()
{
    this->bExitFlag=false;
}
ZGblParam g_GblPara;

ZElement::ZElement()
{
    this->widget=NULL;
    this->nTimeoutTick=0;
    this->baTxData=new QByteArray;
    this->baTxData->resize(TX_RX_MAX_SIZE);
    this->baRxData=new QByteArray;
    this->baRxData->resize(TX_RX_MAX_SIZE);
}
ZElement::~ZElement()
{
    this->baTxData->clear();
    delete this->baTxData;
    this->baRxData->clear();
    delete this->baRxData;
}
