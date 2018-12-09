#include "globals.h"

#include <QLabel>
#include <QHBoxLayout>

namespace{

class HelloWorldLabel : public QWidget{
public:
	HelloWorldLabel() : QWidget(){
		QHBoxLayout* h=new QHBoxLayout;

		h->addWidget(new QLabel("left"));
		h->addWidget(new QLabel("right"));
		setLayout(h);

	}
};

INSTALL_TAB(HelloWorldLabel, __FILE__);

}
