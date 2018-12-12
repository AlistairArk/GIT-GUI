#include "globals.h"
#include "sc17ssm.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QHeaderView>

namespace display{

	int repoCheck(){ // Checks to see if valid repo path entered
	    try{
	        GITPP::REPO r(myDirStr);
	        return(1);
	    }catch(const std::exception& e){
	        return(0); // No repo loaded
	    }
	}

displayCommit::displayCommit(){

		display = new QGridLayout;
		/*std::string path = ".";
		try
		{
			GITPP::REPO r(myDirStr);
		}catch(GITPP::EXCEPTION_CANT_FIND const&)
		{
			display->addWidget(new QLabel("No repository found"),0,0);
		}
		GITPP::REPO r;*/

		//display->addWidget(new QLabel("Current repository is: "),0,0);
		//display->addWidget((r),0,1);
		/*try
		{
			GITPP::REPO r(myDirStr);
		}catch(GITPP::EXCEPTION_CANT_FIND const&)
		{
			display->addWidget(new QLabel("No repository found"),0,0);
		}

		display->addWidget(new QLabel("Current repository is: "),0,0);*/
		//display->addWidget(new Qlabel())

		/*gridLayout->addWidget(new QLabel("Date/Time"),0,0);
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
		setLayout(gridLayout);*/
		/*if(commits == NULL)
		{
			commits = new QTableWidget();
			display->addWidget(commits);
		}
		else{
			for(int i = commits->rowCount(); i>=0; i--)
			{
				commits->removeRow(i);
			}
		}
		commits->setColumnCount(3);
		Header<<"Date"<<"Author"<<"Message";
		commits->setHorizontalHeaderLabels(Header);
		commits->verticalScrollBar()->setDisabled(false);
		commits->verticalHeader()->setVisible(false);*/

		if(repoCheck())
		{
			GITPP::REPO r(myDirStr);
			if(commits == NULL)
				{
					commits = new QTableWidget();
					display->addWidget(commits);
				}
				else{
					for(int i = commits->rowCount(); i>=0; i--)
					{
						commits->removeRow(i);
					}
				}
				commits->setColumnCount(3);
				Header<<"Date"<<"Author"<<"Message";
				commits->setHorizontalHeaderLabels(Header);
				commits->verticalScrollBar()->setDisabled(false);
				commits->verticalHeader()->setVisible(false);
			int l = 0;
			for(auto i:r.commits())
			{
				commits->horizontalHeader()->setStretchLastSection(true);
				QString t = QString::fromStdString(i.time());
				QString author = QString::fromStdString(i.signature().name());
				QString message = QString::fromStdString(i.message());
				commits->insertRow(l);
				commits->setItem(l, 0, new QTableWidgetItem(t));
				commits->setItem(l, 1, new QTableWidgetItem(author));
				commits->setItem(l, 2, new QTableWidgetItem(message));
				l++;
			}
		}
		else
		{
			display->addWidget(new QLabel("No repository found"),0,0);
			if(commits == NULL)
				{
					commits = new QTableWidget();
					display->addWidget(commits);
				}
				else{
					for(int i = commits->rowCount(); i>=0; i--)
					{
						commits->removeRow(i);
					}
				}
				commits->setColumnCount(3);
				Header<<"Date"<<"Author"<<"Message";
				commits->setHorizontalHeaderLabels(Header);
				commits->verticalScrollBar()->setDisabled(false);
				commits->verticalHeader()->setVisible(false);
		}

		/*int l = 0;
		for(auto i:r.commits())
		{
			commits->horizontalHeader()->setStretchLastSection(true);
			QString t = QString::fromStdString(i.time());
			QString author = QString::fromStdString(i.signature().name());
			QString message = QString::fromStdString(i.message());
			commits->insertRow(l);
			commits->setItem(l, 0, new QTableWidgetItem(t));
			commits->setItem(l, 1, new QTableWidgetItem(author));
			commits->setItem(l, 2, new QTableWidgetItem(message));
			l++;
		}*/
		setLayout(display);
	}
	INSTALL_TAB(displayCommit, "display");
}
