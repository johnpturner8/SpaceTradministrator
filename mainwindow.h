#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, QNetworkAccessManager *mgr = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString token;

public slots:
    void login(QString token);
};

#endif // MAINWINDOW_H
