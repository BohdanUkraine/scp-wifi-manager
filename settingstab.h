#ifndef SETTINGSTAB_H
#define SETTINGSTAB_H

#pragma once

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
#include <QPushButton>
#include <QWindow>
#include <QMouseEvent>
#include <QSettings>
#include <QLineEdit>

#include "generic.h"


class SettingsTab : public QWidget
{
    Q_OBJECT

    QPoint pos, size;
    void loadFromFile();

public:
    explicit SettingsTab(QWidget *parent = nullptr);

    void loadAllParams();

    info hotspotInfo;

protected:
    QString cs1="QPushButton {background-color: #333333;border: 0px;}"
                  "QPushButton:hover {background-color: #404040}";
    QString cs2="QPushButton {background-color: #202020;border: 0px;}"
                  "QPushButton:hover {background-color: #454545}"
                  "QPushButton:pressed {background-color: #202020}";

    QColor defaultColor = QColor("#333333");

    bool eventFilter(QObject *watched, QEvent *event) override;

    /*
    const int HEIGHT = 50;
    const int WIDTH = 380;

    int width = 400;
    int height = 300;
    int posX = 0;
    int posY = 0;
    int buttonWidth = 30;

    QRect(0, 50, 400, 250)
    QRect(0, 50, width, height-height/6) */

private:
    QSettings *settings;
    QWidget *win;
    QFrame *line1;

    bool m_isDragging = false;
    QPoint m_dragPosition = QPoint(0,0);

    QLabel *posLabel, *ssidName, *pswdName;
    QPushButton *posButton, *reset, *toggleVisibility, *share;
    QLineEdit *inputSsid, *inputPswd;

    //QLabel *ip;
    //QPushButton *disconnect;




public slots:
    void resetClicked();
    void toggle();
    void displayQrCodeClick();
    void pswdChanged(QString pswd);
    void ssidChanged(QString ssid);
    //void stopMoveWindow();

signals:
    void displayQrCodeSignal(info hotspot);
};

#endif // SETTINGSTAB_H
