// mythread.h

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QWidget>
#include <QMessageBox>
#include <QApplication>
#include <iostream>

using namespace std;

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(qintptr ID, QObject *parent = 0);

    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void sentenceReceived(const QString &sentence); // Declare the sentenceReceived signal
    void queryResults(const QString& results); // Declare queryResults as a signal
    void queryError(const QString &errorMessage);
    void databaseError(const QString &error); // Signal for database error

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QByteArray buffer; // Add a QByteArray member variable to store incoming data
};

#endif // MYTHREAD_H
