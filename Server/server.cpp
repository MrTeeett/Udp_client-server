#include "server.h"

Server::Server() : udpSocket(new QUdpSocket(this)), token(0)
{
    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::readPendingDatagrams);
}

void Server::start()
{
    if (udpSocket->bind(QHostAddress::Any, 2323)) {
        qDebug() << "Server started";
    } else {
        qDebug() << "Error starting server";
    }
}

void Server::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        data.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
        QHostAddress senderAddress;
        quint16 senderPort;

        udpSocket->readDatagram(data.data(), data.size(), &senderAddress, &senderPort);

        qDebug() << "Received datagram from " << senderAddress << ":" << senderPort;

        // Check if the client is already registered
        if (!clientAddresses.contains(senderAddress) || !clientPorts.contains(senderPort)) {
            clientAddresses.append(senderAddress);
            clientPorts.append(senderPort);
            qDebug() << "New client registered: " << senderAddress << ":" << senderPort;
        }

        // Process the datagram data
        QDataStream in(data);
        in.setVersion(QDataStream::Qt_5_15);
        QString str;
        in >> str;
        qDebug() << "Received answer from client: " << str;

        // Process the answer from the client as needed

        // Send a new task to the client
        sendTaskToClient(senderAddress, senderPort);
    }
}

void Server::sendTaskToClient(const QHostAddress &clientAddress, quint16 clientPort)
{
    // Generate a math task
    int operand1 = QRandomGenerator::global()->bounded(2);
    QString task;
    if ((operand1 < 1) or (token != 1)) {
        task = QString("cos(x) * 2 - 1,5");
        token = 1;
    } else if (token != 2) {
        task = QString("sin(x) / 3 + 0,5");
        token = 2;
    } else {
        task = QString("NONE");
    }

    QByteArray sendData;
    QDataStream out(&sendData, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << task;

    udpSocket->writeDatagram(sendData, clientAddress, clientPort);
}
