// main.cpp

#include <QCoreApplication>
#include "myserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Make a server and starts it
    MyServer server;
    server.startServer("192.168.1.1");


    return a.exec();
}

