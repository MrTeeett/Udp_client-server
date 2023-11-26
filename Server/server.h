#ifndef SERVER_H
#define SERVER_H

#include <QUdpSocket>
#include <QObject>
#include <QDebug>
#include <QVector>
#include <QRandomGenerator>
#include <QDataStream>

class Server : public QObject
{
    Q_OBJECT

public:
    Server();
    void start();

private:
    QUdpSocket *udpSocket;
    int token;
    QVector<QHostAddress> clientAddresses;
    QVector<quint16> clientPorts;
    QByteArray data;

    void sendTaskToClient(const QHostAddress &clientAddress, quint16 clientPort);

private slots:
    void readPendingDatagrams();
};

#endif // SERVER_H

