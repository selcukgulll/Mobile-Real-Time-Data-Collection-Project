#ifndef A_H
#define A_H

#include <QTcpServer>
#include <QObject>

class a : public QTcpServer
{
    Q_OBJECT
public:
    a();
};

#endif // A_H
