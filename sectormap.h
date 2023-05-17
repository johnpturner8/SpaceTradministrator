#ifndef SECTORMAP_H
#define SECTORMAP_H

#include <QWidget>
#include <QChartView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScatterSeries>

class SectorMap : public QChartView
{
    Q_OBJECT

public:
    explicit SectorMap(QWidget *parent = nullptr, QNetworkAccessManager *mgr = nullptr, QString token = NULL);
    ~SectorMap();

    void loadInfo(int page = 1);
    void login(QString token);

private:
    QNetworkAccessManager *manager;
    QString token;
    QScatterSeries * stars;
    QPointF m_lastMousePos;

protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;


private slots:
    void setInfo(QNetworkReply * reply);
};

#endif // SECTORMAP_H
