#ifndef DATACHANGE_H
#define DATA_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextCodec>
#include <QByteArray>
#include <QCryptographicHash>

class datachange : public QObject
{
	Q_OBJECT

public:
	datachange(QObject *parent);
	~datachange();
	void writeINI(QString filewatchpath);   //读取INI文件
	void readINI(QString filewatchpath);    //写INI文件
	bool fileExist(QString path);  //文件是否存在
	void fileWatchPath(QString filewatchpath);//修改文件监视路径
	QByteArray getFileMd5(QString filePath);   //获取文件MD5码

private:
	
};

#endif // DATACHANGE_H
