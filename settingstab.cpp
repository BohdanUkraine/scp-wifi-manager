#include "settingstab.h"

#include <iostream>

SettingsTab::SettingsTab(QWidget *parent)
    : QWidget{parent}
{
    settings = new QSettings("SCP", "wifi_manager");
    win = this->window(); //top window reference

    size = QPoint(400,300);


    setFixedWidth(size.x());
    setFixedHeight(size.y()-size.y()/6);
    setAutoFillBackground(true);

    QPalette background = QPalette();
    background.setColor(QPalette::Window, defaultColor);
    setPalette(background);



    posLabel = new QLabel(this);
    posLabel->setGeometry(10,10,150,40);
    posLabel->setText("Postiton:");
    posLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    posLabel->setAlignment(Qt::AlignVCenter);
    posLabel->setStyleSheet("QLabel {font: 16pt; color: white;}");
    posLabel->setWordWrap(true);


    posButton = new QPushButton(this);
    posButton->setGeometry(200, 10, 40, 40);
    posButton->setStyleSheet(cs2);
    posButton->setCursor(Qt::SizeAllCursor);
    posButton->installEventFilter(this);


    reset = new QPushButton(this);
    reset->setGeometry(260, 10, 40, 40);
    reset->setIcon(QIcon(QPixmap("imgs/retry.png")));
    reset->setIconSize(QSize(17,17));
    reset->setStyleSheet(cs2);
    connect(reset, &QPushButton::clicked, this, &SettingsTab::resetClicked);

    //separator

    QPalette linePal = QPalette();
    linePal.setColor(QPalette::Window, Qt::black);

    line1 = new QFrame(this);
    line1->setGeometry(QRect(30, 59, 340, 2));
    line1->setFrameShape(QFrame::HLine);
    line1->setPalette(linePal);


    //hotspot settings

    ssidName = new QLabel(this);
    ssidName->setGeometry(10,70,200,50);
    ssidName->setText("Hotspot name:");
    ssidName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ssidName->setAlignment(Qt::AlignVCenter);
    ssidName->setStyleSheet("QLabel {font: 16pt; color: white;}");
    ssidName->setWordWrap(true);

    pswdName = new QLabel(this);
    pswdName->setGeometry(10,130,200,50);
    pswdName->setText("Password:");
    pswdName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    pswdName->setAlignment(Qt::AlignVCenter);
    pswdName->setStyleSheet("QLabel {font: 16pt; color: white;}");
    pswdName->setWordWrap(true);

    inputSsid = new QLineEdit(this);
    inputSsid->setGeometry(200, 80, 190, 30);

    inputPswd = new QLineEdit(this);
    inputPswd->setGeometry(200, 140, 190, 30);
    inputPswd->setEchoMode(QLineEdit::Password);
    connect(inputPswd, &QLineEdit::textEdited, this, &SettingsTab::pswdChanged);

    toggleVisibility = new QPushButton(this);
    toggleVisibility->setGeometry(160, 140, 30, 30);
    toggleVisibility->setIcon(QIcon(QPixmap("imgs/open_eye.png")));
    toggleVisibility->setIconSize(QSize(28,28));
    toggleVisibility->setStyleSheet(cs2);
    connect(toggleVisibility, &QPushButton::clicked, this, &SettingsTab::toggle);

    share = new QPushButton(this);
    share->setGeometry(120, 140, 30, 30);
    share->setIcon(QIcon(QPixmap("imgs/qr.png")));
    share->setIconSize(QSize(28,28));
    share->setStyleSheet(cs2);
    connect(share, &QPushButton::clicked, this, &SettingsTab::displayQrCodeClick);


    //hotspot values

    hotspotInfo.type = "WPA";

    loadAllParams();

    /*
    callNextForm = new QPushButton(this);
    callNextForm->setFixedSize(WIDTH, HEIGHT);
    callNextForm->setStyleSheet(cs1);

    connect(callNextForm, &QPushButton::clicked, this, &Container::click);*/

}

bool SettingsTab::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == posButton) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton) {
                m_isDragging = true;
                m_dragPosition = me->globalPos() - this->window()->frameGeometry().topLeft();
                return true;
            }
        }
        else if (event->type() == QEvent::MouseMove) {
            if (m_isDragging) {
                QMouseEvent *me = static_cast<QMouseEvent*>(event);
                win->move(me->globalPos() - m_dragPosition);
                return true;
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            m_isDragging = false;
            settings->setValue("windowPosition", win->pos()); //save location to file
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void SettingsTab::loadAllParams(){
    //load from file and set position
    QPoint pos = settings->value("windowPosition", QPoint(1500, 50)).toPoint();

    //manage hotspot

    hotspotInfo.ssid = settings->value("hotspotSSID", "SCPHotspot").toString();
    hotspotInfo.password = settings->value("hotspotPassword", "12345678").toString();

    inputSsid->setText(hotspotInfo.ssid);
    inputPswd->setText(hotspotInfo.password);

    win->move(pos);
}

void SettingsTab::resetClicked(){

    settings->setValue("windowPosition", QPoint(1500, 50));
    //std::cout << win->pos().x() << "    " << win->pos().y() << '\n';
    win->move(1500,50);

}

void SettingsTab::toggle(){
    if(inputPswd->echoMode() == QLineEdit::Password){
        toggleVisibility->setIcon(QIcon(QPixmap("imgs/closed_eye.png")));
        toggleVisibility->setIconSize(QSize(28,28));
        inputPswd->setEchoMode(QLineEdit::Normal);
    }else{
        toggleVisibility->setIcon(QIcon(QPixmap("imgs/open_eye.png")));
        toggleVisibility->setIconSize(QSize(28,28));
        inputPswd->setEchoMode(QLineEdit::Password);
    }

}

void SettingsTab::pswdChanged(QString pswd){
    settings->setValue("hotspotPassword", pswd); //save location to file
}

void SettingsTab::ssidChanged(QString ssid){
    settings->setValue("hotspotSSID", ssid); //save location to file
}

void SettingsTab::displayQrCodeClick(){
    emit displayQrCodeSignal(hotspotInfo);
}
