#include "login.h"
//#include "QtCore/qiodevice.h"
//#include "QtGui/qtextcursor.h"
#include "./ui_login.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <format>
#include <QDebug>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

QNetworkAccessManager *manager;

Login::Login(QWidget *parent, QNetworkAccessManager *mgr)
    : QWidget{parent}
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    manager = mgr;
}

Login::~Login(){
    delete ui;
}

void Login::on_loginButton_clicked()
{
    qDebug() << "Logging in..." << Qt::endl;

    token = ui->tokenEdit->text();
    qInfo() << "onclick Token is: " << token << Qt::endl;

    QNetworkRequest req = QNetworkRequest(QUrl("https://api.spacetraders.io/v2/my/agent/"));
    req.setRawHeader("Authorization", "Bearer " + token.toUtf8());
    manager->get(req);

    connect(manager, &QNetworkAccessManager::finished,
            this, &Login::loginFinished, Qt::SingleShotConnection);
}


void Login::on_registerButton_clicked()
{
    qDebug() << "Registering..." << Qt::endl;

    QString callsign = ui->callSignEdit->text();
    QString faction = ui->factionSelect->currentText();

    QNetworkRequest request = QNetworkRequest(QUrl("https://api.spacetraders.io/v2/register"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    QJsonObject body;
    body.insert("symbol", callsign);
    body.insert("faction", faction);

    qDebug() << QJsonDocument(body).toJson() << Qt::endl;

    manager->post(request, QJsonDocument(body).toJson());

    connect(manager, &QNetworkAccessManager::finished,
            this, &Login::registerFinished, Qt::SingleShotConnection);
}


void Login::registerFinished(QNetworkReply * reply)
{
    qDebug() << "Registered..." << Qt::endl;
    qDebug() << "Response code: " << reply->error() << Qt::endl;

    if(reply->error() == QNetworkReply::NoError){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject rootObj = document.object();

//        qInfo() << document.toJson() << Qt::endl;
//        qInfo() << rootObj.value("data").toObject().value("token").toString();

        token = rootObj.value("data").toObject().value("token").toString();
//        qInfo() << "emitting Token is: " << token << Qt::endl;

        saveToken();

        emit loginSuccess(token);
        this->close();
    }
    else{
        //TODO: Show error message to user
    }
}

void Login::loginFinished(QNetworkReply * reply)
{
//    qDebug() << "Logged in..." << Qt::endl;
//    qDebug() << "Response code: " << reply->error() << Qt::endl;

    if(reply->error() == QNetworkReply::NoError){
        qInfo() << "emitting Token is: " << token << Qt::endl;

        saveToken();

        emit loginSuccess(token);
        this->close();
    }
    else{
        //TODO: Show error message to user
    }
//    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
//    QJsonObject rootObj = document.object();
}

void Login::saveToken(){
    QFile tokenFile("token.txt");
    tokenFile.open(QFile::ReadWrite);
    tokenFile.resize(0);
    tokenFile.write(token.toUtf8());
    tokenFile.flush();
    tokenFile.close();
}
