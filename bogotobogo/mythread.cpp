// mythread.cpp

#include "mythread.h"

MyThread::MyThread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void MyThread::run()
{
    // thread starts here
    qDebug() << " Thread started";

    socket = new QTcpSocket();

    // set the ID
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // We'll have multiple clients, we want to know which is which
    qDebug() << socketDescriptor << " Client connected";

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    exec();
}

void MyThread::readyRead()
{
    // Get the information
    QByteArray data = socket->readAll();

    // Accumulate the data in a buffer
    buffer += QString(data);

    // Check if the buffer contains a complete sentence
    int sentenceEnd = buffer.indexOf("\r\n");
    while (sentenceEnd != -1) {
        // Extract the complete sentence
        QString sentence = buffer.left(sentenceEnd);
        qint16 counter = 0;
        QString myCounter = QString::number(counter);

        // Print the sentence
        qDebug() << socketDescriptor << " Sentence received: " << sentence;

        // Open the database
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", myCounter);
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("1234");
        db.setDatabaseName("MySqlDatabase");

        if (db.open()) {
            qDebug() << "Connection Successful!!";
            // Execute the query
            QSqlQuery query(sentence, db);
            if (query.exec()) {
                qDebug() << "Query Exec successfull";
                QStringList results;
                int columnCount = query.record().count();
                while (query.next()) {
                    QString rowResult;
                    for (int i = 0; i < columnCount; i++) {
                        QString columnResult = query.value(i).toString();
                        rowResult.append(columnResult + "\t");
                    }
                    qDebug() << rowResult;
                    results.append(rowResult);
                    QByteArray rowResultBytes = rowResult.toUtf8(); // Convert QString to QByteArray
                    socket->write(rowResultBytes); // Write the row result to the socket
                }
                // Emit the query results
                emit queryResults(results.join("\n"));
            } else {
                // Emit query error
                emit queryError(query.lastError().text());
            }
        } else {
            // Emit database error
            emit databaseError(db.lastError().text());
        }

        db.close();
        // Remove the processed sentence from the buffer
        buffer = buffer.mid(sentenceEnd + 2);

        // Find the next sentence in the buffer
        sentenceEnd = buffer.indexOf("\r\n");
    counter++;
    }
}




void MyThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";


    socket->deleteLater();
    exit(0);
}

// Implementation for the error(QAbstractSocket::SocketError) signal

