#ifndef TCPTHREAD_H
#define TCPTHREAD_H

//#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include "server.h"
#include "tcpserver.h"
#include "datachange.h"
#include <QtSql/QSqlDatabase>
#include <QtXml/QtXml> 
#include <QDomDocument>  

class TcpThread : public QThread
{
	Q_OBJECT

public:
	TcpThread(int socketDescriptor,QObject *parent);
	//TcpThread(QTcpSocket *socket,QObject *parent);
	~TcpThread();
	void run();
	//database
	void dataBaseConnect();
	void fileInfotoXml(QString filename,qint64 filesize, QString upordown, QHostAddress clientip);//存储文件信息
signals:
	void error(QTcpSocket::SocketError socketError);
	void bytesArrived(qint64,qint32,int);
	public slots:
		void receiveFile();  //接收客户端发过来的文件,这个方法暂时没有起到作用
		void receiveData();  //接收客户端发过来的文件数据
		void startTransfer();
		void updateClientProgress(qint64 numBytes);
		void displayError(QAbstractSocket::SocketError socketError);  
		void connectError();   //连接断开，终止本线程
private:
	int socketDescriptor;
	//QThread tcpThread;
	TcpServer *tcpServer;  
	QTcpSocket *tcpServerConnection;  
	QTcpSocket *tcpSocket;
	qint64 TotalBytes;     //file总大小
	qint64 bytesReceived;  //收到的总字节
	qint64 fileNameSize;  //准备读取的字节
	QString fileName;  //文件路径名称
	QFile *localFile;  
	QByteArray inBlock;
	QByteArray outBlock;
	qint64 bytesToWrite;
	qint64 bytesWritten;
	qint64 loadSize;
	//接收数据
	QString message;  //存放从服务器接收到的字符串  
	quint16 blockSize;  //存放文件的大小信息 
	//QString str;
	QString sFileName;    //真实文件名
	QString sFile;      //文件类型标识,上传或者下载
	QString m_filePath;  //文件存储路径
	QString clientip;    //客户端ip
	QHostAddress qip;

	datachange *_datachange;
};

#endif // TCPTHREAD_H
