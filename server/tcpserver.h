#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>

class TcpServer : public QTcpServer
{
	Q_OBJECT

public:
	//explicit TcpServer(QObject *parent = 0);
	TcpServer(QObject *parent = 0);
	~TcpServer();
signals:
	//void bytesArrived(qint64,qint32,int);
protected:
	void incomingConnection(int socketDescriptor);
private:
	
};

#endif // TCPSERVER_H
