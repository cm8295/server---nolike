/********************************************************************************
** Form generated from reading UI file 'server.ui'
**
** Created: Tue Jan 20 11:25:10 2015
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_H
#define UI_SERVER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_serverClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLabel *statuslabel;
    QProgressBar *progressBar;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *serverClass)
    {
        if (serverClass->objectName().isEmpty())
            serverClass->setObjectName(QString::fromUtf8("serverClass"));
        serverClass->resize(460, 229);
        centralWidget = new QWidget(serverClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(310, 60, 75, 23));
        statuslabel = new QLabel(centralWidget);
        statuslabel->setObjectName(QString::fromUtf8("statuslabel"));
        statuslabel->setGeometry(QRect(30, 130, 121, 16));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(30, 60, 261, 23));
        progressBar->setValue(0);
        serverClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(serverClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 460, 23));
        serverClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(serverClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        serverClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(serverClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        serverClass->setStatusBar(statusBar);

        retranslateUi(serverClass);

        QMetaObject::connectSlotsByName(serverClass);
    } // setupUi

    void retranslateUi(QMainWindow *serverClass)
    {
        serverClass->setWindowTitle(QApplication::translate("serverClass", "server", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("serverClass", "start", 0, QApplication::UnicodeUTF8));
        statuslabel->setText(QApplication::translate("serverClass", "State", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class serverClass: public Ui_serverClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_H
