#ifndef SYSTEMMAP_H
#define SYSTEMMAP_H

#include <QChartView>
#include <QObject>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class SystemMap : public QChartView
{
public:
    SystemMap(QWidget *parent = nullptr);
    ~SystemMap();

    void loadInfo();
    void login(QString token);
    void setSystem(QString systemSymbol);

private:
    QNetworkAccessManager *manager;
    QString token;
    QString system;

private slots:
    void setInfo(QNetworkReply * reply);
};

#endif // SYSTEMMAP_H
