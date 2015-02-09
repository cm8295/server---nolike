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
	void fileInfotoXml(QString filename,qint64 filesize, QString upordown, QHostAddress clientip);//�洢�ļ���Ϣ
signals:
	void error(QTcpSocket::SocketError socketError);
	void bytesArrived(qint64,qint32,int);
	public slots:
		void receiveFile();  //���տͻ��˷��������ļ�,���������ʱû��������
		void receiveData();  //���տͻ��˷��������ļ�����
		void startTransfer();
		void updateClientProgress(qint64 numBytes);
		void displayError(QAbstractSocket::SocketError socketError);  
		void connectError();   //���ӶϿ�����ֹ���߳�
private:
	int socketDescriptor;
	//QThread tcpThread;
	TcpServer *tcpServer;  
	QTcpSocket *tcpServerConnection;  
	QTcpSocket *tcpSocket;
	qint64 TotalBytes;     //file�ܴ�С
	qint64 bytesReceived;  //�յ������ֽ�
	qint64 fileNameSize;  //׼����ȡ���ֽ�
	QString fileName;  //�ļ�·������
	QFile *localFile;  
	QByteArray inBlock;
	QByteArray outBlock;
	qint64 bytesToWrite;
	qint64 bytesWritten;
	qint64 loadSize;
	//��������
	QString message;  //��Ŵӷ��������յ����ַ���  
	quint16 blockSize;  //����ļ��Ĵ�С��Ϣ 
	//QString str;
	QString sFileName;    //��ʵ�ļ���
	QString sFile;      //�ļ����ͱ�ʶ,�ϴ���������
	QString m_filePath;  //�ļ��洢·��
	QString clientip;    //�ͻ���ip
	QHostAddress qip;

	datachange *_datachange;
};

#endif // TCPTHREAD_H
