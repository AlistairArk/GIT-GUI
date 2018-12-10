#include "globals.h"
#include "sc17anc.h"

#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QScrollBar>

namespace search{

SearchTab::SearchTab(){

		Search = new QGridLayout;
		QPushButton *SButton = new QPushButton("Search");
		keyword = new QLineEdit();

		Search->addWidget(new QLabel("Keyword Search : "), 0, 0);
		Search->addWidget(keyword, 0, 1);
		Search->addWidget(SButton, 0, 2);
		Search->addWidget(new QLabel(""), 1, 0);
		setLayout(Search);

		connect(SButton, SIGNAL (released()), this, SLOT (SearchClicked()));
}
	void SearchTab::SearchClicked()
	{
		if(Commits == NULL)
		{
			Commits = new QTableWidget();
			Search->addWidget(Commits);
		}
		else
		{
			for(int i = Commits->rowCount(); i >= 0; i--)
			{
				Commits->removeRow(i);
			}
		}
		Commits->setColumnCount(3);
		Header<<"Date"<<"Author"<<"Message";
		Commits->setHorizontalHeaderLabels(Header);
		Commits->verticalScrollBar()->setDisabled(false);
		Commits->verticalHeader()->setVisible(false);

		GITPP::REPO r(myDirStr);
		std::string data = keyword->text().toStdString();

		int x = 0;
		for(auto i:r.commits())
		{
			if(i.message().find(data) != std::string::npos)
			{
				Commits->horizontalHeader()->setStretchLastSection(true);
				QString t = QString::fromStdString(i.time());
				QString author = QString::fromStdString(i.signature().name());
				QString message = QString::fromStdString(i.message());
				Commits->insertRow(x);
				Commits->setItem(x, 0, new QTableWidgetItem(t));
				Commits->setItem(x, 1, new QTableWidgetItem(author));
				Commits->setItem(x, 2, new QTableWidgetItem(message));

				x++;
			}
		}
		setLayout(Search);
	}

	INSTALL_TAB(SearchTab, "Search");
}
