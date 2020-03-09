#ifndef ZCURVEDIALOG_H
#define ZCURVEDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QToolButton>
#include <QVBoxLayout>
#include <QtCharts>
#include <QScatterSeries>
#include <QLineSeries>

class ZCurveDialog : public QDialog
{
    Q_OBJECT
public:
    ZCurveDialog(QWidget *parent=0);
    ~ZCurveDialog();

private slots:
    void ZSlotScatterSeriesClicked(const QPointF &pt);
    void ZSlotData();
    void ZSlotCalculate();
    void ZSlotCallOne();
    void ZSlotCallAll();
protected:

private:
    QChart *m_chart;
    QChartView *m_chartView;

    QLabel *m_llPointNum;
    QToolButton *m_btnData;
    QToolButton *m_btnCalc;
    QToolButton *m_btnCallOne;
    QToolButton *m_btnCallAll;
    QHBoxLayout *m_hLayBtns;

    QVBoxLayout *m_vLayoutMain;
private:
    //单次测距样本数据点，散点图
    QList<QPointF> m_listData;
    QScatterSeries *m_scatterSeries;

    //最小二乘法拟合后的数据点
    QList<QPointF> m_listData1;
    QLineSeries *m_lineSeries1;

    //分段拟合后的数据点,分段算法
    QList<QPointF> m_listData2;
    QLineSeries *m_lineSeries2;

    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
};

#endif // ZCURVEDIALOG_H
