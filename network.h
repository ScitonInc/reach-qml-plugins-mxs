#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QNetworkInterface>

class Network : public QObject
{
    Q_OBJECT

public:
    explicit Network(QObject *parent = 0);
    ~Network();

    Q_INVOKABLE QString getIp(void);

private:
    QList<QHostAddress> m_ip_list;
};

#endif
