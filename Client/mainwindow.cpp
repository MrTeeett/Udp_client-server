#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QUdpSocket>

const double PI = 3.14159265358979323846;

double func(double x) {
    return cos(x) * 2 - 1.5;
}

double funcEx(double x) {
    return std::sin(x) / 3 + 0.5;
}

double derivative(double x) {
    return std::cos(x) / 3;
}

QString findExtrema() {
    QString result;
    QVector<QPair<double, double>> extremaPoints;  // Вектор для хранения координат точек экстремума

    // Диапазон поиска (задайте по своему усмотрению)
    double lowerBound = -PI;
    double upperBound = PI;

    // Шаг для поиска
    double step = 0.001;

    // Начинаем поиск точек экстремума
    double prevDerivative = derivative(lowerBound);
    for (double x = lowerBound + step; x <= upperBound; x += step) {
        double der = derivative(x);

        // Проверяем изменение знака производной
        if (prevDerivative * der < 0) {
            extremaPoints.append(qMakePair(x - step / 2.0, func(x - step / 2.0)));
        }

        prevDerivative = der;
    }

    // Записываем координаты точек экстремума в строку QString
    for (const auto& point : extremaPoints) {
        result += QString("Found extremum at x = %1, y = %2\n").arg(point.first).arg(point.second);
    }

    return result;
}

double integrate(double a, double b, int n) {
    double h = (b - a) / n;
    double result = (func(a) + func(b)) / 2.0;

    for (int i = 1; i < n; ++i) {
        double x_i = a + i * h;
        result += func(x_i);
    }

    return result * h;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QUdpSocket(this);
    connect(socket, &QUdpSocket::readyRead, this, &MainWindow::slotReadyRead);
}

void MainWindow::on_pushButton_clicked()
{
    socket->bind(QHostAddress::Any, 2323);
}

void MainWindow::slotReadyRead()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QDataStream in(datagram);
        in.setVersion(QDataStream::Qt_5_15);

        if (in.status() == QDataStream::Ok) {
            QString str;
            in >> str;
            ui->textBrowser->append(str);

            awaitingTask = true;

            automaticAnswer();
        } else {
            ui->textBrowser->append("read error");
        }
    }
}

void MainWindow::sendToServer(QString str)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << str;

    socket->writeDatagram(data, QHostAddress("127.0.0.1"), 2323);
}

void MainWindow::on_pushButton_2_clicked()
{
    sendToServer(ui->lineEdit->text());
}


void MainWindow::on_lineEdit_returnPressed()
{
    sendToServer(ui->lineEdit->text());
}

void MainWindow::automaticAnswer() {
    // Если флаг ожидания нового задания установлен
    if (awaitingTask) {
        // Считываем ответ из интерфейса пользователя (пример для QLineEdit)
        QString answer = ui->lineEdit->text();

        // Отправляем ответ на сервер
        sendToServer(answer);

        // Сбрасываем флаг ожидания нового задания
        awaitingTask = false;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    socket->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
