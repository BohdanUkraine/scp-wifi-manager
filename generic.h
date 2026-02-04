#ifndef GENERIC_H
#define GENERIC_H

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
    QString type; //security type
    int frequency;
    int strength;
    QList<state> st;
};


#endif // GENERIC_H
