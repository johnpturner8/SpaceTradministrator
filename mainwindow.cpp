#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "agentinfo.h"
#include "sectormap.h"
#include "shiplist.h"

#include <QVBoxLayout>
#include <QPushButton>

AgentInfo* ag;
SectorMap* sm;
ShipList* sl;

MainWindow::MainWindow(QWidget *parent, QNetworkAccessManager *mgr):
    QWidget{parent},
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Main window opened..." << Qt::endl;

//    QGridLayout* layout = new QGridLayout;
    QVBoxLayout* vlayout = new QVBoxLayout;
    QHBoxLayout* hlayout = new QHBoxLayout;

    sm = new SectorMap (this, mgr);
    ag = new AgentInfo (this, mgr);
    sl = new ShipList (this);
//    QPushButton *button1 = new QPushButton("One");
//    QPushButton *button2 = new QPushButton("Two");
//    QPushButton *button3 = new QPushButton("Three");

    vlayout->addWidget(ag);
//    layout->addWidget(button1, 0, 0);
//    layout->addWidget(button2, 1, 0);
    vlayout->addWidget(sl);
    hlayout->addLayout(vlayout);
    hlayout->addWidget(sm);


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

    this->token = token;
    this->show();

    ag->login(token);
    sm->login(token);
    sl->login(token);
}
