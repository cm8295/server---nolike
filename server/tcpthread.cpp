#include "tcpthread.h"
#include <QtGui>
#include <QtNetwork>
#include <QDir>

TcpThread::TcpThread(int socketDescriptor,QObject *parent)
	: QThread(parent),socketDescriptor(socketDescriptor)
{
	TotalBytes = 0;  
	bytesReceived = 0;  
	fileNameSize = 0;  
	blockSize = 0;
	loadSize = 4*1024;
	bytesToWrite = 0;
	bytesWritten = 0;
	//dataBaseConnect();
}

TcpThread::~TcpThread()
{
	this->tcpServerConnection->deleteLater();
	quit();
}

void TcpThread::run()
{
	tcpServerConnection = new QTcpSocket;
	if (!tcpServerConnection->setSocketDescriptor(socketDescriptor)) {
		emit error(tcpServerConnection->error());
		return;
	}
	//qDebug()<<socketDescriptor;
	connect(tcpServerConnection, SIGNAL(readyRead()),
	this, SLOT(receiveData()),Qt::BlockingQueuedConnection);
	connect(tcpServerConnection,SIGNAL(error(QAbstractSocket::SocketError)),
		this,SLOT(displayError(QAbstractSocket::SocketError))); 
	connect(tcpServerConnection,SIGNAL(connectionClosed()),this,SLOT(connectError()));
	//qDebug()<<"ip is "<< tcpServerConnection->peerAddress();
	exec();
}

void TcpThread::connectError()
{
	//localFile->close();
 	tcpServerConnection->abort();
// 	quit();
 	qDebug()<<"connectError";
}

void TcpThread::receiveFile()
{
	QDataStream in(tcpServerConnection);  
	in.setVersion(QDataStream::Qt_4_7); 
	if(bytesReceived <= sizeof(qint64)*2)  
	{  
		if((tcpServerConnection->bytesAvailable() >=  
			sizeof(qint64)*2)&&(fileNameSize==0))  
		{  
			in>>TotalBytes>>fileNameSize;  
			bytesReceived += sizeof(qint64)*2;  
		}  
		if((tcpServerConnection->bytesAvailable()>=  
			fileNameSize)&&(fileName ==0))  
		{  
			in>>fileName;  
			bytesReceived += fileNameSize;  
			QDir qdircheck;
			QString filePathtemp = "E:\\FileLoadTest1\\" + fileName;
			if(!qdircheck.exists(filePathtemp))
			{
				qdircheck.mkdir(filePathtemp);
			}
			localFile = new QFile(filePathtemp);  //文件存储的路径
			//localFile = new QFile("E:\\FileLoadTest\\" + fileName);  //文件存储的路径
			if(!localFile->open(QFile::WriteOnly))  
			{  
				return;  
			}  
		}  
		else  
		{  
			return;  
		}  
	}  
	if(bytesReceived<TotalBytes)  
	{  
		bytesReceived += tcpServerConnection->bytesAvailable();  
		inBlock = tcpServerConnection->readAll();  
		localFile->write(inBlock);  
		inBlock.resize(0);  
	}  
	//qDebug()<<TotalBytes; 
	//qDebug()<<bytesReceived;  
	if(bytesReceived == TotalBytes)  
	{   
		localFile->close();
		TotalBytes = 0;
		bytesReceived = 0;
		fileNameSize = 0;
		fileName.clear();
		QApplication::restoreOverrideCursor();  /*恢复为普通光标*/
	}  
}

void TcpThread::receiveData()
{
	try
	{
		QDataStream in(tcpServerConnection);  
		in.setVersion(QDataStream::Qt_4_7); 
		if(bytesReceived <= sizeof(qint64)*2)  
		{
			if((tcpServerConnection->bytesAvailable() >=  
				sizeof(qint64)*2)&&(fileNameSize==0))  
			{  
				in>>TotalBytes>>fileNameSize;  
				bytesReceived += sizeof(qint64)*2;  
			}  
			if((tcpServerConnection->bytesAvailable()>=  
				fileNameSize)&&(fileName ==0))  
			{ 
				in>>fileName;  
				sFile = fileName.left(fileName.indexOf('>') + 1);
				sFile += fileName.right(fileName.size()- fileName.lastIndexOf('<'));
				sFileName = fileName.right(fileName.size() - fileName.lastIndexOf('>') - 1);
				sFileName = sFileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
				sFileName = sFileName.left(sFileName.indexOf('<'));
				if(sFile == "DOWN_FILE><DOWN_END")    //下载文件
				{
					//遍历文件夹
					QStringList string_list;
					QString path = "E:\\FileLoadTest\\";
					QDir dir(path);
					if(!dir.exists())
					{
						return;
					}
					dir.setFilter(QDir::Files | QDir::NoSymLinks);
					QFileInfoList list = dir.entryInfoList();
					int file_count = list.count();
					if(file_count <= 0)
					{
						return;
					}

					bool blInfo = false;
					for(int i=0; i<list.length();i++)
					{
						QFileInfo file_info = list.at(i);
						QString suffix = file_info.suffix();
						string_list.append(file_info.fileName());
						if(file_info.fileName() == sFileName)
						{
							blInfo = true;
							break;
						}
					}
					if(!blInfo)
					{
						/*发送数据*/
						QString serverData = "No such file";
						QString serverMessage = "DATA_BEGIN>";
						serverMessage += serverData;
						serverMessage += "<DATA_END";
						QByteArray block;
						QDataStream out(&block,QIODevice::WriteOnly);
						out.setVersion(QDataStream::Qt_4_7);
						out<<qint64(0)<<qint64(0)<<serverMessage;
						TotalBytes += block.size();
						out.device()->seek(0);
						out<<TotalBytes<<qint64(block.size() - sizeof(qint64)*2);
						tcpServerConnection->write(block);
						TotalBytes = 0;
						bytesReceived = 0;
						fileNameSize = 0;
						fileName.clear();
						sFileName.clear();
						sFile.clear();
						QApplication::restoreOverrideCursor();
						return;
					}
					//
					fileName = path;
					fileName += sFileName;
					localFile = new QFile(fileName);
					if(!localFile->open(QFile::ReadOnly))  
					{  
						return;  
					}  
					TotalBytes = localFile->size(); 
					QDataStream sendOut(&outBlock,QIODevice::WriteOnly);  
					sendOut.setVersion(QDataStream::Qt_4_7);  
					QString currentFile = fileName.right(fileName.size()-  
						fileName.lastIndexOf('\\')-1);        //.right出去文件路径部分，仅将文件部分保存在currentFile中
					sendOut<<qint64(0)<<qint64(0)<<currentFile;  //构造一个临时的文件头
					TotalBytes += outBlock.size();  //获得文件头的实际存储大小
					sendOut.device()->seek(0);  //将读写操作指向从头开始
					sendOut<<TotalBytes<<qint64(outBlock.size()- sizeof(qint64)*2);  
					bytesToWrite = TotalBytes - tcpServerConnection->write(outBlock);  
					qDebug()<<currentFile<<TotalBytes;  
					outBlock.resize(0);
					//connect(tcpServerConnection,SIGNAL(connected()),this,SLOT(startTransfer()));
				}
				else if(sFile == "UP_FILE><UP_END")     //上传文件
				{
					bytesReceived += fileNameSize;  
					QDir qdircheck;
					QString tempPathstory = fileName.left(fileName.lastIndexOf('/'));
					tempPathstory = tempPathstory.right(tempPathstory.size() - tempPathstory.indexOf('>') - 1);
					tempPathstory = tempPathstory.left(tempPathstory.indexOf('<'));
					tempPathstory = tempPathstory.replace('/','\\');
					QFileInfo qfilecheck("D:\\FileLoadTest\\" + tempPathstory);
					if(!fileName.contains("/"))
					{
						m_filePath = "D:\\FileLoadTest\\";
					}
					else
					{
						m_filePath = "D:\\FileLoadTest\\" + tempPathstory + "\\";
					}
					if(!qdircheck.exists(m_filePath))
					{
						bool okorfalse1 = qdircheck.mkpath(m_filePath);
					}
					localFile = new QFile(m_filePath + sFileName);  //文件存储的路径
					if(!localFile->open(QFile::WriteOnly))  
					{  
						return;  
					}  
				}
			}  
			else  
			{  
				return;  
			} 
		}
		if(sFile == "UP_FILE><UP_END")
		{
			if(bytesReceived<TotalBytes)  
			{  
				bytesReceived += tcpServerConnection->bytesAvailable();  
				inBlock = tcpServerConnection->readAll();  
				localFile->write(inBlock);  
				inBlock.resize(0);  
			}  
			//qDebug()<<TotalBytes; 
			//qDebug()<<bytesReceived;  
			if(bytesReceived == TotalBytes)  
			{   
				//fileInfotoXml(m_filePath + sFileName,TotalBytes,"up",tcpServerConnection->peerAddress());
				localFile->close();
				TotalBytes = 0;
				bytesReceived = 0;
				fileNameSize = 0;
				m_filePath.clear();
				fileName.clear();
				sFileName.clear();
				sFile.clear();
				this->tcpServerConnection->deleteLater();
				localFile->deleteLater();
				quit();
				QApplication::restoreOverrideCursor();  /*恢复为普通光标*/
			}
		}
		else if(sFile == "DOWN_FILE><DOWN_END")
		{
			connect(tcpServerConnection,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
		}
	}
	catch(QString err)
	{
		localFile->close();
	}
	
}

void TcpThread::startTransfer()  
{   
	localFile = new QFile(fileName);  
	if(!localFile->open(QFile::ReadOnly))  
	{  
		return;  
	}  
	TotalBytes = localFile->size(); 
	QDataStream sendOut(&outBlock,QIODevice::WriteOnly);  
	sendOut.setVersion(QDataStream::Qt_4_7);  
	QString currentFile = fileName.right(fileName.size()-  
		fileName.lastIndexOf('/')-1);        //.right出去文件路径部分，仅将文件部分保存在currentFile中
	sendOut<<qint64(0)<<qint64(0)<<currentFile;  //构造一个临时的文件头
	TotalBytes += outBlock.size();  //获得文件头的实际存储大小
	sendOut.device()->seek(0);  //将读写操作指向从头开始
	sendOut<<TotalBytes<<qint64(outBlock.size()- sizeof(qint64)*2); 
	
	bytesToWrite = TotalBytes - tcpSocket->write(outBlock);  
	qDebug()<<currentFile<<TotalBytes;  
	outBlock.resize(0);  
}  

void TcpThread::updateClientProgress(qint64 numBytes)  
{  
	bytesWritten += (int)numBytes;  
	if(bytesToWrite > 0)  
	{  
		if(bytesToWrite > 0)  
		{  
			outBlock = localFile->read(qMin(bytesToWrite,loadSize));  
			bytesToWrite -= (int)tcpServerConnection->write(outBlock);  
			outBlock.resize(0);  
		}  
		else  
		{  
			localFile->close();  
		}  
	}  
	if(bytesWritten == TotalBytes)
	{
	}
} 

//database
void TcpThread::dataBaseConnect()
{
	QSqlDatabase data_base = QSqlDatabase::addDatabase("QMYSQL");
	data_base.setHostName("localhost");
	data_base.setPort(3306);
	data_base.setDatabaseName("tee");
	data_base.setUserName("root");
	data_base.setPassword("casit.3058519");
	if(!data_base.open())
	{
		//qDebug()<<"failed to conect to mysql";
	}
	else
	{
		//qDebug()<<"success";
	}
		
}

void TcpThread::fileInfotoXml(QString filename,qint64 filesize, QString upordown, QHostAddress clientip)
	/*filename:文件名称；filesize：文件大小；upordown:上传还是下载*/
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	QDomDocument document;
	QFile fileStory("Info.xml");
	if(!fileStory.exists("Info.xml"))
	{
		if(!fileStory.open(QIODevice::WriteOnly|QIODevice::Text))
		{
			return;
		}
		QString strHeader("version=\"1.0\" encoding=\"UTF-8\"");
		document.appendChild( document.createProcessingInstruction("xml", strHeader) );  

		QDomElement root_elem = document.createElement( "items" );   
		document.appendChild( root_elem );  
		QDomElement item1 = document.createElement( "item" );  
		item1.setAttribute( "filename", filename );  
		item1.setAttribute( "filesize", filesize );
		item1.setAttribute( "timer", current_date );  
		item1.setAttribute( "upordown", upordown );
		root_elem.appendChild( item1 );  
		QTextStream out( &fileStory );  
		document.save( out, 4 );  
		fileStory.close(); 
	}
	else if(fileStory.exists("Info.xml"))
	{
		if(!fileStory.open(QIODevice::ReadOnly))
		{
			return;
		}
		if(!document.setContent(&fileStory,false))
		{
			fileStory.close();
			return;
		}
		fileStory.close();
		if(!fileStory.open(QIODevice::WriteOnly|QIODevice::Text))
		{
			return;
		}
		QDomElement roote = document.documentElement();
		QDomElement item_roote = roote.firstChildElement();
		QDomElement item1 = document.createElement( "item" );  
		item1.setAttribute( "filename", filename );  
		item1.setAttribute( "filesize", filesize );
		item1.setAttribute( "timer", current_date );  
		item1.setAttribute( "upordown", upordown );
		item1.setAttribute( "clientip", clientip.toString());
		item_roote.appendChild( item1 );
		QTextStream out( &fileStory );  
		document.save( out, 4 );  
		fileStory.close(); 
	}
}

void TcpThread::displayError(QAbstractSocket::SocketError socketError)  
{  
	if(socketError == QTcpSocket::RemoteHostClosedError)  
		return; 
	tcpServerConnection->abort();
	localFile->close();
	localFile->remove(m_filePath + sFileName);
	this->terminate();
	//quit();
	/*QMessageBox::warning(this,tr("NetWork"),  
	tr("Error:%1.")
	.arg(tcpSocket->errorString()));*/
	//disconnect(tcpSocket,SIGNAL(connected()),this,SLOT(startTransfer()));  
	//disconnect(tcpSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
	//disconnect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
	//this,SLOT(displayError(QAbstractSocket::SocketError)));
	//tcpSocket->abort();  
	/*tcpSocket = new QTcpSocket();
	tcpSocket->connectToHost(QHostAddress(m_serverIp),m_serverPort);  
	connect(tcpSocket,SIGNAL(connected()),this,SLOT(startTransfer()));  
	connect(tcpSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
	connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
		this,SLOT(displayError(QAbstractSocket::SocketError))); */
} 