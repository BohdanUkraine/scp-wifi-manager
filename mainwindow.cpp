#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include <QtDBus/QDBusError>
#include <iostream>
#include <QtDBus/QDBusMetaType>
#include <QTimer>

//known bugs:
//when starting program with disconnected wifi, and connecting to wifi - there is no ip address

typedef QMap<QString, QMap<QString, QVariant> > ConnectionDetails;
Q_DECLARE_METATYPE(ConnectionDetails)

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    //window
    setFixedSize(width, height);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowTitle("wifi manager");

    //retry, hotspot, on/off buttons

    int spacing = (width/5);
    int firstWidth = spacing - (buttonWidth/2);


    retry = new QPushButton(this);
    retry->setGeometry(firstWidth, 9, buttonWidth, buttonWidth);
    retry->setIcon(QIcon(QPixmap("imgs/retry.png")));
    retry->setIconSize(QSize(17,17));
    retry->setStyleSheet(cs1);
    connect(retry, &QPushButton::clicked, this, &mainWindow::requestScan);


    toggleHotspot = new QPushButton(this);
    toggleHotspot->setGeometry(firstWidth + spacing, 9, buttonWidth, buttonWidth);
    toggleHotspot->setIcon(QIcon(QPixmap("imgs/hotspot.png")));
    toggleHotspot->setIconSize(QSize(20,20));
    toggleHotspot->setStyleSheet(cs1);


    togglePower = new QPushButton(this);
    togglePower->setGeometry(firstWidth + spacing*2, 9, buttonWidth, buttonWidth);
    togglePower->setIcon(QIcon(QPixmap("imgs/power.png")));
    togglePower->setIconSize(QSize(20,20));
    togglePower->setStyleSheet(cs1);


    settingsButton = new QPushButton(this);
    settingsButton->setGeometry(firstWidth + spacing*3, 9, buttonWidth, buttonWidth);
    settingsButton->setIcon(QIcon(QPixmap("imgs/settings.png")));
    settingsButton->setIconSize(QSize(18,18));
    settingsButton->setStyleSheet(cs1);
    connect(settingsButton, &QPushButton::clicked, this, &mainWindow::toggleSettings);


    //visual

    line = new QFrame(this);
    line->setGeometry(QRect(10, 47, width - 20, 3));
    line->setFrameShape(QFrame::HLine);
    QPalette linePal = QPalette();
    linePal.setColor(QPalette::Window, Qt::black);
    line->setPalette(linePal);


    QPalette background = QPalette();
    background.setColor(QPalette::Window, backgroundColor);
    setPalette(background);

    //areas

    scrollBox = new QScrollArea(this);
    contentWidget = new QWidget();
    vBox = new QVBoxLayout(contentWidget);

    scrollBox->setGeometry(QRect(0, 50, 400, 250));
    scrollBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollBox->setWidget(contentWidget);
    scrollBox->setWidgetResizable(true);

    vBox->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    vBox->setMargin(0);


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

    scan = new QDBusInterface("org.freedesktop.NetworkManager",
        nmPath, "org.freedesktop.NetworkManager.Device.Wireless",
        QDBusConnection::systemBus());

    //load saved names


    QDBusInterface sved("org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager/Settings",
        "org.freedesktop.NetworkManager.Settings",
        QDBusConnection::systemBus());


    QDBusReply<QList<QDBusObjectPath>> rply = sved.call("ListConnections");
    qDBusRegisterMetaType<ConnectionDetails>();

    for(QDBusObjectPath s : rply.value()){

        QDBusInterface connectionInterface(
            "org.freedesktop.NetworkManager",
            s.path(),
            "org.freedesktop.NetworkManager.Settings.Connection",
            QDBusConnection::systemBus());

        QDBusReply<ConnectionDetails> connectionReply = connectionInterface.call("GetSettings");

        //std::cout << "error: " << connectionReply.error().message().toStdString() << '\n';
        ConnectionDetails settings = connectionReply.value();

        QVariantMap wifiSettings = settings.value("802-11-wireless");
        QByteArray ssid = wifiSettings.value("ssid").toByteArray();
        if(ssid != ""){
            known.append(ssid);
            savedPaths[ssid] = s.path();
            //std::cout << " - " << QString(ssid).toStdString() << std::endl;
        }
        //std::cout << s.path().toStdString() << '\n';
    }

    requestScan();
    //refreshList();

}

void mainWindow::requestScan(){
    scan->call("RequestScan",QVariant::fromValue(QVariantMap()));
    /*PreviewContainer *test2 = new PreviewContainer("ssid", 1);
    vBox->addWidget(test2);
    PreviewContainer *test1 = new PreviewContainer("ssid", 1);
    vBox->addWidget(test1);*/
    QTimer::singleShot(1500, this, &mainWindow::refreshList);
}

void mainWindow::toggleSettings(){

}


QList<QDBusObjectPath> mainWindow::pathListFromVariant(QVariant var){
    QList<QDBusObjectPath> result;
    const QDBusArgument &dbusArg = var.value<QDBusArgument>();
    dbusArg >> result;
    return result;
}

void mainWindow::getActiveConnections(){
    //get active connections

    active.clear();

    QDBusInterface activeIF(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus());

    QDBusReply<QVariant> activeReply = activeIF.call("Get",
        "org.freedesktop.NetworkManager", "ActiveConnections");
    activeConnections = pathListFromVariant(activeReply.value());

    for(QDBusObjectPath p : activeConnections){

        QDBusInterface iface(
            "org.freedesktop.NetworkManager",
            p.path(),
            "org.freedesktop.DBus.Properties",
            QDBusConnection::systemBus());

        QDBusReply<QVariant> devicesActive = iface.call("Get",
            "org.freedesktop.NetworkManager.Connection.Active", "Devices");

        QList<QDBusObjectPath> activeDevices = pathListFromVariant(devicesActive.value());

        for(QDBusObjectPath dp : activeDevices){

            QDBusInterface deviceInterface(
                "org.freedesktop.NetworkManager",
                dp.path(),
                "org.freedesktop.DBus.Properties",
                QDBusConnection::systemBus());

            QDBusReply<QVariant> deviceTypeReply = deviceInterface.call("Get",
                "org.freedesktop.NetworkManager.Device", "DeviceType");

            if (deviceTypeReply.isValid() && deviceTypeReply.value().toUInt() == 2) { // 2 = Wi-Fi
                //GET IP
                QDBusReply<QVariant> ipConfig = iface.call("Get",
                                                           "org.freedesktop.NetworkManager.Connection.Active", "Ip4Config");

                QDBusObjectPath ipConfigPath = qvariant_cast<QDBusObjectPath>(ipConfig.value());

                QDBusInterface ipConfigInterface(
                    "org.freedesktop.NetworkManager",
                    ipConfigPath.path(),
                    "org.freedesktop.DBus.Properties",
                    QDBusConnection::systemBus());

                QDBusReply<QVariant> ip = ipConfigInterface.call("Get",
                                                                 "org.freedesktop.NetworkManager.IP4Config", "Addresses");

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

                QDBusReply<QVariant> apPathReply = deviceInterface.call("Get",
                    "org.freedesktop.NetworkManager.Device.Wireless", "ActiveAccessPoint");

                QDBusObjectPath lol = qvariant_cast<QDBusObjectPath>(apPathReply.value());

                if (apPathReply.isValid()) {

                    QDBusInterface apInterface(
                        "org.freedesktop.NetworkManager",
                        lol.path(),
                        "org.freedesktop.DBus.Properties",
                        QDBusConnection::systemBus());

                    QDBusReply<QVariant> ssidReply = apInterface.call("Get",
                        "org.freedesktop.NetworkManager.AccessPoint", "Ssid");

                    if (ssidReply.isValid()) {
                        QString ssid = QString::fromUtf8(ssidReply.value().toByteArray());
                        active.append(ssid);
                    }
                }
            }
        }
    }
}

void mainWindow::clearVBox(){
    if ( vBox->layout() != NULL )
    {
        QLayoutItem* item;
        while ( ( item = vBox->layout()->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        //delete vBox->layout();
    }
    contentWidget->setMinimumHeight(0);
    contentWidget->adjustSize();
}

void mainWindow::refreshList(){

    getActiveConnections();
    clearVBox();


    QDBusInterface wifiInterface(
        "org.freedesktop.NetworkManager",
        nmPath,
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus());

    QDBusReply<QVariant> reply1 = wifiInterface.call("Get",
        "org.freedesktop.NetworkManager.Device.Wireless", "AccessPoints");

    QList<QDBusObjectPath> accessPoints = pathListFromVariant(reply1.value());

    //main loop, creates containers
    for (const QDBusObjectPath &apPath : accessPoints) {
        QDBusInterface apInterface(
            "org.freedesktop.NetworkManager",
            apPath.path(),
            "org.freedesktop.DBus.Properties",
            QDBusConnection::systemBus());

        QDBusReply<QVariant> ssidReply = apInterface.call("Get",
            "org.freedesktop.NetworkManager.AccessPoint", "Ssid");
        QDBusReply<QVariant> frequencyReply = apInterface.call("Get",
            "org.freedesktop.NetworkManager.AccessPoint", "Frequency");
        QDBusReply<QVariant> strengthReply = apInterface.call("Get",
            "org.freedesktop.NetworkManager.AccessPoint", "Strength");

        info currentWIFI;
        currentWIFI.frequency = frequencyReply.value().toUInt();
        currentWIFI.strength = strengthReply.value().toInt(); //percent
        currentWIFI.ssid = QString::fromUtf8(ssidReply.value().toByteArray());
        currentWIFI.path = apPath.path();

        if(currentWIFI.ssid != ""){

            if(active.contains(currentWIFI.ssid)){
                currentWIFI.ip = myIP;
                //std::cout << currentWIFI.ip.toStdString() << "\n";
                currentWIFI.st.append(state::connected);
            }
            else
                currentWIFI.st.append(state::disconnected);

            if(known.contains(currentWIFI.ssid ))
                currentWIFI.st.append(state::saved);


            PreviewContainer *test = new PreviewContainer(currentWIFI);
            test->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            test->setFixedHeight(50);
            vBox->addWidget(test);
            connect(test, &PreviewContainer::tryConnectSignal, this, &mainWindow::tryConnect);
            connect(test, &PreviewContainer::disconnectSignal, this, &mainWindow::disconnectFromWifi);
            connect(test, &PreviewContainer::deleteConnectionSignal, this, &mainWindow::deleteConnection);


            containers.append(test);
        }
    }

    contentWidget->setMinimumHeight(vBox->sizeHint().height());
    contentWidget->adjustSize();

    scrollBox->setWidgetResizable(true);
    scrollBox->update();
}

void mainWindow::disconnectFromWifi(info wifi, PreviewContainer* sender){
    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.NetworkManager",
        QDBusConnection::systemBus());

    wifi.st.clear();
    wifi.st.append(saved);

    for(PreviewContainer* c : containers){
        info tmp = c->getInfo();
        if(active.contains(tmp.ssid)){
            tmp.st.clear();
            tmp.st.append(disconnected);
            tmp.st.append(saved);
            c->stateChanged(tmp);
        }
    }
    getActiveConnections();

    QDBusReply<void> deactivateReply;

    for(QDBusObjectPath o : activeConnections)
        deactivateReply = nmInterface.call("DeactivateConnection", o);
    activeConnections.clear();
    active.clear();


    if (!deactivateReply.isValid()){
        //"Can't disconnect"
        sender->showStatus("Can't disconnect"); //deactivateReply.error().message()
        wifi.st.append(connected);
    }
    else{
        sender->showStatus("Disonnected successfully");
        wifi.st.append(disconnected);
    }

    sender->stateChanged(wifi);
}

void mainWindow::tryConnect(info wifi, PreviewContainer* sender){

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
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &mainWindow::processActivateReply);
}

void mainWindow::processActivateReply(QDBusPendingCallWatcher* call){

    QDBusPendingReply<QDBusObjectPath> reply = *call;
    //std::cout << reply.value().path().toStdString() << '\n';


    if (reply.isValid()) {
        QString path = reply.value().path();

        std::cout << path.toStdString() << '\n';

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

void mainWindow::onConnectionPropertiesChanged(const QString &interface,
                                               const QVariantMap &changedProperties,
                                               const QStringList &invalidatedProperties){

    info wifi = processedWifi;


    if (interface == "org.freedesktop.NetworkManager.Connection.Active") {
        if (changedProperties.contains("State")) {
            uint state = changedProperties.value("State").toUInt();

            if (state == 2) {// NM_ACTIVE_CONNECTION_STATE_ACTIVATED
                getActiveConnections();

                wifi.st.clear();
                wifi.st.append(saved);
                wifi.st.append(connected);
                wifi.ip = myIP;


                for(PreviewContainer* c : containers){
                    info tmp = c->getInfo();
                    if(tmp.ssid == wifi.ssid && tmp.path != wifi.path){
                        tmp.st.clear();
                        tmp.st.append(connected);
                        tmp.st.append(saved);
                        tmp.ip = myIP;
                        c->stateChanged(tmp);
                    }
                }

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


void mainWindow::deleteConnection(info wifi, PreviewContainer* sender){

    //wifi to delete
    QDBusInterface connectionInterface(
        "org.freedesktop.NetworkManager",
        savedPaths[wifi.ssid],
        "org.freedesktop.NetworkManager.Settings.Connection",
        QDBusConnection::systemBus());


    QDBusReply<void> deleteConnectionReply = connectionInterface.call("Delete");

    if (!deleteConnectionReply.isValid()) {
        sender->showStatus(deleteConnectionReply.error().message());
        //std::cout << deleteConnectionReply.error().message().toStdString() << '/n';
    } else {
        sender->showStatus("Deleted successfuly");
    }


    //refresh visual
    getActiveConnections();
    for(PreviewContainer* c : containers){
        info tmp = c->getInfo();
        if(tmp.ssid == wifi.ssid  && tmp.path != wifi.path){
            tmp.st.clear();
            tmp.st.append(disconnected);
            c->stateChanged(tmp);
        }
    }
    wifi.st.clear();
    wifi.st.append(disconnected);
    sender->stateChanged(wifi);
}

mainWindow::~mainWindow()
{
    delete ui;
}
