#include "globals.h"
#include "sc17ssm.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QHeaderView>
#include <QMessageBox>
#include <QLayoutItem>

namespace display{

displayCommit::displayCommit(){

		display = new QGridLayout;
		refreshButton = new QPushButton("Refresh");
		label = new QLabel("No repository selected");

		commits->setColumnCount(3);
		Header<<"Date"<<"Author"<<"Message";
		commits->setHorizontalHeaderLabels(Header);
		commits->verticalScrollBar()->setDisabled(false);
		commits->verticalHeader()->setVisible(false);
		commits->horizontalHeader()->setStretchLastSection(true);
		commits->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

		refresh();

		display->addWidget(refreshButton,0,2);
		display->addWidget(commits, 1, 0, 1, 3);
		setLayout(display);
		connect(refreshButton, SIGNAL (released()), this, SLOT (refresh()));
	}
	void displayCommit::refresh(){
		if(commits->rowCount() > 0)
		{
			for(int i = commits->rowCount(); i>=0; i--)
			{
				commits->removeRow(i);
			}
		}
		try
		{
			GITPP::REPO r(myDirStr);
			auto c=r.config();
			std::cout << c["core.bare"].value();
			if(c["core.bare"].value()=="true")
			{
				r.checkout(myBranchStr);
				
			}
			int l = 0;
			label->setVisible(false);
			for(auto i:r.commits())
			{
				std::cout << l;
				QString t = QString::fromStdString(i.time());
				QString author = QString::fromStdString(i.signature().name());
				QString message = QString::fromStdString(i.message());
				commits->insertRow(l);
				commits->setItem(l, 0, new QTableWidgetItem(t));
				commits->setItem(l, 1, new QTableWidgetItem(author));
				commits->setItem(l, 2, new QTableWidgetItem(message));
				l++;
			}
			if(l == 0)
			{
				QMessageBox::information(this,tr("Alert!"),tr("No commits present"));
			}
			setLayout(display);
		}
		catch(const std::exception& e)
		{
			display->addWidget(label,0,0);
		}
	}
	INSTALL_TAB(displayCommit, "Commits");
}
