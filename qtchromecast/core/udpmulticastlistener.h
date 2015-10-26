#ifndef UDPMULTICASTLISTENER_H
#define UDPMULTICASTLISTENER_H

#include "qtchromecast_global.h"
#include <QUdpSocket>

class QTCHROMECASTSHARED_EXPORT UdpMulticastListener : public QUdpSocket
{
    Q_OBJECT
public:

    /*!
     * Constructor.
     *
     * \param address ip address that will be bind to the socket.
     * \param port port that will be bind to the socket.
     * \param objectName name of the object or class that will be using this
     * one. Used on warning messages.
     * \param parent parent.
     */
    UdpMulticastListener(QString address, quint32 port, QString objectName,
                     QObject *parent = 0);

    /*!
     * Destructor
     */
    virtual ~UdpMulticastListener();

    /*!
     * Starts listening to the address and port passed on constructor.
     */
    void start();

private:

    /*!
     * \property objectName
     *
     * Object or class name that will be used on debug messages.
     */
    QString objectName;

    /*!
     * \property address
     *
     * IP address that will be bind to the socket.
     */
    QString address;

    /*!
     * \property port
     *
     * Port that will be bind to the socket.
     */
    quint32 port;

};


#endif // UDPMULTICASTLISTENER_H
