#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _socket(this)
{
    ui->setupUi(this);
    _socket.connectToHost(QHostAddress("127.0.0.1"), 1234);
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(on_sendButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


// SELECT Amount FROM Production WHERE Product_ID =  "123241";

void MainWindow::onReadyRead()
{
    QByteArray data = _socket.readAll();
    qDebug() << "Received data:" << data;

    // Only write back to the socket if necessary
    if (data.contains("received")) {
        _socket.write(QByteArray("ok !\r\n"));
    }

    ui->receivedText->append(data);
}





void MainWindow::sendMessage(const QString& message)
{
    QString finalMessage = message + "\r\n"; // Add carriage return and line feed
    QByteArray data = finalMessage.toUtf8();
    _socket.write(data);
}


void MainWindow::on_sendButton_clicked()
{
    QString message = ui->messageEdit->toPlainText();
    sendMessage(message);
    ui->messageEdit->clear();
}
