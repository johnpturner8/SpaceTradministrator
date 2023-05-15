#ifndef AGENTINFO_H
#define AGENTINFO_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class AgentInfo;
}

class AgentInfo : public QWidget
{
    Q_OBJECT

public:
    explicit AgentInfo(QWidget *parent = nullptr, QNetworkAccessManager *mgr = nullptr, QString token = NULL);
    ~AgentInfo();

    void loadInfo();
    void login(QString token);

private:
    Ui::AgentInfo *ui;
    QNetworkAccessManager *manager;
    QString token;

private slots:
    void setInfo(QNetworkReply * reply);
};

#endif // AGENTINFO_H
