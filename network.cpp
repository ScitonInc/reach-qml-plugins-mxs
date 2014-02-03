#include <QDebug>

#include "network.h"


Network::Network(QObject *parent) : QObject(parent)
{
    m_ip_list = QNetworkInterface::allAddresses();
}

Network::~Network()
{
    
}

QString Network::getIp()
{
    foreach(QHostAddress ip, m_ip_list) {
        qDebug() << ip.toString();
        if(ip != ip.LocalHost) {
            qDebug() << "returning: " << ip.toString();
            return ip.toString();
        }
    }

    qDebug() << "returning: localhost";
    return "127.0.0.1";
}
