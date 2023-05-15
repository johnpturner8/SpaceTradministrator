#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr, QNetworkAccessManager *mgr = nullptr);
    ~Login();

private slots:
    void on_loginButton_clicked();

    void on_registerButton_clicked();

    void registerFinished(QNetworkReply * reply);

    void loginFinished(QNetworkReply * reply);


private:
    Ui::Login *ui;
    QString token;

signals:
    void loginSuccess(QString token);
};

#endif // LOGIN_H
