#ifndef ZGBLPARAM_H
#define ZGBLPARAM_H

#include <QWidget>
#include <QByteArray>
class ZGblParam
{
public:
    ZGblParam();
public:
    bool bExitFlag;

    QString fontFamily;
};
extern ZGblParam g_GblPara;

//element in FIFO.
#define TX_RX_MAX_SIZE   256
#define FIFO_DEPTH  5
class ZElement
{
public:
    ZElement();
    ~ZElement();
public:
    QWidget *widget;
    qint32 nTimeoutTick;
    qint32 nRegAddr;
    QByteArray *baTxData;
    qint32 nTxLen;
    QByteArray *baRxData;
    qint32 nRxLen;
};

typedef enum{
    Battery_Empty=0,
    Battery_Quarter,
    Battery_Half,
    Battery_Three_Quarter,
    Battery_Full,
}Battery_State;
#endif // ZGBLPARAM_H
