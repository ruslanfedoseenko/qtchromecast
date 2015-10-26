#include "ssdpclienttest.h"
#include <QApplication>
SsdpClientTest::SsdpClientTest()
{
    m_pClient = new SSDPClient("TestSSDPClient", this);
    connect(m_pClient, SIGNAL(newDeviceEvent(QString,QString,QString,QString,QString,QString)), SLOT(onSsdpDeviceFound(QString,QString,QString,QString,QString,QString)));
    m_pClient->start();
}

void SsdpClientTest::onSsdpDeviceFound(const QString &usn, const QString &location, const QString &st, const QString &ext, const QString &server, const QString &cacheControl)
{
    qDebug() << "Found device: usn " << usn << " location " << location << " st " << st << " ext " << ext << " server " << server << " cacheControl " << cacheControl;
}

int main(int argc, char* argv[])
{
    QApplication a(argc,argv);
    SsdpClientTest cl;

    return a.exec();
}
