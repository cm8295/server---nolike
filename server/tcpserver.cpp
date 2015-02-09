#include "tcpserver.h"
#include "tcpthread.h"

TcpServer::TcpServer(QObject *parent)
	: QTcpServer(parent)
{

}

TcpServer::~TcpServer()
{

}

void TcpServer::incomingConnection(int socketDescriptor)
{
	/*fun1*/
	TcpThread *thread = new TcpThread(socketDescriptor,this);
	/*fun2*/
	/*QTcpSocket *tcp_sock_tmp = new QTcpSocket();
	tcp_sock_tmp->setSocketDescriptor(socketDescriptor);
	TcpThread *thread = new TcpThread(tcp_sock_tmp,this);*/
	connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
	connect(thread,SIGNAL(bytesArrived(qint64,qint32,int)),
		this,SIGNAL(bytesArrived(qint64,qint32,int)));
	thread->start();
}