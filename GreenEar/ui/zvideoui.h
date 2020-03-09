#ifndef ZVIDEOUI_H
#define ZVIDEOUI_H

#include <QWidget>
#include <QPainter>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QTimer>
#include "zgblparam.h"
class ZVideoUI : public QWidget
{
    Q_OBJECT
public:
    explicit ZVideoUI(QWidget *parent = nullptr);
    ~ZVideoUI();

    void ZSetBatteryState(Battery_State state);
protected:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
signals:
    void ZSigArrowClicked(qint32 direction);
    void ZSigRdBattery();
    void ZSigVolumeCtl(qint32 which,bool inc);
    void ZSigVolumeSet();
private slots:
    void ZSlotButtonClicked();

    void ZSlotTimeout();
private:

    QToolButton *m_tbBattery;
    QToolButton *m_tbVerify;
    QToolButton *m_tbVolume;
    QHBoxLayout *m_hLayTop;

    QGridLayout *m_gridLayArrow;
    QToolButton *m_tbArrow[4];
    QHBoxLayout *m_hLayBtm;

    QVBoxLayout *m_vLayMain;
private:
    QTimer *m_timer;

private:
    bool m_bMousePressed;
    QPoint m_PosOld;
    QPoint m_PosNew;
};

#endif // ZVIDEOUI_H
