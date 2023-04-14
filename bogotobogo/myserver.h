// myserver.h

#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include "mythread.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    void startServer(const QString& ipAddress);
signals:

public slots:
    void handleClientError(QTcpSocket::SocketError socketError); // Slot to handle error signal

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // MYSERVER_H
