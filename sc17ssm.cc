#include "globals.h"
#include "gitpp7.h"

#include <QLabel>
#include <QHBoxLayout>

namespace{

class HelloWorldLabel : public QWidget{
public:
	HelloWorldLabel() : QWidget(){
		QGridLayout *gridLayout = new QGridLayout;
		std::string path = ".";
		int l = 1;
		try
		{
			GITPP::REPO r(myDirStr);
		}catch(GITPP::EXCEPTION_CANT_FIND const&)
		{
			gridLayout->addWidget(new QLabel("No repository found"),0,0);
		}
		GITPP::REPO r;
		auto c=r.config();

		gridLayout->addWidget(new QLabel("Date/Time"),0,0);
		gridLayout->addWidget(new QLabel("Author "),0,1);
		gridLayout->addWidget(new QLabel("Commit Message"),0,2);
		for(auto i : r.commits()){
			QString mess1 = QString::fromStdString(i.message());
			QString name1 = QString::fromStdString(i.signature().name());
			QString time1 = QString::fromStdString(i.time());
			gridLayout->addWidget(new QLabel(name1),l,1);
			gridLayout->addWidget(new QLabel(mess1),l,2);
			gridLayout->addWidget(new QLabel(time1),l,0);
			l++;
		}
		setLayout(gridLayout);

	}
};

INSTALL_TAB(HelloWorldLabel, __FILE__);

}
