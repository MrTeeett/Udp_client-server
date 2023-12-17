// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <functional>
#include <cmath>


namespace Ui {
class MainWindow;
}

class QUdpSocket;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void readDatagram();



private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
};

#endif // MAINWINDOW_H
//void connectToServer();
   /* void calculateTask(int taskNumber, double intervalStart, double intervalEnd, double precision);
    *
    double func1(double x);
    double func2(double x);
    double func3(double x);*/
    /* void on_pushButton_2_clicked();
    void slotReadyRead();
    void slotDisconnected();

    void on_pushButton_clicked();*/
