#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_returnPressed();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *socket;  // Change to QUdpSocket
    QByteArray data;
    QString currentTask;
    bool awaitingTask;

public slots:
    void slotReadyRead();
    void sendToServer(QString str);
    void automaticAnswer();
};

#endif // MAINWINDOW_H
