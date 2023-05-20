#include "systemmap.h"
#include "QtCore/qjsonarray.h"
#include "QtCore/qjsondocument.h"
#include "QtCore/qjsonobject.h"
#include <QScatterSeries>

const int NUM_WP_TYPES = 8;

QScatterSeries star;
QScatterSeries planets;
QScatterSeries gas_giants;
QScatterSeries moons;
QScatterSeries stations;
QScatterSeries gates;
QScatterSeries asteroids;
QScatterSeries debris;
QScatterSeries gravity_wells;

QScatterSeries * waypointSeries[NUM_WP_TYPES] = {&planets, &gas_giants, &moons, &stations, &gates, &asteroids, &debris, &gravity_wells};

enum WaypointTypes {
    PLANET,
    GAS_GIANT,
    MOON,
    ORBITAL_STATION,
    JUMP_GATE,
    ASTEROID_FIELD,
    NEBULA,
    DEBRIS_FIELD,
    GRAVITY_WELL
};

std::map<QString, WaypointTypes> waypointTypeValues;


SystemMap::SystemMap(QWidget *parent) :
    QChartView(new QChart, parent)
{
    manager = new QNetworkAccessManager;

    chart()->setTheme(QChart::ChartThemeDark);
    chart()->legend()->hide();

    star.append(0, 0);

    //initialize map
    waypointTypeValues["PLANET"] = PLANET;
    waypointTypeValues["GAS_GIANT"] = GAS_GIANT;
    waypointTypeValues["MOON"] = MOON;
    waypointTypeValues["ORBITAL_STATION"] = ORBITAL_STATION;
    waypointTypeValues["JUMP_GATE"] = JUMP_GATE;
    waypointTypeValues["ASTEROID_FIELD"] = ASTEROID_FIELD;
    waypointTypeValues["NEBULA"] = NEBULA;
    waypointTypeValues["DEBRIS_FIELD"] = DEBRIS_FIELD;
    waypointTypeValues["GRAVITY_WELL"] = GRAVITY_WELL;

    chart()->addSeries(&star);
    for(int i = 0; i < NUM_WP_TYPES; i++){
        waypointSeries[i]->setMarkerSize(5);
        chart()->addSeries(waypointSeries[i]);
    }

    chart()->createDefaultAxes();
    chart()->axes(Qt::Horizontal).first()->setRange(-100.0, 100.0);
    chart()->axes(Qt::Vertical).first()->setRange(-100.0, 100.0);
}

SystemMap::~SystemMap()
{
}

void SystemMap::login(QString token){
    this->token = token;
    loadInfo();
}

void SystemMap::setSystem(QString systemSymbol){
    system = systemSymbol;
    loadInfo();
}

void SystemMap::loadInfo(){
    if(token.isEmpty() || system.isEmpty()) return;

    QNetworkRequest req = QNetworkRequest(QUrl("https://api.spacetraders.io/v2/systems/" + system));
    req.setRawHeader("Authorization", "Bearer " + token.toUtf8());
    manager->get(req);

    connect(manager, &QNetworkAccessManager::finished,
            this, &SystemMap::setInfo, Qt::UniqueConnection);
}

void SystemMap::setInfo(QNetworkReply * reply){
    if(reply->error() == QNetworkReply::NoError){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject rootObj = document.object();
        QJsonObject data = rootObj.value("data").toObject();
        QJsonArray waypoints = data.value("waypoints").toArray();

        for(int i = 0; i < NUM_WP_TYPES; i++){
            QJsonObject wp = waypoints[i].toObject();
            QString type = wp.value("type").toString();
            qInfo() << type << waypointTypeValues[type];
            waypointSeries[waypointTypeValues[type]]->append(wp.value("x").toInt(), wp.value("y").toInt());
        }
    }
    else{
        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "ERROR in systemmap setinfo, Response code: " << status_code << Qt::endl;
    }
}
