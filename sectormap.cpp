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

const int STAR_TYPES = 10;

QScatterSeries neutron_stars;
QScatterSeries red_stars;
QScatterSeries orange_stars;
QScatterSeries blue_stars;
QScatterSeries young_stars;
QScatterSeries white_dwarfs;
QScatterSeries black_holes;
QScatterSeries hypergiants;
QScatterSeries nebulas;
QScatterSeries unstables;

enum StarTypes {
    NEUTRON_STAR,
    RED_STAR,
    ORANGE_STAR,
    BLUE_STAR,
    YOUNG_STAR,
    WHITE_DWARF,
    BLACK_HOLE,
    HYPERGIANT,
    NEBULA,
    UNSTABLE
};

std::map<QString, StarTypes> starTypeValues;

QScatterSeries * stars[STAR_TYPES] = {&neutron_stars, &red_stars, &orange_stars, &blue_stars, &young_stars, &white_dwarfs, &black_holes, &hypergiants, &nebulas, &unstables};

SectorMap::SectorMap(QWidget *parent, QNetworkAccessManager *mgr, QString token) :
    QChartView(new QChart, parent),
    m_isTouching(false)
{
    this->token = token;
    manager = new QNetworkAccessManager;

    chart()->setTheme(QChart::ChartThemeDark);
//    setRubberBand(QChartView::RectangleRubberBand);
//    setDragMode(QGraphicsView::ScrollHandDrag);
    chart()->legend()->hide();
    chart()->setAnimationOptions(QChart::SeriesAnimations);

//    stars = new QScatterSeries();

    neutron_stars.setName("Neutron Star");
    red_stars.setName("Red Star");
    orange_stars.setName("Orange Star");
    blue_stars.setName("Blue Star");
    young_stars.setName("Young Star");
    white_dwarfs.setName("White Dwarf");
    black_holes.setName("Black Hole");
    hypergiants.setName("Hypergiant");
    nebulas.setName("Nebulas");
    unstables.setName("Unstables");

    for(int i = 0; i < STAR_TYPES; i++){
        stars[i]->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        stars[i]->setBorderColor(Qt::transparent);
        stars[i]->setMarkerSize(3);
    }

    red_stars.setColor(Qt::red);
    orange_stars.setColor(QColorConstants::Svg::orange);
    blue_stars.setColor(QColorConstants::Svg::blue);
    young_stars.setColor(QColorConstants::Cyan);
    white_dwarfs.setColor(Qt::white);
    black_holes.setColor(Qt::black);
    black_holes.setBorderColor(Qt::white);
    black_holes.setMarkerSize(4);
    hypergiants.setMarkerSize(6);
    hypergiants.setColor(Qt::yellow);
    nebulas.setMarkerShape(QScatterSeries::MarkerShapeRotatedRectangle);
    nebulas.setColor(QColorConstants::Svg::purple);
    unstables.setMarkerShape(QScatterSeries::MarkerShapeTriangle);
    unstables.setColor(Qt::green);

    //initialize map
    starTypeValues["NEUTRON_STAR"] = NEUTRON_STAR;
    starTypeValues["RED_STAR"] = RED_STAR;
    starTypeValues["ORANGE_STAR"] = ORANGE_STAR;
    starTypeValues["BLUE_STAR"] = BLUE_STAR;
    starTypeValues["YOUNG_STAR"] = YOUNG_STAR;
    starTypeValues["WHITE_DWARF"] = WHITE_DWARF;
    starTypeValues["BLACK_HOLE"] = BLACK_HOLE;
    starTypeValues["HYPERGIANT"] = HYPERGIANT;
    starTypeValues["NEBULA"] = NEBULA;
    starTypeValues["UNSTABLE"] = UNSTABLE;

//    stars->setName("stars");
//    stars->setMarkerShape(QScatterSeries::MarkerShapeCircle);
//    stars->setMarkerSize(2);
//    stars->setBorderColor(Qt::transparent);
//    stars->append(0, 0);

//    *stars << QPointF(1, 1);
//    stars->setColor(Qt::white);

//    qInfo() << stars->color();

    for(int i = 0; i < STAR_TYPES; i++)
        chart()->addSeries(stars[i]);


    chart()->createDefaultAxes();
    chart()->axes(Qt::Horizontal).first()->setRange(-1000.0, 1000.0);
    chart()->axes(Qt::Vertical).first()->setRange(-1000.0, 1000.0);

//    chart()->removeAxis(chart()->axes(Qt::Horizontal).first());
//    chart()->removeAxis(chart()->axes(Qt::Vertical).first());


//    chart()->setPlotArea(QRectF(-1000.0, 1000.0, 2000.0, 2000.0));
}

void SectorMap::wheelEvent(QWheelEvent *event)
{
    QGraphicsView::wheelEvent(event);
    qreal factor = event->angleDelta().y() < 0? 0.9: 1.1;
    chart()->zoom(factor);
    event->accept();
    QChartView::wheelEvent(event);
}

bool SectorMap::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        // By default touch events are converted to mouse events. So
        // after this event we will get a mouse event also but we want
        // to handle touch events as gestures only. So we need this safeguard
        // to block mouse events that are actually generated from touch.
        m_isTouching = true;

        // Turn off animations when handling gestures they
        // will only slow us down.
        chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void SectorMap::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QChartView::mousePressEvent(event);
}

void SectorMap::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QChartView::mouseMoveEvent(event);
}

void SectorMap::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;

    // Because we disabled animations when touch event was detected
    // we must put them back on.
    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);
}

void SectorMap::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Equal:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -10);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

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
            QString type = system.value("type").toString();
            qInfo() << type << starTypeValues[type];
            stars[starTypeValues[type]]->append(system.value("x").toInt(), system.value("y").toInt());

//            stars->append(system.value("x").toInt(), system.value("y").toInt());
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


//int SectorMap::heightForWidth(int w) const
//{
//    return w;
//}

//bool SectorMap::hasHeightForWidth() const
//{ return true; }

