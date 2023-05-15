#include "login.h"
#include "mainwindow.h"
#include "agentinfo.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QNetworkAccessManager>
#include <QPointer>
#include <QObject>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    qDebug() << "Current path: " << QDir::currentPath() << Qt::endl;

    QApplication a(argc, argv);

    QNetworkAccessManager mgr;

    Login l(nullptr, &mgr);

    MainWindow m(nullptr, &mgr);

    QObject::connect(&l, SIGNAL(loginSuccess(QString)), &m, SLOT(login(QString)));

    //check for token
    QFile tokenFile("token.txt");
    if(!tokenFile.exists()){
        l.show();
    }
    else{
        if(tokenFile.open(QFile::ReadWrite)){
            tokenFile.seek(0);
            m.login(tokenFile.readAll());
            tokenFile.close();
        }
    }

    return a.exec();
}
