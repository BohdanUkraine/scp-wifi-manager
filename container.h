#ifndef CONTAINER_H
#define CONTAINER_H

#include <QWidget>
#include <QPalette>
#include <QColor>
#include <QLabel>
#include <QSizePolicy>
#include <QPushButton>
#include <QFontDatabase>
#include <QFont>
#include <QPicture>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QIcon>
#include <QLineEdit>
#include <QMovie>

enum state{
    connected,
    saved,
    disconnected
};

struct info{
    QString ssid;
    QString password;
    QString ip;
    QString path;
    int frequency;
    int strength;
    QList<state> st;
};

class Container : public QWidget
{
    Q_OBJECT
public:
    explicit Container(QWidget *parent = nullptr);
    ~Container();

private:


protected:
    QString cs1="QPushButton {background-color: #333333;border: 0px;}"
                "QPushButton:hover {background-color: #404040}";
    QString cs2="QPushButton {background-color: #202020;border: 0px;}"
                "QPushButton:hover {background-color: #454545}"
                "QPushButton:pressed {background-color: #202020}";
    const int HEIGHT = 50;
    const int WIDTH = 380;
    QColor defaultColor = QColor("#333333");
    QPushButton *callNextForm;
    info myInfo;

public slots:
    virtual void click();

};

class StatusContainer : public Container
{
    Q_OBJECT
public:
    StatusContainer(QWidget *parent = nullptr, QString text = "lorum ipsum");
    ~StatusContainer();

    void displayStatus(QString status);
    void reset();

private:
    QLabel *statusAnim;
    QLabel *status;
    QLabel *cover;
    QMovie *movie;
    bool stopping;

    QPropertyAnimation *slideOut = new QPropertyAnimation(this, "pos", parent());
    QPropertyAnimation *revealAnim;
    QPropertyAnimation *revealText;
    QPropertyAnimation *revealText2;

public slots:
    void count();
    void reveal();
    void close();
    void EmitEnd();

signals:
    void Ended();
};

class PreviewContainer : public Container
{
    Q_OBJECT
public:
    PreviewContainer(info wifiInfo, QWidget *parent = nullptr);
    ~PreviewContainer();

    void showStatus(QString error);
    void stateChanged(info wifiInfo);
    info getInfo();

private:
    QLabel *name;
    QLabel *strength;
    QLabel *savedImg;

    QString ssid;
    int strengthPersent;
    Container *nextForm;
    StatusContainer *status;

    QPropertyAnimation *slideIn;
    QPropertyAnimation *statusSlideIn;

    QString strengthFromInt(int strength);


public slots:
    void click();
    void remakeStatus();
    void tryConnect(QString pswd);
    void disconnect();
    void deleteConnection();

signals:
    void tryConnectSignal(info wifi, PreviewContainer* sender);
    void disconnectSignal(info wifi, PreviewContainer* sender);
    void deleteConnectionSignal(info wifi, PreviewContainer* sender);


};

class ConnectedContainer : public Container
{
    Q_OBJECT
public:
    ConnectedContainer(info wifiInfo, QWidget *parent = nullptr);
    ~ConnectedContainer();

private:
    QLabel *ip;
    QPushButton *disconnect;
    QPushButton *share;

    QPropertyAnimation *slideOut = new QPropertyAnimation(this, "pos", parent());

public slots:
    void click();
    void disconnectClick();

signals:
    void disconnectSignal();
};

class PasswordContainer : public Container
{
    Q_OBJECT
public:
    PasswordContainer(QWidget *parent = nullptr);
    ~PasswordContainer();

private:

    QLineEdit *input;
    QPushButton *toggleVisibility;
    QPushButton *tryConnect;

    QPropertyAnimation *slideOut = new QPropertyAnimation(this, "pos", parent());

public slots:
    void click();
    void toggle();
    void connectToWifi();

signals:
    void tryConnectSignal(QString password);
};

class SavedDisconnectedContainer : public Container
{
    Q_OBJECT
public:
    SavedDisconnectedContainer(QWidget *parent = nullptr);
    ~SavedDisconnectedContainer();

private:
    QLabel *disconnected;
    QPushButton *tryConnect;
    QPushButton *share;
    QPushButton *forget;

    QPropertyAnimation *slideOut = new QPropertyAnimation(this, "pos", parent());

public slots:
    void connectToWifi();
    void click();
    void deleteWifi();

signals:
    void tryConnectSignal(QString pswd);
    void tryDelete();
};


#endif // CONTAINER_H
