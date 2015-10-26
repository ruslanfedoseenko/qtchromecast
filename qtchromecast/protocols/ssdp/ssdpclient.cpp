#include "ssdpclient.h"
#include <QUdpSocket>
SSDPClient::SSDPClient(QString serverName, QObject *parent)
    : QObject(parent)
    , m_serverName(serverName)
    , m_bIsRunning(false)
    , m_pUdpListener(NULL)
{

}


SSDPClient::~SSDPClient()
{
    if (isRunning())
    {
        stop();
        delete m_pUdpListener;
    }
}

void SSDPClient::start()
{
    if (!isRunning()) {
           m_pUdpListener = new UdpMulticastListener("239.255.255.250", 1900,
                                                    m_serverName);
           connect(m_pUdpListener, SIGNAL(readyRead()), this, SLOT(datagramReceived()));
           m_pUdpListener->start();

           QByteArray message("M-SEARCH * HTTP/1.1\r\n"        \
                                  "HOST: 239.255.255.250:1900\r\n" \
                                  "MAN: \"ssdp:discover\"\r\n" \
                                  "MX: 1\r\n" \
                                  "ST: urn:dial-multiscreen-org:service:dial:1\r\n" \
                                  "\r\n");

              bool ok = m_pUdpListener->writeDatagram(message.constData(), message.size(),
                                           QHostAddress("239.255.255.250"), 1900);
               if (!ok) {
                   qDebug() << "Sending failed" << m_pUdpListener->errorString();

                   return;
               }
           m_bIsRunning = true;
       } else {
           qDebug() << "Brisa SSDP Client: Already running!";
       }
}

void SSDPClient::stop()
{
    if (isRunning()) {
      m_pUdpListener->disconnectFromHost();;
      m_bIsRunning = false;
    } else {
        qDebug() << "Brisa SSDP Client: Already stopped!";
    }
}

bool SSDPClient::isRunning() const
{
    return m_bIsRunning;
}


void SSDPClient::datagramReceived() {
    while (m_pUdpListener->hasPendingDatagrams()) {
        QByteArray *datagram = new QByteArray();

        datagram->resize(m_pUdpListener->pendingDatagramSize());
        m_pUdpListener->readDatagram(datagram->data(), datagram->size());

        QString temp(datagram->data());
        QHttpRequestHeader *parser = new QHttpRequestHeader(temp);

        notifyReceived(parser);

        delete datagram;
        delete parser;
    }

}

void SSDPClient::notifyReceived(QHttpRequestHeader *datagram) {
    if (!datagram->hasKey("nts"))
        return;

    if (datagram->value("nts") == "ssdp:alive") {
        emit newDeviceEvent(datagram->value("usn"),
                            datagram->value("location"), datagram->value("nt"),
                            datagram->value("ext"), datagram->value("server"),
                            datagram->value("cacheControl"));
        qDebug() << "Brisa SSDP Client: Received alive from " <<
                datagram->value("usn") << "";

    } else if (datagram->value("nts") == "ssdp:byebye") {
        emit removedDeviceEvent(datagram->value("usn"));
        qDebug() << "Brisa SSDP Client: Received byebye from " <<
                datagram->value("usn") << "";

    } else {
        qDebug() << "Brisa SSDP Client: Received unknown subtype: " <<
                datagram->value("nts") << "";
    }
}
