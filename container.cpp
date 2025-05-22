#include "container.h"
#include <QTimer>
#include <iostream>

Container::Container(QWidget *parent)
    : QWidget{parent}
{
    //init
    setFixedWidth(WIDTH);
    setFixedHeight(HEIGHT);
    setAutoFillBackground(true);

    QPalette background = QPalette();
    background.setColor(QPalette::Window, defaultColor);
    setPalette(background);

    callNextForm = new QPushButton(this);
    callNextForm->setFixedSize(WIDTH, HEIGHT);
    callNextForm->setStyleSheet(cs1);

    connect(callNextForm, &QPushButton::clicked, this, &Container::click);

}

Container::~Container(){

}

void Container::click(){
}

QString PreviewContainer::strengthFromInt(int strength){
    if (strength >= 80) {
        return "\uE804";  // Відмінний сигнал
    } else if (strength >= 60) {
        return "\uE803";  // Добрий сигнал
    } else if (strength >= 40) {
        return "\uE802";  // Середній сигнал
    } else if (strength >= 20) {
        return "\uE801";  // Слабкий сигнал
    } else {
        return "\uE800";  // Дуже слабкий сигнал
    }
}

void PreviewContainer::remakeStatus(){
    status->reset();
    statusSlideIn->stop();
    statusSlideIn->setTargetObject(status);
}

PreviewContainer::PreviewContainer(info wifiInfo, QWidget *parent)
    : Container{parent}
{
    myInfo = wifiInfo;

    QString displayName = wifiInfo.ssid;
    if(wifiInfo.frequency > 5000)
        displayName = displayName + + " (" + QString::number(wifiInfo.frequency/1000.0, 'f', 1) + " GHz)";

    name = new QLabel(this);
    name->setGeometry(10,0,320,50);
    name->setText(displayName);
    name->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    name->setAlignment(Qt::AlignVCenter);
    name->setStyleSheet("QLabel {font: 16pt; color: white;}");
    name->setWordWrap(true);

    QFontDatabase::addApplicationFont("scp-wifi-strength-font.ttf");
    QFont scpFont("scp-wifi-strength-font", 24, QFont::Normal);

    strength = new QLabel(this);
    strength->setText(strengthFromInt(wifiInfo.strength));
    strength->setGeometry(330,0,50,50);
    strength->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    strength->setFont(scpFont);
    strength->setStyleSheet("QLabel {color: white;}");

    QPixmap img;
    img.load("imgs/saved.png");
    img = img.scaledToHeight(10);
    savedImg = new QLabel(this);
    savedImg->setGeometry(365,30,10,15);
    savedImg->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    savedImg->setPixmap(img);

    nextForm = nullptr;
    slideIn = nullptr;
    stateChanged(wifiInfo);

    status = new StatusContainer(this);
    connect(status, &StatusContainer::Ended, this, &PreviewContainer::remakeStatus);
    status->move(390,0);

    slideIn = new QPropertyAnimation(nextForm, "pos", this);
    slideIn->setDuration(70);
    slideIn->setStartValue(QPoint(390, 0));
    slideIn->setEndValue(QPoint(0, 0));

    statusSlideIn = new QPropertyAnimation(status, "pos", this);
    statusSlideIn->setDuration(70);
    statusSlideIn->setStartValue(QPoint(390, 0));
    statusSlideIn->setEndValue(QPoint(0, 0));

    status->raise();

}

void PreviewContainer::stateChanged(info wifiInfo){
    myInfo = wifiInfo;

    if(nextForm != nullptr)
        delete nextForm;

    //nextForm->hide();

    if(!wifiInfo.st.contains(saved))
        savedImg->setVisible(false);
    else
        savedImg->setVisible(true);

    if(wifiInfo.st.contains(connected)){
        //std::cout << "connected changed" << wifiInfo.ssid.toStdString() << '\n';
        nextForm = new ConnectedContainer(wifiInfo, this);
        connect(static_cast<ConnectedContainer*>(nextForm),
                &ConnectedContainer::disconnectSignal,
                this, &PreviewContainer::disconnect);
    } else
    if(wifiInfo.st.contains(saved) && wifiInfo.st.contains(disconnected)){
        //std::cout << "saved disconnected changed" << wifiInfo.ssid.toStdString() << '\n';
        nextForm = new SavedDisconnectedContainer(this);
        connect(static_cast<SavedDisconnectedContainer*>(nextForm),
                &SavedDisconnectedContainer::tryConnectSignal,
                this, &PreviewContainer::tryConnect);

        connect(static_cast<SavedDisconnectedContainer*>(nextForm),
                &SavedDisconnectedContainer::tryDelete,
                this, &PreviewContainer::deleteConnection);
    } else
    if(!wifiInfo.st.contains(saved) && wifiInfo.st.contains(disconnected)){
        //std::cout << "disconnected changed" << wifiInfo.ssid.toStdString() << '\n';
        nextForm = new PasswordContainer(this);
        connect(static_cast<PasswordContainer*>(nextForm), &PasswordContainer::tryConnectSignal,
                this, &PreviewContainer::tryConnect);
    }

    nextForm->move(390,0);
    nextForm->show();
    if(slideIn != nullptr){
        slideIn->setTargetObject(nextForm);
        slideIn->setCurrentTime(0);
    }
}

PreviewContainer::~PreviewContainer(){

}

void PreviewContainer::showStatus(QString error){
    status->displayStatus(error);
    nextForm->click();
}

void PreviewContainer::click(){
    //std::cout << "clicked\n";
    slideIn->start();
}

info PreviewContainer::getInfo(){
    return myInfo;
}

void PreviewContainer::tryConnect(QString pswd){
    //std::cout << pswd.toStdString() << '\n';
    statusSlideIn->start();
    myInfo.password = pswd;
    emit tryConnectSignal(myInfo, this);
}

void PreviewContainer::disconnect(){
    statusSlideIn->start();
    emit disconnectSignal(myInfo, this);
}

void PreviewContainer::deleteConnection(){
    statusSlideIn->start();
    emit deleteConnectionSignal(myInfo, this);
}

ConnectedContainer::ConnectedContainer(info wifiInfo, QWidget *parent)
    : Container{parent}
{
    myInfo = wifiInfo;

    ip = new QLabel(this);
    ip->setGeometry(10,0,320,50);
    ip->setText(myInfo.ip);
    ip->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ip->setAlignment(Qt::AlignVCenter);
    ip->setStyleSheet("QLabel {font: 16pt; color: white;}");

    share = new QPushButton(this);
    share->setGeometry(290, 10, 30, 30);
    share->setIcon(QIcon(QPixmap("imgs/qr.png")));
    share->setIconSize(QSize(28,28));
    share->setStyleSheet(cs2);

    disconnect = new QPushButton(this);
    disconnect->setGeometry(340, 10, 30, 30);
    disconnect->setIcon(QIcon(QPixmap("imgs/exit.png")));
    disconnect->setIconSize(QSize(20,20));
    disconnect->setStyleSheet(cs2);
    connect(disconnect, &QPushButton::clicked, this, &ConnectedContainer::disconnectClick);

    slideOut->setDuration(70);
    slideOut->setStartValue(QPoint(0, 0));
    slideOut->setEndValue(QPoint(390, 0));

}

void ConnectedContainer::disconnectClick(){
    emit disconnectSignal();
}

ConnectedContainer::~ConnectedContainer(){

}

void ConnectedContainer::click(){
    slideOut->start();
}

PasswordContainer::PasswordContainer(QWidget *parent)
    : Container{parent}
{
    input = new QLineEdit(this);
    input->setGeometry(10, 10, 270, 30);
    input->setEchoMode(QLineEdit::Password);

    toggleVisibility = new QPushButton(this);
    toggleVisibility->setGeometry(290, 10, 30, 30);
    toggleVisibility->setIcon(QIcon(QPixmap("imgs/open_eye.png")));
    toggleVisibility->setIconSize(QSize(28,28));
    toggleVisibility->setStyleSheet(cs2);

    tryConnect = new QPushButton(this);
    tryConnect->setGeometry(340, 10, 30, 30);
    tryConnect->setIcon(QIcon(QPixmap("imgs/enter.png")));
    tryConnect->setIconSize(QSize(20,20));
    tryConnect->setStyleSheet(cs2);

    slideOut->setDuration(70);
    slideOut->setStartValue(QPoint(0, 0));
    slideOut->setEndValue(QPoint(390, 0));

    connect(toggleVisibility, &QPushButton::clicked, this, &PasswordContainer::toggle);
    connect(tryConnect, &QPushButton::clicked, this, &PasswordContainer::connectToWifi);

}

PasswordContainer::~PasswordContainer(){

}

void PasswordContainer::click(){
    slideOut->start();
}

void PasswordContainer::toggle(){
    if(input->echoMode() == QLineEdit::Password){
        toggleVisibility->setIcon(QIcon(QPixmap("imgs/closed_eye.png")));
        toggleVisibility->setIconSize(QSize(28,28));

        input->setEchoMode(QLineEdit::Normal);
    }else{
        toggleVisibility->setIcon(QIcon(QPixmap("imgs/open_eye.png")));
        toggleVisibility->setIconSize(QSize(28,28));

        input->setEchoMode(QLineEdit::Password);
    }

}

void PasswordContainer::connectToWifi(){
    emit tryConnectSignal(input->text());
}

SavedDisconnectedContainer::SavedDisconnectedContainer(QWidget *parent)
    : Container{parent}
{

    disconnected = new QLabel(this);
    disconnected->setGeometry(10,0,320,50);
    disconnected->setText("disconnected, available");
    disconnected->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    disconnected->setAlignment(Qt::AlignVCenter);
    disconnected->setStyleSheet("QLabel {font: 16pt; color: white;}");

    forget = new QPushButton(this);
    forget->setGeometry(240, 10, 30, 30);
    forget->setIcon(QIcon(QPixmap("imgs/trash.png")));
    forget->setIconSize(QSize(20,20));
    forget->setStyleSheet(cs2);
    connect(forget, &QPushButton::clicked, this, &SavedDisconnectedContainer::deleteWifi);

    share = new QPushButton(this);
    share->setGeometry(290, 10, 30, 30);
    share->setIcon(QIcon(QPixmap("imgs/qr.png")));
    share->setIconSize(QSize(28,28));
    share->setStyleSheet(cs2);

    tryConnect = new QPushButton(this);
    tryConnect->setGeometry(340, 10, 30, 30);
    tryConnect->setIcon(QIcon(QPixmap("imgs/enter.png")));
    tryConnect->setIconSize(QSize(20,20));
    tryConnect->setStyleSheet(cs2);
    connect(tryConnect, &QPushButton::clicked, this, &SavedDisconnectedContainer::connectToWifi);

    slideOut->setDuration(70);
    slideOut->setStartValue(QPoint(0, 0));
    slideOut->setEndValue(QPoint(390, 0));

}

void SavedDisconnectedContainer::connectToWifi(){
    emit tryConnectSignal("");
}

void SavedDisconnectedContainer::deleteWifi(){
    emit tryDelete();
}

SavedDisconnectedContainer::~SavedDisconnectedContainer(){

}

void SavedDisconnectedContainer::click(){
    slideOut->start();
}

StatusContainer::StatusContainer(QWidget *parent, QString text)
    : Container{parent}
{
    callNextForm->setHidden(true);

    //TEXT
    status = new QLabel(this);
    status->setGeometry(50,0,370,50);
    status->setText(text);
    status->setAlignment(Qt::AlignVCenter);
    status->setStyleSheet("QLabel {font: 16pt; color: white;}");
    status->move(QPoint(-370, 0));

    cover = new QLabel(this);
    cover->setGeometry(0,0,205,50);
    cover->setStyleSheet("QLabel {background-color: #333333;}");

    //ANIM GIF
    statusAnim = new QLabel(this);
    statusAnim->setGeometry(165,5,40,40);
    statusAnim->setAlignment(Qt::AlignCenter);
    statusAnim->setStyleSheet("QLabel {font: 16pt; color: white;}");
    movie = new QMovie("imgs/loading.gif");
    movie->setScaledSize(QSize(40,40));
    statusAnim->setMovie(movie);
    movie->start();

    statusAnim->raise();
    status->lower();

    slideOut->setDuration(100);
    slideOut->setStartValue(QPoint(0, 0));
    slideOut->setEndValue(QPoint(390, 0));

    revealAnim = new QPropertyAnimation(statusAnim, "pos", this);
    revealText = new QPropertyAnimation(cover, "geometry", this);
    revealText2 = new QPropertyAnimation(status, "pos", this);

    revealAnim->setDuration(500); //position of gif
    revealAnim->setStartValue(QPoint(165, 5));
    revealAnim->setEndValue(QPoint(5, 5));

    revealText->setDuration(500); //size of cover
    revealText->setStartValue(QRect(0, 0, 205, 50));
    revealText->setEndValue(QRect(0, 0, 45, 50));

    revealText2->setDuration(500); //position of text
    revealText2->setStartValue(QPoint(-370, 0)); //50,0,370,50
    revealText2->setEndValue(QPoint(50, 0));

    //QTimer::singleShot(1500, this, &StatusContainer::stopGifAnim);
    connect(movie, &QMovie::frameChanged, this, &StatusContainer::count);
    connect(slideOut, &QPropertyAnimation::finished, this, &StatusContainer::EmitEnd);
}

void StatusContainer::count(){
    if(movie->currentFrameNumber() == 0 && stopping)
    {
        movie->stop();
        if (movie->state() == QMovie::NotRunning)
        {
            emit movie->finished();
            QTimer::singleShot(300, this, &StatusContainer::reveal);;
        }
    }
}

void StatusContainer::reveal(){
    revealAnim->start();
    revealText->start();
    revealText2->start();
    QTimer::singleShot(2000, this, &StatusContainer::close);
}

void StatusContainer::displayStatus(QString status){
    //setText(status);
    this->status->setText(status);
    //stopping = true;
    QTimer::singleShot(1500, this, [=](){stopping = true;}); // because too fast :)
}

StatusContainer::~StatusContainer(){

}

void StatusContainer::close(){
    slideOut->start();
}

void StatusContainer::EmitEnd(){
    emit Ended();
}

void StatusContainer::reset(){
    stopping = false;
    revealAnim->setCurrentTime(0);
    revealText->setCurrentTime(0);
    revealText2->setCurrentTime(0);
    slideOut->setCurrentTime(0);

    revealText->targetObject()->setProperty("geometry", revealText->startValue());
    revealText2->targetObject()->setProperty("pos", revealText2->startValue());
    revealAnim->targetObject()->setProperty("pos", revealAnim->startValue());

    movie->start();
}
