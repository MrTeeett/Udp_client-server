#include "udpserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    UdpServer s;
    s.show();
    return app.exec();
}
