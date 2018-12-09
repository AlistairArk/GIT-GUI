#include "globals.h"

#include <QLabel>

namespace{

class HelloWorldLabel : public QLabel{
public:
	HelloWorldLabel() : QLabel(){
		setText("Hello World");
	}
};

INSTALL_TAB(HelloWorldLabel, "hello");

}
