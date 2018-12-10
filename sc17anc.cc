#include "globals.h"
#include "sc17anc.h"

#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

INSTALL_TAB(SearchTab, "Search");

SearchTab::SearchTab(){

		Search = new QGridLayout;
		QPushButton *SButton = new QPushButton("Search");
		keyword = new QLineEdit();

		//Search->addWidget(new QLabel("Selected repo : "), 0, 0);
		Search->addWidget(new QLabel(), 0, 1);
		Search->addWidget(new QLabel("Keyword Search : "), 1, 0);
		Search->addWidget(keyword, 1, 1);
		Search->addWidget(SButton, 1, 2);
		Search->addWidget(new QLabel(""), 2, 0);
		setLayout(Search);

		connect(SButton, SIGNAL (released()), this, SLOT (SearchClicked()));
}
	void SearchTab::SearchClicked()
	{

		std::string data = keyword->text().toStdString();

		Search->addWidget(new QLabel("Branch"), 3, 0);
		Search->addWidget(new QLabel("Date"), 3, 1);
		Search->addWidget(new QLabel("Author"), 3, 2);
		Search->addWidget(new QLabel("Message"), 3, 3);

		int x = 4;
		for(auto i:r.commits())
		{
			if(i.message().find(data) != std::string::npos)
			{
				Search->addWidget(new QLabel(i.branch()), x, 0);
				Search->addWidget(new QLabel(i.time()), x, 1);
				Search->addWidget(new QLabel(i.signature().name()), x, 2);
				Search->addWidget(new QLabel(i.message()), x, 3);
				x++;
			}
		}
		setLayout(Search);
	}

	void Clear()
	{
		int x = 4;
		for(auto i:r.commits())
		{
			if(i.message().find(data) != std::string::npos)
			{
				Search->addWidget(new QLabel(""), x, 0);
				Search->addWidget(new QLabel(""), x, 1);
				Search->addWidget(new QLabel(""), x, 2);
				Search->addWidget(new QLabel(""), x, 3);
				x++;
			}
		}
	}
