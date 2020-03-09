#ifndef ZMODBUSPROTOCOL_H
#define ZMODBUSPROTOCOL_H

#include <QByteArray>

class ZModBusProtocol
{
public:
    static const qint32 nReg_Battery_R=0x000001;
    static const qint32 nReg_RSSI_R=0x000002;
    static const qint32 nReg_Distance_R=0x000003;
    static const qint32 nReg_LenMotorCtl_W=0x800004;
    static const qint32 nReg_2DBracketCtl_W=0x800005;

    static const qint32 nReg_OutVolume_R=0x000006;
    static const qint32 nReg_OutVolume_W=0x800006;

    static const qint32 nReg_VideoCtl_R=0x000007;
    static const qint32 nReg_VideoCtl_W=0x800007;

    static const qint32 nReg_FPGA_R=0x000008;
    static const qint32 nReg_FPGA_W=0x800008;

    static const qint32 nReg_CrossXY_R=0x000009;
    static const qint32 nReg_CrossXY_W=0x800009;

    static const qint32 nReg_AutoFocus1_R=0x3F0000;
    static const qint32 nReg_AutoFocus1_W=0xBF0000;
public:
    ZModBusProtocol();

public:
    QByteArray ZGetCmd_Rd_Battery();
    QByteArray ZGetCmd_Rd_RSSI();
    QByteArray ZGetCmd_Wr_AutoFocus();
    QByteArray ZGetCmd_Rd_Distance();
    QByteArray ZGetCmd_Wr_LenMotorCtl(bool clockwise);
    QByteArray ZGetCmd_Wr_2DBracketCtl(qint32 direction);
    QByteArray ZGetCmd_Wr_OutVolume(qint32 percent);
};

#endif // ZMODBUSPROTOCOL_H
