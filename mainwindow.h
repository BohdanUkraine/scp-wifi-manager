#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <iostream>
#include <QPushButton>
#include <QFrame>
#include <QColor>
#include <QPalette>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QTimer>
#include <QPropertyAnimation>
#include <QScrollBar>

#include "container.h"
#include "wifimanager.h"
#include "settingstab.h"
#include "generic.h"

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
    QPushButton *toggleHotspotButton;
    QPushButton *settingsButton;
    QPropertyAnimation *settingsSlideIn;
    QPropertyAnimation *settingsSlideOut;
    QPropertyAnimation *qrSlideIn;
    SettingsTab *settingsTab;
    bool settingsVisible = false;

    QScrollArea *scrollBox;
    QScrollBar *scrollBar;
    QWidget *contentWidget;
    QVBoxLayout *vBox;

    QFrame *line;
    QColor backgroundColor = QColor("#333333");
    QString cs1="QPushButton {background-color: #202020;border: 3px;}"
                "QPushButton:hover {background-color: #454545}"
                "QPushButton:pressed {background-color: #202020}";

    QString hotspotWorksStyle="QPushButton {background-color: #204020;border: 3px;}"
                  "QPushButton:hover {background-color: #456545}"
                  "QPushButton:pressed {background-color: #204020}";


    QString scrollStyle="QScrollBar:vertical {"
                          "    border: none;"
                          "    background: #333333;"
                          "    width: 6px;"
                          "    margin: 0px;"
                          "}"
                          "QScrollBar::handle:vertical {"
                          "    background: #666666;"
                          "    min-height: 20px;"
                          "    border-radius: 3px;"
                          "}"
                          "QScrollBar::handle:vertical:hover {"
                          "    background: #888888;"
                          "}"
                          "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                          "    height: 0px;"
                          "}";

    //work with wifi

    void clearVBox();

    WiFiManager* wifimanager;
    QList<PreviewContainer*> containers;
    QrContainer *qr;


public slots:

    void toggleSettings();

    void togglewifi();

    void toggleHotspot();

    void displayScanResults();

    void displayQrCode(info wifiInfo);

    void refreshContainers(QString ssid, QList<state> states, QString ip = "");

    void refreshUiOnConnectionChanged(QString oldSsid, QString newSsid);
    void refreshUiOnDisconnected(QString ssid);
    void refreshUiOnConnectionDeleted(QString ssid);

};
#endif // MAINWINDOW_H
