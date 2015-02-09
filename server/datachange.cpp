#include "datachange.h"

datachange::datachange(QObject *parent)
	: QObject(parent)
{
	
}

void datachange::writeINI(QString filewatchpath)
{ 
	//QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省  
	QSettings *configIniWrite = new QSettings("ProxySetting.ini", QSettings::IniFormat);  
	configIniWrite->setIniCodec(QTextCodec::codecForName("GB2312"));
	//configIniWrite.beginGroup("company");
	//向ini文件中写入内容,setValue函数的两个参数是键值对  
	//向ini文件的第一个节写入内容,ip节下的第一个参数  
	configIniWrite->setValue("/watchpath/first", filewatchpath);  
	//向ini文件的第一个节写入内容,ip节下的第二个参数  
	//configIniWrite->setValue("watch/second", "127.0.0.1");  
	//向ini文件的第二个节写入内容,port节下的第一个参数  
	//configIniWrite->setValue("port/open", "2222");  
	//写入完成后删除指针  
	delete configIniWrite;  
}

void datachange::readINI(QString filewatchpath)
{
	QSettings *configIniRead = new QSettings("ProxySetting.ini", QSettings::IniFormat);  
	configIniRead->setIniCodec(QTextCodec::codecForName("GB2312"));
	//将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型   
	QString ipResult = configIniRead->value("/watchpath/first").toString();  
	//QString portResult = configIniRead->value("/port/open").toString();  
	//打印得到的结果   
	qDebug() << ipResult;  
	//qDebug() << portResult;  
	//读入入完成后删除指针   
	delete configIniRead;
}

bool datachange::fileExist(QString path)
{
	QFileInfo fileinfo(path);
	if(fileinfo.isFile());
	{
		if(fileinfo.exists())
		{
			return true;
		}
	}
	return false;
}

void datachange::fileWatchPath(QString filewatchpath)
{
	//QString dirs = QFileDialog::getExistingDirectory(this,tr("浏览文件夹"),"/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
}

QByteArray datachange::getFileMd5(QString filePath)
{
	QFile localFile(filePath);

	if (!localFile.open(QFile::ReadOnly))
	{
		qDebug() << "file open error.";
		return 0;
	}
	QCryptographicHash ch(QCryptographicHash::Md5);
	quint64 totalBytes = 0;
	quint64 bytesWritten = 0;
	quint64 bytesToWrite = 0;
	quint64 loadSize = 1024 * 4;
	QByteArray buf;
	totalBytes = localFile.size();
	bytesToWrite = totalBytes;
	while (1)
	{
		if(bytesToWrite > 0)
		{
			buf = localFile.read(qMin(bytesToWrite, loadSize));
			ch.addData(buf);
			bytesWritten += buf.length();
			bytesToWrite -= buf.length();
			buf.resize(0);
		}
		else
		{
			break;
		}

		if(bytesWritten == totalBytes)
		{
			break;
		}
	}
	localFile.close();
	QByteArray md5 = ch.result();
	return md5;
}

datachange::~datachange()
{

}
