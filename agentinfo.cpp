#include "agentinfo.h"
#include "ui_agentinfo.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

AgentInfo::AgentInfo(QWidget *parent, QNetworkAccessManager *mgr, QString token) :
    QWidget(parent),
    ui(new Ui::AgentInfo)
{
    ui->setupUi(this);

    this->token = token;

    manager = mgr;

//    loadInfo();
}

AgentInfo::~AgentInfo()
{
    delete ui;
}

void AgentInfo::login(QString token){
    qDebug() << "Logging in with token: " << token << Qt::endl;
    this->token = token;
    loadInfo();
}

void AgentInfo::loadInfo(){
    //check token has been set
    if(token.isEmpty()) return;

    qDebug() << "Loading info with token: " << token << Qt::endl;

    QNetworkRequest req = QNetworkRequest(QUrl("https://api.spacetraders.io/v2/my/agent/"));
    req.setRawHeader("Authorization", "Bearer " + token.toUtf8());
    manager->get(req);

    connect(manager, &QNetworkAccessManager::finished,
            this, &AgentInfo::setInfo, Qt::SingleShotConnection);
}

void AgentInfo::setInfo(QNetworkReply * reply){
    qDebug() << "setting info..." << Qt::endl;
//    qDebug() << reply->readAll() << Qt::endl;

    if(reply->error() == QNetworkReply::NoError){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject rootObj = document.object();
        QJsonObject data = rootObj.value("data").toObject();

        qDebug() << document.toJson();

        QString callsign = data["symbol"].toString();
        QString headquarters = data["headquarters"].toString();
        int credits = data["credits"].toInteger();

        qDebug() << callsign << Qt::endl << headquarters << Qt::endl << credits << Qt::endl;

        ui->call_sign->setText(callsign);
        ui->hq->setText(headquarters);
        ui->credits->setText(QString::number(credits));
    }
}
