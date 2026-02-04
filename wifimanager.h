#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#pragma once

#include <QObject>
#include <iostream>
#include <QUuid>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMetaType>

#include "container.h"
#include "generic.h"

class WiFiManager : public QObject
{
    Q_OBJECT
public:
    explicit WiFiManager(QObject *parent = nullptr);


    //work with wifi

    void getActiveConnections();    //get connected wifi data
                                    //wifi with 5ghz and 2.4ghz have 2 different paths to connect to
    void getSavedConnections();     //get saved wifi connections and refresh list

    QList<QDBusObjectPath> pathListFromVariant(QVariant var);


    QList<info> avaliableConnections;           //all about avaliable connections
    QList<QString> known, active;               //ssid of saved and connected wifi
    QMap<QString, QString> savedPaths;          //saved connections savedPaths[<ssid>] = <path>
    QList<QDBusObjectPath> activeConnections;   //which I am connected to


    QString nmPath;     //NetworkManager path
    QString myIP;       //current connected network ip


    QDBusInterface *scanInterface;

    QDBusPendingCallWatcher* watcher;   //async calls heler

    info processedWifi;
    PreviewContainer* processedSender;

    bool getHotspotStatus();
    void startHotspot(info wifi);
    void stopHotspot();

    bool getWifiStatus();
    void turnOn();
    void turnOff();


public slots:

    void startScan();   //start async wifi scan
    void endScan();     //refresh data after async wifi scan ends


    void tryConnect(info wifi, PreviewContainer* sender);
    void processActivateReply(QDBusPendingCallWatcher* call);
    void onConnectionPropertiesChanged(const QString &interface,
                                       const QVariantMap &changedProperties,
                                       const QStringList &invalidatedProperties);

    void disconnectFromWifi(info wifi, PreviewContainer* sender);

    void deleteConnection(info wifi, PreviewContainer* sender);

signals:
    void scanEnded();
    void connectionChanged(QString oldSsid, QString newSsid);
    void disconnectedFromWifi(QString Ssid);
    void connectionDeleted(QString Ssid);

};

#endif // WIFIMANAGER_H
