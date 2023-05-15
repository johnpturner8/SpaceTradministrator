#include "login.h"
#include "mainwindow.h"
#include "agentinfo.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QNetworkAccessManager>
#include <QPointer>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QNetworkAccessManager mgr;

    Login l(nullptr, &mgr);
    l.show();

    MainWindow m(nullptr, &mgr);

//    AgentInfo ag(&m, &mgr, nullptr);
    //    ag.loadInfo();
//    ag.show();

    QObject::connect(&l, SIGNAL(loginSuccess(QString)), &m, SLOT(login(QString)));
//    m.show();

    return a.exec();
}
