#include "wifimanager.h"

typedef QMap<QString, QMap<QString, QVariant> > ConnectionDetails;
Q_DECLARE_METATYPE(ConnectionDetails)

WiFiManager::WiFiManager(QObject *parent)
    : QObject{parent}
{
    //setup NetworkManager DBus interface
    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.NetworkManager",
        QDBusConnection::systemBus());

    QDBusReply<QList<QDBusObjectPath>> reply = nmInterface.call("GetDevices");

    for (const QDBusObjectPath &devicePath : reply.value()) {
        QDBusInterface deviceInterface(
            "org.freedesktop.NetworkManager",
            devicePath.path(),
            "org.freedesktop.NetworkManager.Device",
            QDBusConnection::systemBus());

        if (deviceInterface.property("DeviceType").toString() == "2")
            nmPath = devicePath.path();
    }

    //scan interface to refer to when scan

    scanInterface = new QDBusInterface("org.freedesktop.NetworkManager",
                              nmPath, "org.freedesktop.NetworkManager.Device.Wireless",
                              QDBusConnection::systemBus());


    getSavedConnections();      //refresh saved networks
    getActiveConnections();     //refresh connected network

    startScan();
}

//SCAN AVALIABLE WIFI NETWORKS

void WiFiManager::startScan(){

    //std::cout << "startScan called\n";

    watcher = new QDBusPendingCallWatcher(
        scanInterface->asyncCall("RequestScan", QVariant::fromValue(QVariantMap())),
        this);

    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, &WiFiManager::endScan);
}

void WiFiManager::endScan(){

    //std::cout << "endScan called\n";

    watcher->disconnect(watcher, &QDBusPendingCallWatcher::finished,
                        this, &WiFiManager::endScan);

    avaliableConnections.clear();

    QString hotspotSsidFilter = "";

    QDBusInterface settingsInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager/Settings",
        "org.freedesktop.NetworkManager.Settings",
        QDBusConnection::systemBus());

    QDBusReply<QList<QDBusObjectPath>> connections = settingsInterface.call("ListConnections");

    for (const QDBusObjectPath &connPath : connections.value()) {
        QDBusInterface conn(
            "org.freedesktop.NetworkManager",
            connPath.path(),
            "org.freedesktop.NetworkManager.Settings.Connection",
            QDBusConnection::systemBus());

        QDBusReply<QMap<QString, QVariantMap>> settingsReply = conn.call("GetSettings");

        if (settingsReply.isValid()) {
            QMap<QString, QVariantMap> res = settingsReply.value();

            // Перевіряємо ID профілю, який ми самі задали при створенні
            if (res["connection"]["id"].toString() == "SCP_Hotspot_Profile") {
                // SSID лежить у секції wireless і має тип QByteArray
                QByteArray ssidData = res["802-11-wireless"]["ssid"].toByteArray();
                hotspotSsidFilter = QString::fromUtf8(ssidData);
                break;
            }
        }
    }


    QDBusInterface wifiInterface(
        "org.freedesktop.NetworkManager",
        nmPath,
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus());

    QDBusReply<QVariant> wifis = wifiInterface.call(
        "Get",
        "org.freedesktop.NetworkManager.Device.Wireless",
        "AccessPoints");

    QList<QDBusObjectPath> accessPoints = pathListFromVariant(wifis.value());

    //main loop, gets info on all avaliable wifis
    for (const QDBusObjectPath &apPath : accessPoints) {
        QDBusInterface apInterface(
            "org.freedesktop.NetworkManager",
            apPath.path(),
            "org.freedesktop.DBus.Properties",
            QDBusConnection::systemBus());

        //get info on wifi network

        QDBusReply<QVariant> ssidReply = apInterface.call(
            "Get",
            "org.freedesktop.NetworkManager.AccessPoint",
            "Ssid");

        QDBusReply<QVariant> frequencyReply = apInterface.call(
            "Get",
            "org.freedesktop.NetworkManager.AccessPoint",
            "Frequency");

        QDBusReply<QVariant> strengthReply = apInterface.call(
            "Get",
            "org.freedesktop.NetworkManager.AccessPoint",
            "Strength");

        QDBusReply<QVariant> rsnReply = apInterface.call(
            "Get",
            "org.freedesktop.NetworkManager.AccessPoint",
            "nRsFlags");




        if(QString::fromUtf8(ssidReply.value().toByteArray()) != "" &&
            QString::fromUtf8(ssidReply.value().toByteArray()) != hotspotSsidFilter){

            //parse and assemble into struct

            info currentWIFI;   //the wifi which is currently being processed

            currentWIFI.ssid = QString::fromUtf8(ssidReply.value().toByteArray());
            currentWIFI.frequency = frequencyReply.value().toUInt();
            currentWIFI.strength = strengthReply.value().toInt(); //percent
            currentWIFI.path = apPath.path();
            uint32_t rsnFlags = rsnReply.value().toUInt();

            if (rsnFlags == 0) {
                currentWIFI.type = "nopass";
            }
            else if (rsnFlags & 0x100 || rsnFlags & 0x200) {
                currentWIFI.type = "WPA";
            }
            else if (rsnFlags & 0x1 || rsnFlags & 0x2) {
                currentWIFI.type = "WEP";
            }
            else {
                currentWIFI.type = "WPA";
            }


            //add flags

            if(active.contains(currentWIFI.ssid)){      //check if is connected to this wifi
                currentWIFI.ip = myIP;
                currentWIFI.st.append(state::connected);
            }
            else
                currentWIFI.st.append(state::disconnected);

            if(known.contains(currentWIFI.ssid)){       //check if this wifi is saved
                currentWIFI.st.append(state::saved);
            }

            avaliableConnections.append(currentWIFI);

            //std::cout << currentWIFI.ssid.toStdString() << '\n';
        }
    }
    emit scanEnded();
}

//GET INFO FROM NETWORK_MANAGER

void WiFiManager::getSavedConnections(){

    QDBusInterface saved("org.freedesktop.NetworkManager",
                         "/org/freedesktop/NetworkManager/Settings",
                         "org.freedesktop.NetworkManager.Settings",
                         QDBusConnection::systemBus());


    QDBusReply<QList<QDBusObjectPath>> connections = saved.call("ListConnections");
    qDBusRegisterMetaType<ConnectionDetails>();

    for(QDBusObjectPath savedConnection : connections.value()){

        QDBusInterface connectionInterface(
            "org.freedesktop.NetworkManager",
            savedConnection.path(),
            "org.freedesktop.NetworkManager.Settings.Connection",
            QDBusConnection::systemBus());

        QDBusReply<ConnectionDetails> connectionSettings = connectionInterface.call("GetSettings");
        ConnectionDetails settings = connectionSettings.value();
        QVariantMap wifiSettings = settings.value("802-11-wireless");
        QByteArray ssid = wifiSettings.value("ssid").toByteArray();
        if(ssid != ""){
            known.append(ssid);
            savedPaths[ssid] = savedConnection.path();
            //list saved ssid
            //std::cout << " - " << QString(ssid).toStdString() << std::endl;
        }
    }
}

void WiFiManager::getActiveConnections(){
    //get active connections

    active.clear();

    QDBusInterface activeIF(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus());

    QDBusReply<QVariant> activeReply = activeIF.call(
        "Get",
        "org.freedesktop.NetworkManager",
        "ActiveConnections");

    activeConnections = pathListFromVariant(activeReply.value());

    for(QDBusObjectPath activeConnection : activeConnections){

        QDBusInterface iface(
            "org.freedesktop.NetworkManager",
            activeConnection.path(),
            "org.freedesktop.DBus.Properties",
            QDBusConnection::systemBus());

        QDBusReply<QVariant> devicesActive = iface.call(
            "Get",
            "org.freedesktop.NetworkManager.Connection.Active",
            "Devices");

        QList<QDBusObjectPath> activeDevices = pathListFromVariant(devicesActive.value());

        for(QDBusObjectPath activeDevice : activeDevices){

            QDBusInterface deviceInterface(
                "org.freedesktop.NetworkManager",
                activeDevice.path(),
                "org.freedesktop.DBus.Properties",
                QDBusConnection::systemBus());

            QDBusReply<QVariant> deviceTypeReply = deviceInterface.call(
                "Get",
                "org.freedesktop.NetworkManager.Device",
                "DeviceType");

            if (deviceTypeReply.isValid() && deviceTypeReply.value().toUInt() == 2) { // 2 = Wi-Fi

                QDBusReply<QVariant> apPathReply = deviceInterface.call(
                    "Get",
                    "org.freedesktop.NetworkManager.Device.Wireless",
                    "ActiveAccessPoint");

                QDBusObjectPath activeAP = qvariant_cast<QDBusObjectPath>(apPathReply.value());

                if (apPathReply.isValid()) {

                    //GET SSID

                    QDBusInterface apInterface(
                        "org.freedesktop.NetworkManager",
                        activeAP.path(),
                        "org.freedesktop.DBus.Properties",
                        QDBusConnection::systemBus());

                    QDBusReply<QVariant> ssidReply = apInterface.call(
                        "Get",
                        "org.freedesktop.NetworkManager.AccessPoint",
                        "Ssid");

                    if (ssidReply.isValid()) {
                        QString ssid = QString::fromUtf8(ssidReply.value().toByteArray());
                        active.append(ssid);
                    }

                    //GET IP

                    QDBusReply<QVariant> ipConfig = iface.call(
                        "Get",
                        "org.freedesktop.NetworkManager.Connection.Active",
                        "Ip4Config");

                    QDBusObjectPath ipConfigPath = qvariant_cast<QDBusObjectPath>(ipConfig.value());

                    QDBusInterface ipConfigInterface(
                        "org.freedesktop.NetworkManager",
                        ipConfigPath.path(),
                        "org.freedesktop.DBus.Properties",
                        QDBusConnection::systemBus());

                    QDBusReply<QVariant> ip = ipConfigInterface.call(
                        "Get",
                        "org.freedesktop.NetworkManager.IP4Config",
                        "Addresses");

                    //std::cout << ip.error().message().toStdString() << '\n';
                    QDBusArgument dbusArgument = ip.value().value<QDBusArgument>();
                    QList<QList<uint>> addresses;

                    dbusArgument.beginArray();
                    while (!dbusArgument.atEnd()) {
                        QList<uint> addressEntry;
                        dbusArgument.beginArray();
                        while (!dbusArgument.atEnd()) {
                            uint value;
                            dbusArgument >> value;
                            addressEntry.append(value);
                        }
                        dbusArgument.endArray();
                        addresses.append(addressEntry);
                    }
                    dbusArgument.endArray();

                    for (const QList<uint> &entry : addresses) {
                        if (!entry.isEmpty()) {
                            uint rawIP = entry[0];
                            myIP = QString("%1.%2.%3.%4")
                                       .arg(rawIP & 0xFF)
                                       .arg((rawIP >> 8) & 0xFF)
                                       .arg((rawIP >> 16) & 0xFF)
                                       .arg((rawIP >> 24) & 0xFF);
                        }
                    }
                }
            }
        }
    }
}

//TURN WIFI ON AND OFF

bool WiFiManager::getWifiStatus(){
    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus());

    QDBusReply<QVariant> reply = nmInterface.call("Get",
                                                  "org.freedesktop.NetworkManager",
                                                  "WirelessEnabled");

    if (reply.isValid()) {
        return reply.value().toBool();
    }

    return false;
}

void WiFiManager::turnOn(){
    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus());

    nmInterface.call("Set",
                     "org.freedesktop.NetworkManager",
                     "WirelessEnabled",
                     QVariant::fromValue(QDBusVariant(true)));
}

void WiFiManager::turnOff(){
    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus());

    nmInterface.call("Set",
                     "org.freedesktop.NetworkManager",
                     "WirelessEnabled",
                     QVariant::fromValue(QDBusVariant(false)));
}

//CONNECTION SEQUENSE

void WiFiManager::tryConnect(info wifi, PreviewContainer* sender){

    if(!wifi.st.contains(saved)){

        //if not saved

        ConnectionDetails connectionSettings;

        QVariantMap connection;
        connection["id"] = wifi.ssid;
        connection["type"] = "802-11-wireless";
        connection["uuid"] = QUuid::createUuid().toString(QUuid::WithoutBraces);

        QVariantMap wireless;
        wireless["ssid"] = wifi.ssid.toUtf8();
        wireless["mode"] = "infrastructure";

        QVariantMap wirelessSecurity;
        wirelessSecurity["key-mgmt"] = "wpa-psk";
        wirelessSecurity["psk"] = wifi.password;

        QVariantMap ipv4;
        ipv4["method"] = "auto";
        QVariantMap ipv6;
        ipv6["method"] = "auto";

        connectionSettings["connection"] = connection;
        connectionSettings["802-11-wireless"] = wireless;
        connectionSettings["802-11-wireless-security"] = wirelessSecurity;
        connectionSettings["ipv4"] = ipv4;
        connectionSettings["ipv6"] = ipv6;

        QDBusInterface settingsInterface(
            "org.freedesktop.NetworkManager",
            "/org/freedesktop/NetworkManager/Settings",
            "org.freedesktop.NetworkManager.Settings",
            QDBusConnection::systemBus());

        QDBusReply<QDBusObjectPath> addConnectionReply = settingsInterface.call("AddConnection",
                                                                                QVariant::fromValue(connectionSettings));

        if (!addConnectionReply.isValid()) {
            if(addConnectionReply.error().message() ==
                "802-11-wireless-security.psk: property is invalid")
                sender->showStatus("Wrong password");
            else
                sender->showStatus("Unknown error");
            return;
        }else{
            savedPaths[wifi.ssid] = addConnectionReply.value().path();
        }
    }


    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.NetworkManager",
        QDBusConnection::systemBus());


    QDBusPendingCall asyncActivateCall = nmInterface.asyncCall(
        "ActivateConnection",
        QDBusObjectPath(savedPaths[wifi.ssid]),
        QDBusObjectPath(nmPath),
        QDBusObjectPath("/"));


    processedSender = sender;
    processedWifi = wifi;

    watcher = new QDBusPendingCallWatcher(asyncActivateCall, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &WiFiManager::processActivateReply);
}

void WiFiManager::processActivateReply(QDBusPendingCallWatcher* call){

    QDBusPendingReply<QDBusObjectPath> reply = *call;
    //std::cout << reply.value().path().toStdString() << '\n';


    if (reply.isValid()) {
        QString path = reply.value().path();

        //std::cout << path.toStdString() << '\n';

        QDBusConnection::systemBus().connect(
            "org.freedesktop.NetworkManager",
            path,
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            this,
            SLOT(onConnectionPropertiesChanged(QString, QVariantMap, QStringList)));
    } else {
        //processedSender->showStatus("Activation failed: " + reply.error().message());
        //std::cout << reply.error().message().toStdString() << '\n';
    }

    call->deleteLater();
    watcher->deleteLater();
}

void WiFiManager::onConnectionPropertiesChanged(const QString &interface,
                                               const QVariantMap &changedProperties,
                                               const QStringList &invalidatedProperties){

    info wifi = processedWifi;


    if (interface == "org.freedesktop.NetworkManager.Connection.Active") {
        if (changedProperties.contains("State")) {
            uint state = changedProperties.value("State").toUInt();

            if (state == 2) {// NM_ACTIVE_CONNECTION_STATE_ACTIVATED

                QString oldssid = "h789h87h87h897h98h897h89h897h";

                if (active.count() > 0)
                    oldssid = active.at(0);     //old connected wifi

                getActiveConnections();

                QString newssid = active.at(0);     //new connected wifi

                emit connectionChanged(oldssid, newssid);

                wifi.st.clear();
                wifi.st.append(saved);
                wifi.st.append(connected);
                wifi.ip = myIP;


                processedSender->showStatus("Connected successfully");
                processedSender->stateChanged(wifi);

            }else if (state == 0 || state == 4 || state == 3) {
                // 0 - NM_ACTIVE_CONNECTION_STATE_NONE
                // 1 - NM_ACTIVE_CONNECTION_STATE_ACTIVATING - not used
                // 3 - NM_ACTIVE_CONNECTION_STATE_DEACTIVATING
                // 4 - NM_ACTIVE_CONNECTION_STATE_DEACTIVATED

                processedSender->showStatus("Wrong password");

                wifi.st.clear();
                wifi.st.append(disconnected);

                QDBusInterface connectionInterface(
                    "org.freedesktop.NetworkManager",
                    savedPaths[wifi.ssid],
                    "org.freedesktop.NetworkManager.Settings.Connection",
                    QDBusConnection::systemBus());

                connectionInterface.call("Delete");

                processedSender->stateChanged(wifi);
            }
        }
    }
}



//DISCONNECT FROM WIFI

void WiFiManager::disconnectFromWifi(info wifi, PreviewContainer* sender){
    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.NetworkManager",
        QDBusConnection::systemBus());

    QDBusReply<void> deactivateReply;
    for(QDBusObjectPath o : activeConnections)
        deactivateReply = nmInterface.call("DeactivateConnection", o);


    if (!deactivateReply.isValid()){
        //"Can't disconnect"
        sender->showStatus("Can't disconnect"); //deactivateReply.error().message()
    }
    else{
        sender->showStatus("Disonnected successfully");
        activeConnections.clear();
        active.clear();

        emit disconnectedFromWifi(wifi.ssid);
    }
}

//DELETE WIFI

void WiFiManager::deleteConnection(info wifi, PreviewContainer* sender){

    //wifi to delete
    QDBusInterface connectionInterface(
        "org.freedesktop.NetworkManager",
        savedPaths[wifi.ssid],
        "org.freedesktop.NetworkManager.Settings.Connection",
        QDBusConnection::systemBus());


    QDBusReply<void> deleteConnectionReply = connectionInterface.call("Delete");

    if (!deleteConnectionReply.isValid()) {
        sender->showStatus(deleteConnectionReply.error().message());
    } else {
        sender->showStatus("Deleted successfuly");
        getSavedConnections();

        emit connectionDeleted(wifi.ssid);
    }

}

//HOTSPOT MANAGEMENT

bool WiFiManager::getHotspotStatus(){
    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus());

    // Отримуємо список активних з'єднань
    QDBusReply<QVariant> activeReply = nmInterface.call("Get",
                                                        "org.freedesktop.NetworkManager",
                                                        "ActiveConnections");

    if (!activeReply.isValid()) return false;

    // Використовуємо твій готовий парсер pathListFromVariant
    QList<QDBusObjectPath> activePaths = pathListFromVariant(activeReply.value());

    for (const QDBusObjectPath &connPath : activePaths) {
        QDBusInterface activeConnInterface(
            "org.freedesktop.NetworkManager",
            connPath.path(),
            "org.freedesktop.DBus.Properties",
            QDBusConnection::systemBus());

        // Отримуємо ID (наприклад, "SCP_Hotspot_Profile")
        QDBusReply<QVariant> idReply = activeConnInterface.call("Get",
                                                                "org.freedesktop.NetworkManager.Connection.Active",
                                                                "Id");

        if (idReply.isValid() && idReply.value().toString() == "SCP_Hotspot_Profile") {
            return true; // Знайшли активний хотспот
        }
    }

    return false; // Активного хотспоту немає
}

void WiFiManager::startHotspot(info wifi){

    QVariantMap connection;
    connection["type"] = "802-11-wireless";
    connection["uuid"] = QUuid::createUuid().toString().remove('{').remove('}');
    connection["id"] = "SCP_Hotspot_Profile";
    connection["autoconnect"] = false;

    QVariantMap wireless;
    wireless["ssid"] = wifi.ssid.toUtf8();
    wireless["mode"] = "ap";
    // wireless["band"] = "bg";

    QVariantMap security;
    security["key-mgmt"] = "wpa-psk";
    security["psk"] = wifi.password;

    QVariantMap ip4;
    ip4["method"] = "shared";

    QMap<QString, QVariantMap> settings;
    settings["connection"] = connection;
    settings["802-11-wireless"] = wireless;
    settings["802-11-wireless-security"] = security;
    settings["ipv4"] = ip4;

    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.NetworkManager",
        "AddAndActivateConnection");

    QVariant settingsVariant;
    settingsVariant.setValue(settings);

    message << settingsVariant;
    message << QVariant::fromValue(QDBusObjectPath(nmPath));
    message << QVariant::fromValue(QDBusObjectPath("/"));

    /*QDBusMessage reply = */QDBusConnection::systemBus().call(message);

    /*if (reply.type() == QDBusMessage::ReplyMessage) {
        // reply.arguments().at(0) - Connection
        // reply.arguments().at(1) - ActiveConnection
        std::cout << "Хотспот запущено успішно. Шлях: "
                  << reply.arguments().at(1).value<QDBusObjectPath>().path().toStdString();
    } else {
        std::cout << "Не вдалося запустити хотспот: " << reply.errorMessage().toStdString();
    }*/
}

void WiFiManager::stopHotspot(){

    QDBusInterface deviceInterface(
        "org.freedesktop.NetworkManager",
        nmPath,
        "org.freedesktop.NetworkManager.Device",
        QDBusConnection::systemBus());

    deviceInterface.call("Disconnect");
    //std::cout << "disconnected\n";

    QDBusInterface settingsInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager/Settings",
        "org.freedesktop.NetworkManager.Settings",
        QDBusConnection::systemBus());

    QDBusReply<QList<QDBusObjectPath>> connections = settingsInterface.call("ListConnections");
    for (const QDBusObjectPath &connPath : connections.value()) {
        QDBusInterface conn(
            "org.freedesktop.NetworkManager",
            connPath.path(),
            "org.freedesktop.NetworkManager.Settings.Connection",
            QDBusConnection::systemBus());

        QDBusReply<QMap<QString, QVariantMap>> settings = conn.call("GetSettings");
        if (settings.isValid() && settings.value()["connection"]["id"].toString() == "SCP_Hotspot_Profile") {
            conn.call("Delete");
            //std::cout << "deleted\n";
            break;
        }
    }
}

//PARSER

QList<QDBusObjectPath> WiFiManager::pathListFromVariant(QVariant var){
    QList<QDBusObjectPath> result;
    const QDBusArgument &dbusArg = var.value<QDBusArgument>();
    dbusArg >> result;
    return result;
}
