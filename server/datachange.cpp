#include "datachange.h"

datachange::datachange(QObject *parent)
	: QObject(parent)
{
	
}

void datachange::writeINI(QString filewatchpath)
{ 
	//QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ  
	QSettings *configIniWrite = new QSettings("ProxySetting.ini", QSettings::IniFormat);  
	configIniWrite->setIniCodec(QTextCodec::codecForName("GB2312"));
	//configIniWrite.beginGroup("company");
	//��ini�ļ���д������,setValue���������������Ǽ�ֵ��  
	//��ini�ļ��ĵ�һ����д������,ip���µĵ�һ������  
	configIniWrite->setValue("/watchpath/first", filewatchpath);  
	//��ini�ļ��ĵ�һ����д������,ip���µĵڶ�������  
	//configIniWrite->setValue("watch/second", "127.0.0.1");  
	//��ini�ļ��ĵڶ�����д������,port���µĵ�һ������  
	//configIniWrite->setValue("port/open", "2222");  
	//д����ɺ�ɾ��ָ��  
	delete configIniWrite;  
}

void datachange::readINI(QString filewatchpath)
{
	QSettings *configIniRead = new QSettings("ProxySetting.ini", QSettings::IniFormat);  
	configIniRead->setIniCodec(QTextCodec::codecForName("GB2312"));
	//����ȡ����ini�ļ�������QString�У���ȡֵ��Ȼ��ͨ��toString()����ת����QString����   
	QString ipResult = configIniRead->value("/watchpath/first").toString();  
	//QString portResult = configIniRead->value("/port/open").toString();  
	//��ӡ�õ��Ľ��   
	qDebug() << ipResult;  
	//qDebug() << portResult;  
	//��������ɺ�ɾ��ָ��   
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
	//QString dirs = QFileDialog::getExistingDirectory(this,tr("����ļ���"),"/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
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
