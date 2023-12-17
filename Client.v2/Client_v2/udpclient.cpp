#include "udpclient.h"
#include <QUdpSocket>
#include <QMessageBox>
#include <QHostAddress>
#include <cmath>

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

UdpClient::UdpClient(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("UDP Client"));		// Устанавливаем заголовок формы
    /* Инициализируем каждый элемент управления */
            ReceiveTextEdit = new QTextEdit(this);
    CloseBtn = new QPushButton(tr("Close"),this);
    /* Устанавливаем макет */
        mainLayout=new QVBoxLayout(this);
    mainLayout->addWidget(ReceiveTextEdit);
    mainLayout->addWidget(CloseBtn);
    connect(CloseBtn,SIGNAL(clicked()),this,SLOT(CloseBtnClicked()));
    port =5555;                             // Устанавливаем параметр номера порта UDP, указываем для мониторинга данных по этому порту
    udpSocket = new QUdpSocket(this);		// Создаем QUdpSocket
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    bool result=udpSocket->bind(port);// Привязать порт
    if(!result)
    {
        QMessageBox::information(this,"error","udp socket create error!");
        return;
    }
}

UdpClient::~UdpClient()
{

}

void UdpClient::CloseBtnClicked()
{
    close();
}

void UdpClient::dataReceived()
{
    QString res = NULL;
    while(udpSocket->hasPendingDatagrams())// Есть дейтаграмма для чтения
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
        QString msg=datagram.data();
        ReceiveTextEdit->append(msg);

        if (msg == "cos(x) * 2 - 1,5"){
            double totalArea = integrate(-100, 100, 1000000);
            int precision = 6;  // 10^-6
            res = QString::number(totalArea, 'f', precision);
            ReceiveTextEdit->append(res);
        }
        else if (msg == "sin(x) / 3 + 0,5"){
            res = findExtrema();
            ReceiveTextEdit->append(res);
        }

        // Отправляем сообщение "Я получил сообщение" серверу
        udpSocket->writeDatagram(res.toLatin1(), sender, senderPort);
    }
}
