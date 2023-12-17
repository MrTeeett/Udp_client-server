#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataStream>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    udpSocket(new QUdpSocket(this))
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readDatagram);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString ip = ui->lineEdit->text();
    qint16 port = ui->lineEdit_2->text().toInt();

    if (ip.isEmpty() || port <= 0)
    {
        QMessageBox::critical(this, "Error", "Invalid IP or port");
        return;
    }

    udpSocket->connectToHost(QHostAddress::Broadcast, port);

    // Отправляем сообщение о подключении на сервер
    QString message = "Connected to the server";
    QByteArray data = message.toUtf8();
    qDebug() << "Sending data to server:" << data;
    udpSocket->writeDatagram(data, QHostAddress::Broadcast, port);
}

void MainWindow::readDatagram()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        // Обработка полученных данных от сервера
        QString receivedMessage = QString::fromUtf8(datagram);
        qDebug() << "Received message from server:" << receivedMessage;
        ui->textBrowser->append(receivedMessage);

        // Добавление новой функциональности
        if (receivedMessage == "1") {
            // Ваш код здесь
            qDebug() << "Received '1' from server";
        }
    }
}

/*void MainWindow::on_pushButton_2_clicked()
{
    // Send a request to the server for task calculation
    QByteArray requestData;
    QDataStream requestStream(&requestData, QIODevice::WriteOnly);
    requestStream.setVersion(QDataStream::Qt_5_0);
    requestStream << static_cast<quint8>(1); // Request type for task calculation

    // Broadcast the request to all servers in the network
    socket->writeDatagram(requestData, QHostAddress::Broadcast, 2323);

    ui->textBrowser->append("Request sent");
}

void MainWindow::slotReadyRead()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        QDataStream in(datagram);
        in.setVersion(QDataStream::Qt_5_0);

        if (in.status() == QDataStream::Ok) {
            quint8 responseType;
            in >> responseType;

            if (responseType == 0) {
                // Handle data response from the server
                int taskNumber;
                double intervalStart, intervalEnd, precision;
                in >> taskNumber >> intervalStart >> intervalEnd >> precision;
                calculateTask(taskNumber, intervalStart, intervalEnd, precision);
            } else if (responseType == 2) {
                // Handle the response with the total result from the server
                double totalResult;
                in >> totalResult;
                ui->textBrowser->append("Total result from the server: " + QString::number(totalResult));
            } else {
                ui->textBrowser->append("Unexpected response type from the server.");
            }
        } else {
            qDebug() << "DataStream error";
            ui->textBrowser->append("DataStream error");
        }
    }
}



void MainWindow::calculateTask(int taskNumber, double intervalStart, double intervalEnd, double precision)
{
    // Perform calculations based on taskNumber
    double result = 0;
    if (taskNumber == 1) {
        for (double i = intervalStart; i <= intervalEnd; i += precision) {
            double y = func1(i);
            if (y > 0) {
                result += y * precision;
            }
        }
    } else if (taskNumber == 2) {
        for (double i = intervalStart; i <= intervalEnd; i += precision) {
            double y = func2(i);
            if (y > 0) {
                result += y * precision;
            }
        }
    } else if (taskNumber == 3) {
        while ((intervalEnd - intervalStart) / 2 < precision) {
            double mid = (intervalEnd + intervalStart) / 2;
            if (func3(mid) == 0) {
                result = mid;
            } else if (func3(mid) * func3(intervalStart) > 0) {
                intervalEnd = mid;
            } else {
                intervalStart = mid;
            }
        }
        result = (intervalEnd + intervalStart) / 2;
    } else {
        ui->textBrowser->append("Invalid task number.");
        return;
    }

    // Send the result back to the server
    QByteArray responseData;
    QDataStream responseStream(&responseData, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);
    responseStream << static_cast<quint8>(2) << result; // Response type for task result

    // Broadcast the result to all servers in the network
    socket->writeDatagram(responseData, QHostAddress::Broadcast, 2323);
}

double MainWindow::func1(double x) {
    return sin(log(x));
}

double MainWindow::func2(double x) {
    return 3 / (cos(2 * x) - 0.1);
}

double MainWindow::func3(double x) {
    return 2 / (sin(x) + 2);
}

// ...

void MainWindow::on_pushButton_clicked()
{
    // Get the IP address and port from the QLineEdit widgets
    QString ipAddress = ui->lineEdit->text();
    quint16 port = ui->lineEdit_2->text().toUShort();

    // Validate the IP address
    QHostAddress serverAddress(ipAddress);
    if (serverAddress.isNull() || serverAddress.protocol() == QAbstractSocket::UnknownNetworkLayerProtocol) {
        ui->textBrowser->append("Invalid IP address. Please enter a valid IP address.");
        return;
    }

    // Connect to the server
    socket->connectToHost(serverAddress, port);

    if (socket->waitForConnected()) {
        ui->textBrowser->append("Connected to the server.");
    } else {
        ui->textBrowser->append("Failed to connect to the server. Check the IP and port.");
        qDebug() << "Error: " << socket->errorString();
    }
}

// ...
*/



