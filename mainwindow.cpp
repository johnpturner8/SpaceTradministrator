#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "agentinfo.h"
#include "sectormap.h"

#include <QVBoxLayout>
#include <QPushButton>

AgentInfo* ag;
SectorMap* sm;

MainWindow::MainWindow(QWidget *parent, QNetworkAccessManager *mgr):
    QWidget{parent},
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Main window opened..." << Qt::endl;

    QGridLayout* layout = new QGridLayout;

    sm = new SectorMap (this, mgr);
    ag = new AgentInfo (this, mgr);
//    QPushButton *button1 = new QPushButton("One");
//    QPushButton *button2 = new QPushButton("Two");
//    QPushButton *button3 = new QPushButton("Three");

    layout->addWidget(sm, 1, 0);
    layout->addWidget(ag, 0, 1);
//    layout->addWidget(button1, 0, 0);
//    layout->addWidget(button2, 1, 0);
//    layout->addWidget(button3, 1, 1);

    setLayout(layout);

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
}
