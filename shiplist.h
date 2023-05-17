#ifndef SHIPLIST_H
#define SHIPLIST_H

#include "QtNetwork/qnetworkaccessmanager.h"
#include <QWidget>

namespace Ui {
class ShipList;
}

class ShipList : public QWidget
{
    Q_OBJECT

public:
    explicit ShipList(QWidget *parent = nullptr);
    ~ShipList();

    void loadInfo(int page = 1);
    void login(QString token);

private:
    Ui::ShipList *ui;
    QNetworkAccessManager *manager;
    QString token;

private slots:
    void setInfo(QNetworkReply * reply);
};


#endif // SHIPLIST_H
