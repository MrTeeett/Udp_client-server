#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QUdpSocket>
#include <QTimer>

    class UdpServer : public QWidget
{
    Q_OBJECT

public:
    UdpServer(QWidget *parent = 0);
    ~UdpServer();
public slots:
    void StartBtnClicked();
    void timeout();
private:
    QLabel *TimerLabel;
    QLineEdit *TextLineEdit;
    QPushButton *StartBtn;
    QVBoxLayout *mainLayout;
    int port;
    bool isStarted;
    int numberFunc = 0;
    QUdpSocket *udpSocket;
    QTimer *timer;
};

#endif // UDPSERVER_H
