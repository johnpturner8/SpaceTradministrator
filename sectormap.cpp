#include "sectormap.h"
#include "QtWidgets/qapplication.h"

#include <QScatterSeries>
#include <QChartView>
#include <QChart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QJsonValue>

int page = 1;
int total = 700;
int limit = 20;
QTimer timer;

SectorMap::SectorMap(QWidget *parent, QNetworkAccessManager *mgr, QString token) :
    QChartView(new QChart, parent)
{
    this->token = token;
    manager = new QNetworkAccessManager;

    chart()->setTheme(QChart::ChartThemeDark);
    setRubberBand(QChartView::RectangleRubberBand);

    stars = new QScatterSeries();
    stars->setName("stars");
    stars->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    stars->setMarkerSize(1);
    stars->setBorderColor(Qt::transparent);
//    stars->append(0, 0);

//    *stars << QPointF(1, 1);
    stars->setColor(Qt::white);

    qInfo() << stars->color();

    chart()->addSeries(stars);


    chart()->createDefaultAxes();
    chart()->axes(Qt::Horizontal).first()->setRange(-1000.0, 1000.0);
    chart()->axes(Qt::Vertical).first()->setRange(-1000.0, 1000.0);

    chart()->removeAxis(chart()->axes(Qt::Horizontal).first());
    chart()->removeAxis(chart()->axes(Qt::Vertical).first());


//    chart()->setPlotArea(QRectF(-1000.0, 1000.0, 2000.0, 2000.0));

//    chart()->setPreferredWidth(200);
//    chart()->setPreferredHeight(200);
}

void SectorMap::wheelEvent(QWheelEvent *event)
{
    qreal factor = event->angleDelta().y() < 0? 0.9: 1.1;
    chart()->zoom(factor);
    event->accept();
    QChartView::wheelEvent(event);
}

//void SectorMap::mousePressEvent(QMouseEvent *event)
//{
//    if (event->button() == Qt::LeftButton)
//    {
//        QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
//        m_lastMousePos = event->pos();
//        event->accept();
//    }

//    QChartView::mousePressEvent(event);
//}

//void SectorMap::mouseMoveEvent(QMouseEvent *event)
//{
//    // pan the chart with a middle mouse drag
//    if (event->buttons() & Qt::LeftButton)
//    {
//        QRectF bounds = QRectF(0,0,0,0);
//        for(QScatterSeries series : this->chart()->series())
//            bounds.united(series->bounds());

//                auto dPos = this->chart()->mapToValue(event->pos()) - this->chart()->mapToValue(m_lastMousePos);

//        if (this->rubberBand() == QChartView::RectangleRubberBand)
//            this->chart()->zoom(bounds.translated(-dPos.x(), -dPos.y()));
//        else if (this->rubberBand() == QChartView::HorizontalRubberBand)
//            this->chart()->zoom(bounds.translated(-dPos.x(), 0));
//        else if (this->rubberBand() == QChartView::VerticalRubberBand)
//            this->chart()->zoom(bounds.translated(0, -dPos.y()));

//        m_lastMousePos = event->pos();
//        event->accept();
//    }

//    QChartView::mouseMoveEvent(event);
//}

SectorMap::~SectorMap()
{
}

void SectorMap::login(QString token){
    qDebug() << "Logging into sectormap with token: " << token << Qt::endl;
    this->token = token;
    loadInfo();
}

void SectorMap::loadInfo(int s){
    //check token has been set
    if(token.isEmpty()) return;

    connect(&timer, &QTimer::timeout, this, [=]() {
        qInfo() << "timer triggered";
        QNetworkRequest req = QNetworkRequest(QUrl("https://api.spacetraders.io/v2/systems?page=" + QByteArray::number(page) + "&limit=" + QByteArray::number(limit)));
        qInfo() << req.url();
        req.setRawHeader("Authorization", "Bearer " + token.toUtf8());
        manager->get(req);

        connect(manager, &QNetworkAccessManager::finished,
                this, &SectorMap::setInfo, Qt::UniqueConnection);

        page++;

        if((page+1) * limit > total){
            qDebug() << "stop at page " << page;
            timer.stop();
        }
//
    });

    timer.start(1000);
}


void SectorMap::setInfo(QNetworkReply * reply){
    if(reply->error() == QNetworkReply::NoError){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject rootObj = document.object();
        QJsonArray data = rootObj.value("data").toArray();


        for(int i = 0; i < data.size(); i++){
            QJsonObject system = data[i].toObject();
            stars->append(system.value("x").toInt(), system.value("y").toInt());
        }
    }
    else{
        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "ERROR in sectormap setinfo, Response code: " << status_code << Qt::endl;
    }
//    qInfo() << "sectormap set info";
//    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//    qDebug() << "Response code: " << status_code << Qt::endl;
//    qInfo() << "url: " << reply->url();
//    qDebug() << reply->resadAll() << Qt::endl;

    //check if last page or need to wait for rate limit
}
