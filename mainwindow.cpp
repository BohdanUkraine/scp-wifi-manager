#include "mainwindow.h"
#include "ui_mainwindow.h"

//known bugs:
//when starting program with disconnected wifi, and connecting to wifi - there is no ip address
//sometimes deletes connection when disconnecting or changing connections

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

    wifimanager = new WiFiManager();
    connect(wifimanager, &WiFiManager::scanEnded, this, &mainWindow::displayScanResults);
    connect(wifimanager, &WiFiManager::connectionChanged, this, &mainWindow::refreshUiOnConnectionChanged);
    connect(wifimanager, &WiFiManager::disconnectedFromWifi, this, &mainWindow::refreshUiOnDisconnected);
    connect(wifimanager, &WiFiManager::connectionDeleted, this, &mainWindow::refreshUiOnConnectionDeleted);


    int spacing = (width/5);
    int firstWidth = spacing - (buttonWidth/2);


    retry = new QPushButton(this);
    retry->setGeometry(firstWidth, 9, buttonWidth, buttonWidth);
    retry->setIcon(QIcon(QPixmap("imgs/retry.png")));
    retry->setIconSize(QSize(17,17));
    retry->setStyleSheet(cs1);
    connect(retry, &QPushButton::clicked, wifimanager, &WiFiManager::startScan);



    toggleHotspotButton = new QPushButton(this);
    toggleHotspotButton->setGeometry(firstWidth + spacing, 9, buttonWidth, buttonWidth);
    toggleHotspotButton->setIcon(QIcon(QPixmap("imgs/hotspot.png")));
    toggleHotspotButton->setIconSize(QSize(20,20));

    if(wifimanager->getHotspotStatus()) //hotspot is up
        toggleHotspotButton->setStyleSheet(hotspotWorksStyle);
    else                                //hotspot is down
        toggleHotspotButton->setStyleSheet(cs1);

    connect(toggleHotspotButton, &QPushButton::clicked, this, &mainWindow::toggleHotspot);



    togglePower = new QPushButton(this);
    togglePower->setGeometry(firstWidth + spacing*2, 9, buttonWidth, buttonWidth);
    togglePower->setIcon(QIcon(QPixmap("imgs/power.png")));
    togglePower->setIconSize(QSize(20,20));

    if(wifimanager->getWifiStatus()) //hotspot is up
        togglePower->setStyleSheet(hotspotWorksStyle);
    else                                //hotspot is down
        togglePower->setStyleSheet(cs1);

    connect(togglePower, &QPushButton::clicked, this, &mainWindow::togglewifi);



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

    scrollBar = new QScrollBar();
    scrollBar->setStyleSheet(scrollStyle);

    scrollBox->setGeometry(QRect(0, 50, 400, 250));
    scrollBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollBox->setWidget(contentWidget);
    scrollBox->setWidgetResizable(true);
    scrollBox->setVerticalScrollBar(scrollBar);

    vBox->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    vBox->setMargin(0);

    //settings
    settingsTab = new SettingsTab(this);
    settingsTab->move(0,300);

    settingsSlideIn = new QPropertyAnimation(settingsTab, "pos", this);
    settingsSlideIn->setDuration(70);
    settingsSlideIn->setStartValue(QPoint(0, 300));
    settingsSlideIn->setEndValue(QPoint(0, 50));

    settingsSlideOut = new QPropertyAnimation(settingsTab, "pos", this);
    settingsSlideOut->setDuration(70);
    settingsSlideOut->setStartValue(QPoint(0, 50));
    settingsSlideOut->setEndValue(QPoint(0, 300));

    settingsVisible = false;

    connect(settingsTab, &SettingsTab::displayQrCodeSignal, this, &mainWindow::displayQrCode);


    //qr code

    qr = new QrContainer(this);
    qr->setFixedWidth(400);
    qr->setFixedHeight(300);

    qrSlideIn = new QPropertyAnimation(qr, "pos", this);
    qrSlideIn->setDuration(70);
    qrSlideIn->setStartValue(QPoint(0, 300));
    qrSlideIn->setEndValue(QPoint(0, 0));

    qr->move(0,300);
    qr->raise();

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

void mainWindow::displayScanResults(){

    clearVBox();
    containers.clear();

    for(info wifi : wifimanager->avaliableConnections){

        PreviewContainer *wifiOption = new PreviewContainer(wifi);
        wifiOption->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        wifiOption->setFixedHeight(50);
        vBox->addWidget(wifiOption);
        containers.append(wifiOption);
        connect(wifiOption, &PreviewContainer::tryConnectSignal, wifimanager, &WiFiManager::tryConnect);
        connect(wifiOption, &PreviewContainer::disconnectSignal, wifimanager, &WiFiManager::disconnectFromWifi);
        connect(wifiOption, &PreviewContainer::deleteConnectionSignal, wifimanager, &WiFiManager::deleteConnection);
        connect(wifiOption, &PreviewContainer::displayQrCodeSignal, this, &mainWindow::displayQrCode);

    }

    contentWidget->setMinimumHeight(vBox->sizeHint().height());
    contentWidget->adjustSize();

    scrollBox->setWidgetResizable(true);
    scrollBox->update();
}

void mainWindow::refreshContainers(QString ssid, QList<state> states, QString ip){

    for(PreviewContainer* container : containers){
        info processedInfo = container->getInfo();
        if(processedInfo.ssid.contains(ssid)){
            processedInfo.st.clear();
            processedInfo.st = states;
            processedInfo.ip = ip;
            container->stateChanged(processedInfo);
        }
    }
}

void mainWindow::refreshUiOnConnectionChanged(QString oldSsid, QString newSsid){

    refreshContainers(oldSsid, {disconnected, saved});
    refreshContainers(newSsid, {connected, saved}, wifimanager->myIP);
}

void mainWindow::refreshUiOnDisconnected(QString ssid){

    refreshContainers(ssid, {disconnected, saved});
}

void mainWindow::refreshUiOnConnectionDeleted(QString ssid){

    refreshContainers(ssid, {disconnected});
}

void mainWindow::displayQrCode(info wifiInfo){
    qr->generateQrCode(wifiInfo);
    qrSlideIn->start();
}

void mainWindow::toggleSettings(){
    if(!settingsVisible){
        //settingsSlideOut->stop();
        settingsSlideIn->start();
        settingsVisible = true;
        //std::cout << "slide in\n";
    }
    else{
        //settingsSlideIn->stop();
        settingsSlideOut->start();
        settingsVisible = false;
        //std::cout << "slide out\n";
    }
}

void mainWindow::toggleHotspot(){
    if(wifimanager->getHotspotStatus()){ //it works
        wifimanager->stopHotspot();
        toggleHotspotButton->setStyleSheet(cs1);
    } else{
        wifimanager->startHotspot(settingsTab->hotspotInfo);
        toggleHotspotButton->setStyleSheet(hotspotWorksStyle);
    }
}

void mainWindow::togglewifi(){
    if(wifimanager->getWifiStatus()){
        wifimanager->turnOff();
        togglePower->setStyleSheet(cs1);
        clearVBox();
    } else{
        wifimanager->turnOn();
        togglePower->setStyleSheet(hotspotWorksStyle);
        QTimer::singleShot(2000, wifimanager, &WiFiManager::startScan);
    }
}

mainWindow::~mainWindow()
{
    delete ui;
}
