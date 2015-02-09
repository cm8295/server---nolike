#ifndef SERVER_H
#define SERVER_H

#include <QtGui/QMainWindow>
#include "ui_server.h"
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include "tcpthread.h"
#include "tcpserver.h"

class server : public QMainWindow
{
	Q_OBJECT

public:
	server(QWidget *parent = 0, Qt::WFlags flags = 0);
	~server();
	void CreatTrayMenu();
	void CreatTrayIcon();
	void closeEvent(QCloseEvent *event);
	private slots:
		void on_OkButton_clicked();
		//void updateProgress(qint64,qint32,int);
		void iconActivated(QSystemTrayIcon::ActivationReason reason);
private:
	TcpServer tcpServer;
	QSystemTrayIcon *myTrayIcon;
	QMenu *myMenu;
	QAction *miniSizeAction;  
	QAction *maxSizeAction;  
	QAction *restoreWinAction; 
	QAction *quitAction;
public:
	Ui::serverClass ui;
};

#endif // SERVER_H
