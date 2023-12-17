#include <QApplication>
#include "udpclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UdpClient c;
    c.show();
    return a.exec();
}
