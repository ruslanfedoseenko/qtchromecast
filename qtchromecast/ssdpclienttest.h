#ifndef SSDPCLIENTTEST_H
#define SSDPCLIENTTEST_H

#include <QObject>
#include "ssdpclient.h"

class SsdpClientTest : public QObject
{
    Q_OBJECT
public:
    SsdpClientTest();

signals:

public slots:
    void onSsdpDeviceFound(const QString &usn, const QString &location,
                      const QString &st, const QString &ext, const QString &server,
                      const QString &cacheControl);
private:
    SSDPClient* m_pClient;
};

#endif // SSDPCLIENTTEST_H
