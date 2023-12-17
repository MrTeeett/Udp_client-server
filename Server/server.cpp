// server.cpp
#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent), udpSocket(new QUdpSocket(this))
{
    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::readDatagram);
    //connect(udpSocket, &QUdpSocket::readyRead, this, &Server::sendMessage);
}

void Server::startServer()
{
    if (udpSocket->bind(QHostAddress::Any, 12345)) // Указываем порт, который будет прослушиваться сервером
    {
        qDebug() << "Server started on port 12345";
    }
    else
    {
        qWarning() << "Failed to start server";
    }
}

void Server::readDatagram()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        // Processing received data from the client
        QString receivedMessage = QString::fromUtf8(datagram);
        qDebug() << "Received message from client:" << receivedMessage;

        // Sending a response back to the client
        QString responseMessage = "Server received your message";
        QByteArray responseData = responseMessage.toUtf8();
        udpSocket->writeDatagram(responseData, sender, senderPort);
    }
}

void Server::sendMessage()
{
    QByteArray message("1");
    udpSocket->writeDatagram(message.data(), message.size(), QHostAddress::Broadcast, 12345);
    //qDebug() << "Sent message to client: " << message;
}
