#include "server.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	server w;
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	w.show();
	return a.exec();
}
