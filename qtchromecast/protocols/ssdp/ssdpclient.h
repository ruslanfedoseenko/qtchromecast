#ifndef SSDPCLIENT_H
#define SSDPCLIENT_H

#include <QObject>
#include <QHttpRequestHeader>
#include "qtchromecast_global.h"
#include "udpmulticastlistener.h"
class QUdpSocket;
class QTCHROMECASTSHARED_EXPORT SSDPClient : public QObject
{
    Q_OBJECT
public:
    explicit SSDPClient(QString serverName, QObject *parent = 0);
    virtual ~SSDPClient();

public slots:
    /*!
     *
     *  Connects to the MultiCast group and starts the client.
     *
     *  \sa isRunning(), stop()
     */
    void start();

    /*!
     *  Stops the client.
     *
     *  \sa isRunning(), start()
     */
    void stop();

    /*!
     *  Checks if the client is running
     *
     *  \return true if is running
     */
    bool isRunning() const;

signals:
    /*!
     *  \fn void SSDPClient::newDeviceEvent(const QString &usn, const QString &location,
     *                                           const QString &st, const QString &ext,
     *                                           const QString &server, const QString &cacheControl)
     *
     *  This signal is emitted when the client receives a "ssdp:alive" message from a device joining
     *  the network
     *
     *  \sa removedDeviceEvent()
     */
    void newDeviceEvent(const QString &usn, const QString &location,
            const QString &st, const QString &ext, const QString &server,
            const QString &cacheControl);

    /*!
     *  \fn void BrisaSSDPClient::removedDeviceEvent(const QString &usn)
     *
     *  This signal is emitted when the client receives a "ssdp:byebye" message from a device leaving the
     *  network
     *
     *  \sa newDeviceEvent()
     */
    void removedDeviceEvent(const QString &usn);
private slots:
    /*!
     *  \internal
     *  Receives UDP datagrams from a QUdpSocket.
     */
    void datagramReceived();

    /*!
     *  \internal
     *  Parses the UDP datagram received from "datagramReceived()".
     *
     *  \param datagram datagram
     */
    void notifyReceived(QHttpRequestHeader *datagram);
private:
    bool m_bIsRunning;
    QString m_serverName;

    UdpMulticastListener* m_pUdpListener;
};

#endif // SSDPCLIENT_H
