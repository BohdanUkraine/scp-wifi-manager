#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFrame>
#include <QColor>
#include <QPalette>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusConnectionInterface>
#include "container.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class mainWindow;
}
QT_END_NAMESPACE

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private:
    //Settings
    int width = 400;
    int height = 300;
    int posX = 0;
    int posY = 0;
    int buttonWidth = 30;


    Ui::mainWindow *ui;
    QPushButton *retry;
    QPushButton *togglePower;
    QPushButton *toggleHotspot;
    QPushButton *settingsButton;

    QScrollArea *scrollBox;
    QWidget *contentWidget;
    QVBoxLayout *vBox;

    QFrame *line;
    QColor backgroundColor = QColor("#333333");
    QString cs1="QPushButton {background-color: #202020;border: 0px;}"
                "QPushButton:hover {background-color: #454545}"
                "QPushButton:pressed {background-color: #202020}";

    //work with wifi

    void getActiveConnections();
    void clearVBox();
    QList<QDBusObjectPath> pathListFromVariant(QVariant var);

    QString nmPath;
    QString myIP;
    QList<QString> known, active;
    QMap<QString, QString> savedPaths;
    QList<QDBusObjectPath> activeConnections;
    QList<PreviewContainer*> containers;
    QDBusInterface *scan;


public slots:
    void tryConnect(info wifi, PreviewContainer* sender);
    void disconnectFromWifi(info wifi, PreviewContainer* sender);
    void deleteConnection(info wifi, PreviewContainer* sender);
    void requestScan();
    void toggleSettings();
    //void refreshPos();
    void refreshList();
};
#endif // MAINWINDOW_H
