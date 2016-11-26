#include "CGDemo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	CGDemo demo;
	demo.show();

	return a.exec();
}
