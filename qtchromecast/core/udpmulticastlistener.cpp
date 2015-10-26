#include "udpmulticastlistener.h"

#if defined(Q_OS_UNIX) || defined(Q_OS_ANDROID)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <cstring>
#define MAX_LEN  1024   /* maximum receive string size */
#define LOCAL_HOST "127.0.0.1"
#include <QNetworkInterface>
UdpMulticastListener::UdpMulticastListener(QString address, quint32 port,
                                   QString objectName, QObject *parent) :
        QUdpSocket(parent)
{
    this->port = port;
    this->objectName = objectName;
    this->address = address;
}

UdpMulticastListener::~UdpMulticastListener()
{
}


QBool isLoopbackIPv4Address(QString address) {
    return QBool(!address.compare("127.0.0.1"));
}

QBool isLoopbackIPv6Address(QString address) {
    return QBool(!address.compare("0:0:0:0:0:0:0:1"));
}

QBool isPromiscuousIPv4Address(QString address) {
    return QBool(!address.compare("0.0.0.0"));
}

QBool isPromiscuousIPv6Address(QString address) {
    return QBool(!address.compare("0:0:0:0:0:0:0:0") | !address.compare("::"));
}

QString getValidIP() {
/*#if defined(Q_OS_UNIX) || defined(Q_OS_ANDROID)
    BrisaConfigurationManager *config = BrisaConfigurationManager::getInstance();
    QString interfaceName = config->getParameter("network", "interface");
    QString ip = getIp(interfaceName);
    if (ip.isEmpty()) {
        ip = config->getParameter("network", "ip");
    }
    if (ip.isEmpty()) {
        ip = QHostAddress(QHostAddress::Any).toString();
    }
    return ip;
#else*/
    foreach(QHostAddress addressEntry , QNetworkInterface::allAddresses() )
        {
            QString address = addressEntry.toString();
            if (!(isLoopbackIPv4Address(address)) && !(isLoopbackIPv6Address(
                    address)) && !(isPromiscuousIPv4Address(address))
                    && !(isPromiscuousIPv6Address(address))) {
                return address;
            }
        }
    qDebug()
            << "Couldn't acquire a non loopback IP  address,returning 127.0.0.1.";
    return "127.0.0.1";
//#endif
}

void UdpMulticastListener::start()
{

#if defined(Q_WS_X11) || defined (Q_OS_ANDROID)
    if (!this->bind(QHostAddress(this->address), this->port, QUdpSocket::ShareAddress |
                    QUdpSocket::ReuseAddressHint)) {
            qWarning() << this->objectName << ": failure to bind interface.";
    }
#endif

    int fd;
    fd = this->socketDescriptor();
    struct ip_mreq mreq;
    memset(&mreq, 0, sizeof(ip_mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(this->address.toUtf8());
    bool boolean = true;

    QString ip = getValidIP();
    if (ip == LOCAL_HOST)
    {
        mreq.imr_interface.s_addr = inet_addr(ip.toUtf8());
    }
    else
    {
        mreq.imr_interface.s_addr = htons(INADDR_ANY);
    }
#if !defined(Q_WS_X11) && !defined(Q_OS_ANDROID)
    qWarning() << "windows procedure is running...";
       WSADATA wsaData;              /* Windows socket DLL structure */
       struct sockaddr_in mc_addr;   /* socket address structure */

           /* Load Winsock 2.0 DLL */
       if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
             fprintf(stderr, "WSAStartup() failed");
             exit(1);
       }

       /* create socket to join multicast group on */
       if ((fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
         qWarning() << "socket() failed";
         exit(1);
       }

       /* set reuse port to on to allow multiple binds per host */
       if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&boolean,
            sizeof(boolean))) < 0) {
         qWarning() << "setsockopt() failed";
             exit(1);
       }

       /* construct a multicast address structure */
       memset(&mc_addr, 0, sizeof(mc_addr));
       mc_addr.sin_family      = AF_INET;
       mc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
       mc_addr.sin_port        = htons(this->port);

       /* bind to multicast address to socket */
       if ((::bind(fd, (struct sockaddr *) &mc_addr,
            sizeof(mc_addr))) < 0) {
         qWarning() << "bind() failed";
         exit(1);
       }

         /* construct an IGMP join request structure */
       mreq.imr_multiaddr.s_addr = inet_addr(address.toAscii());
       mreq.imr_interface.s_addr = htonl(INADDR_ANY);


       /* send an ADD MEMBERSHIP message via setsockopt */
       if ((setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
            (char*) &mreq, sizeof(mreq))) < 0) {
           qWarning() << this->objectName << ": could not join MULTICAST group.";

         exit(1);
       }
       this->setSocketDescriptor(fd,QUdpSocket::BoundState,QUdpSocket::ReadOnly);
#endif


#if defined(Q_WS_X11) || defined(Q_OS_ANDROID)
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   &mreq, sizeof(mreq)) < 0 ||
        setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP,
                   &boolean, sizeof (boolean)) < 0)
    {
          qWarning() << this->objectName << ": could not join MULTICAST group.";
    }
#endif
}

