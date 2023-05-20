#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "agentinfo.h"
#include "sectormap.h"
#include "shiplist.h"
#include "systemmap.h"

#include <QVBoxLayout>
#include <QPushButton>

AgentInfo* ag;
SectorMap* sm;
ShipList* sl;
SystemMap* sy;

MainWindow::MainWindow(QWidget *parent, QNetworkAccessManager *mgr):
    QWidget{parent},
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Main window opened..." << Qt::endl;

//    QGridLayout* layout = new QGridLayout;
    QVBoxLayout* vlayout1 = new QVBoxLayout;
    QVBoxLayout* vlayout2 = new QVBoxLayout;

    QHBoxLayout* hlayout = new QHBoxLayout;

    sm = new SectorMap (this, mgr);
    ag = new AgentInfo (this, mgr);
    sl = new ShipList (this);
    sy = new SystemMap (this);
//    QPushButton *button1 = new QPushButton("One");
//    QPushButton *button2 = new QPushButton("Two");
//    QPushButton *button3 = new QPushButton("Three");

    vlayout1->addWidget(ag);
//    layout->addWidget(button1, 0, 0);
//    layout->addWidget(button2, 1, 0);
    vlayout1->addWidget(sl);
    hlayout->addLayout(vlayout1);
    vlayout2->addWidget(sy);
    vlayout2->addWidget(sm);
    hlayout->addLayout(vlayout2);

    setLayout(hlayout);

    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login(QString token){
    qInfo() << "Logged in to mainwindow with " << token << Qt::endl;

    connect(ag, &AgentInfo::hqRetrieved, sy, [=](QString hq) {
        QStringList l = hq.split('-');
        QString system = l[0] + "-" + l[1];
        sy->setSystem(system);
//        sy->setSystem("X1-AA27");
    });

    this->token = token;
    this->show();

    ag->login(token);
    sm->login(token);
    sl->login(token);
    sy->login(token);
}
