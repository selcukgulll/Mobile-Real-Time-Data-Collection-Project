// myserver.cpp

#include "myserver.h"
#include "mythread.h"

MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
}

void MyServer::startServer(const QString& ipAddress)
{
    int port = 1234;

    if(!this->listen(QHostAddress::Any,port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << " on IP address " << ipAddress << "127.0.0.1";
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    MyThread *thread = new MyThread(socketDescriptor, this);

    // connect signal/slot
    // once a thread is not needed, it will be deleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void MyServer::handleClientError(QTcpSocket::SocketError socketError)
{
    // Handle the error signal here
    qDebug() << "Client error: " << socketError;
}
