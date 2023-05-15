#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "agentinfo.h"

#include <QVBoxLayout>

AgentInfo* ag;

MainWindow::MainWindow(QWidget *parent, QNetworkAccessManager *mgr):
    QWidget{parent},
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Main window opened..." << Qt::endl;

    ag = new AgentInfo (this, mgr, nullptr);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(ag);
    setLayout(layout);
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
}
