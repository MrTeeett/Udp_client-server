// server.h

#ifndef SERVER_H
#define SERVER_H
#define PI 3.141593
#include <QUdpSocket>
#include <QVector>
#include <QDebug>
#include <QDataStream>


class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

public slots:
    void startServer();
    void sendMessage();

private slots:
    void readDatagram();

private:
    QUdpSocket *udpSocket;
};

#endif // SERVER_H
/*quint16 clientPort;
int taskNumber;
double totalResult;
int receivedResults;*/
/*vvoid slotReadyRead();
void sendDataToClients();
void incomingConnection(qintptr socketDescriptor);
void processResult(double result);
void sendResultToClients();*/
