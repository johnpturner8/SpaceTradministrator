#include "shiplist.h"
#include "QtCore/qjsonarray.h"
#include "QtCore/qjsondocument.h"
#include "QtCore/qjsonobject.h"
#include "QtNetwork/qnetworkreply.h"
#include "ui_shiplist.h"

const int LIMIT = 20;

ShipList::ShipList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShipList)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager;
}

ShipList::~ShipList()
{
    delete ui;
}

void ShipList::login(QString token){
    this->token = token;
    loadInfo();
}

void ShipList::loadInfo(int page){
    assert(!token.isEmpty());

    QNetworkRequest req = QNetworkRequest(QUrl("https://api.spacetraders.io/v2/my/ships?page=" + QByteArray::number(page) + "&limit=" + QByteArray::number(LIMIT)));
    qInfo() << req.url();
    req.setRawHeader("Authorization", "Bearer " + token.toUtf8());
    manager->get(req);

    connect(manager, &QNetworkAccessManager::finished,
            this, &ShipList::setInfo, Qt::UniqueConnection);
}

void ShipList::setInfo(QNetworkReply * reply){
    if(reply->error() == QNetworkReply::NoError){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject rootObj = document.object();
        QJsonArray data = rootObj.value("data").toArray();

        for(int i = 0; i < data.size(); i++){
            QJsonObject ship = data[i].toObject();
//            qInfo() << QJsonDocument(ship).toJson();
            ui->tableWidget->insertRow( ui->tableWidget->rowCount() );
            ui->tableWidget->setVerticalHeaderItem( ui->tableWidget->rowCount()-1, new QTableWidgetItem(ship.value("symbol").toString()));
            QJsonObject nav = ship.value("nav").toObject();
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(nav.value("waypointSymbol").toString()));
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(nav.value("status").toString()));
            QJsonObject crew = ship.value("crew").toObject();
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(QByteArray::number(crew.value("current").toInt()) + "/" + QByteArray::number(crew.value("capacity").toInt())));
            QJsonObject fuel = ship.value("fuel").toObject();
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 3, new QTableWidgetItem(QByteArray::number(fuel.value("current").toInt()) + "/" + QByteArray::number(fuel.value("capacity").toInt())));
        }

        ui->tableWidget->resizeRowsToContents();
        ui->tableWidget->resizeColumnsToContents();
//        ui->tableWidget->adjustSize();
    }
    else{
        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "ERROR in shiplist setinfo, Response code: " << status_code << Qt::endl;
    }
}
