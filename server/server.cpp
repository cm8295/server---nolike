#include "server.h"

server::server(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	CreatTrayIcon();
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(on_OkButton_clicked()));  
}

server::~server()
{

}

void server::on_OkButton_clicked()
{ 
	while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress::Any,19842))
	//while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress("192.168.1.77"),19842))
		{
			QMessageBox::StandardButton ret = QMessageBox::critical(this,
			tr("Error"),
			tr("Ip error: %1.")
			.arg(tcpServer.errorString()),
			QMessageBox::Retry
			| QMessageBox::Cancel);
			if (ret == QMessageBox::Cancel)
				return;
			}
	ui.statuslabel->setText(tr("listening:%1").arg("19842"));
	ui.pushButton->setEnabled(false);
	/*connect(&tcpServer,SIGNAL(bytesArrived(qint64,qint32,int)),
		this,SLOT(updateProgress(qint64,qint32,int)));*/
	}

void server::CreatTrayMenu()  
{  
	miniSizeAction = new QAction("Min(&N)",this);  
	maxSizeAction = new QAction("Max(&X)",this);  
	restoreWinAction = new QAction("Restore(&R)",this);  
	quitAction = new QAction("Quit(&Q)",this);  
	this->connect(miniSizeAction,SIGNAL(triggered()),this,SLOT(hide()));  
	this->connect(maxSizeAction,SIGNAL(triggered()),this,SLOT(showMaximized()));  
	this->connect(restoreWinAction,SIGNAL(triggered()),this,SLOT(showNormal()));  
	this->connect(quitAction,SIGNAL(triggered()),qApp,SLOT(quit()));  
	myMenu = new QMenu((QWidget*)QApplication::desktop());  
	myMenu->addAction(miniSizeAction);  
	myMenu->addAction(maxSizeAction);  
	myMenu->addAction(restoreWinAction);  
	myMenu->addSeparator();     //����һ�������  
	myMenu->addAction(quitAction);  
}  

void server::CreatTrayIcon()
{
	CreatTrayMenu();  
	if (!QSystemTrayIcon::isSystemTrayAvailable())      //�ж�ϵͳ�Ƿ�֧��ϵͳ����ͼ��  
	{  
		return;  
	}  
	myTrayIcon = new QSystemTrayIcon(this);  
	myTrayIcon->setIcon(QIcon("myIcon.ico"));   //����ͼ��ͼƬ  
	setWindowIcon(QIcon("myIcon.ico"));  //��ͼƬ���õ�������  
	myTrayIcon->setToolTip("server V1.0");    //����ʱ��������ȥ����ʾ��Ϣ  
	myTrayIcon->showMessage("server","server",QSystemTrayIcon::Information,10000);  
	myTrayIcon->setContextMenu(myMenu);     //�������������Ĳ˵�  
	myTrayIcon->show();  
	this->connect(myTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));  
}

void server::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason)
	{
	case QSystemTrayIcon::Trigger:
	case QSystemTrayIcon::DoubleClick:  
		showNormal(); 
		break; 
	case QSystemTrayIcon::MiddleClick:  
		myTrayIcon->showMessage("server","server",QSystemTrayIcon::Information,10000);  
		break;  
	default:  
		break;
	}
}

void server::closeEvent(QCloseEvent *event)  
{  
	if (myTrayIcon->isVisible())  
	{  
		myTrayIcon->showMessage("server","hide",QSystemTrayIcon::Information,5000);  

		hide();     //��С��  
		event->ignore();  
	}  
	else  
		event->accept();  
} 