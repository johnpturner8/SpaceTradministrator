//#include "textfinder.h"
#include "login.h"

#include <QApplication>
#include <QNetworkAccessManager>
#include <QPointer>

//static QPointer<QNetworkAccessManager> globalManager;

//QNetworkAccessManager *nMgr() {
////    Q_ASSERT(!qApp || QThread::currentThread() == qApp->thread());
//    return globalManager;
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QNetworkAccessManager mgr;
//    globalManager = &mgr;

    Login w(nullptr, &mgr);
    w.show();
    return a.exec();
}
