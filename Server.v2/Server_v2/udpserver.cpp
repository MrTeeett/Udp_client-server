#include "udpserver.h"
#include <QHostAddress>
#include <QUdpSocket>
#include <QDebug>

UdpServer::UdpServer(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("UDP Server"));
    /* Инициализируем каждый элемент управления */
    TimerLabel = new QLabel(tr("Таймер:"),this);
    TextLineEdit = new QLineEdit(this);
    StartBtn = new QPushButton(tr("начать"),this);
    /* Устанавливаем макет */
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(TimerLabel);
    mainLayout->addWidget(TextLineEdit);
    mainLayout->addWidget(StartBtn);
    connect(StartBtn,SIGNAL(clicked()),this,SLOT(StartBtnClicked()));
    port = 5555;		// Устанавливаем параметр номера порта UDP
    isStarted = false;
    udpSocket=new QUdpSocket;// Создаем QUdpSocket
    timer = new QTimer(this);
    // Регулярно отправлять информацию о трансляции
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
}

UdpServer::~UdpServer()
{

}

void UdpServer::StartBtnClicked()
{
    if(!isStarted)
    {
        StartBtn->setText("Стоп");
        timer->start(1000);
        isStarted=true;
    }
    else
    {
        StartBtn->setText("начать");
        isStarted=false;
        timer->stop();
    }
}

void UdpServer::timeout()
{
    //QString msg = TextLineEdit->text();
    QString msg = NULL;
    if (numberFunc == 2){
        msg = "sin(x) / 3 + 0,5";
        numberFunc = 1;
    }
    else if (numberFunc == 0 or numberFunc == 1){
        msg = "cos(x) * 2 - 1,5";
        numberFunc = 2;
    }

    qDebug()<<msg.toLatin1();
    int length=0;
    if(msg=="")
    {
        return;
    }
    if((length=udpSocket->writeDatagram(msg.toLatin1(),msg.length(),QHostAddress::Broadcast,port))!=msg.length())// QHostAddress :: Broadcast отправляет на широковещательный адрес
    {
        return;
    }
    if (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QString msg = datagram.data();
        qDebug() << "Received message from client: " << msg;
        // Здесь вы можете обработать полученное сообщение
    }
}





