// Window subclassing example - main program
// (NDE, 2016-09-08)

#include "window.h"

#include <QApplication>
#include <iostream>
#include <unistd.h>

int argc=0;
char* argv[] = {NULL};

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	HelloWindow window;
	window.show();
	app.exec();

	// avoid segfault in QtWidget destructors.
	// _exit(0);
}
