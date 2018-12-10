// Window subclassing example - class implementation
// (NDE, 2016-09-08)

//#include <QtWidgets> // qt5 only?
#include <QHBoxLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QApplication>
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>
#include <iostream>
#include <QTextStream>
#include "stats.h"
#include "tabs.h"

MainWindow::MainWindow()
{
	setFixedSize(600, 480);

	createWindowContent();
	createMenuAndStatusBar();
	repoInit();

}

void MainWindow::createWindowContent()
{
	// Create layout
	QTabWidget* tabs = new QTabWidget();

	QHBoxLayout* layout=new QHBoxLayout();
	//layout->addStretch();

	// transfer ownership?
	layout->addWidget(tabs);

	QWidget *StatisticsTab = new QWidget(tabs);
	tabs->addTab(StatisticsTab, "Statistics");

	QPushButton*	summary_pushButton;
	QPushButton*	totals_pushButton;
	QPushButton*	save_pushButton;
	summary_pushButton = new QPushButton;
	totals_pushButton = new QPushButton;
	save_pushButton = new QPushButton;
	const QSize btnSize = QSize(100, 25);
	summary_pushButton->setFixedSize(btnSize);
	totals_pushButton->setFixedSize(btnSize);
	save_pushButton->setFixedSize(btnSize);
	summary_pushButton->setText("Summary");
	totals_pushButton->setText("Totals");
	save_pushButton->setText("Save");

	// slots
	connect(summary_pushButton, SIGNAL(clicked()), this, SLOT(summary()));
	connect(totals_pushButton, SIGNAL(clicked()), this, SLOT(totals()));
	connect(save_pushButton, SIGNAL(clicked()), this, SLOT(save()));

	QHBoxLayout* centralLayout = new QHBoxLayout();
	QVBoxLayout* verticalLayout = new QVBoxLayout();
	QHBoxLayout* buttonsLayout = new QHBoxLayout();
	QHBoxLayout* saveButtonsLayout = new QHBoxLayout();

	gitList = new QTableWidget;
	buttonsLayout->addWidget(summary_pushButton);
	buttonsLayout->addWidget(totals_pushButton);
	saveButtonsLayout->addWidget(save_pushButton);
	buttonsLayout->setAlignment(Qt::AlignRight);
	saveButtonsLayout->setAlignment(Qt::AlignRight);
	verticalLayout->addLayout(buttonsLayout);
	verticalLayout->addWidget(gitList);
	verticalLayout->addLayout(saveButtonsLayout);
	centralLayout->addLayout(verticalLayout);
	StatisticsTab->setLayout(centralLayout);

	// Put layout in the middle of the window
	_mainWidget = new QWidget();
	_mainWidget->setLayout(layout);

	setCentralWidget(_mainWidget);
}

void MainWindow::createMenuAndStatusBar()
{
	QMenu* fileMenu = menuBar()->addMenu("&File");

	QAction* open = fileMenu->addAction("&Open");
	open->setShortcuts(QKeySequence::Open);
	open->setStatusTip("Open an existing file");

	QAction* quit = fileMenu->addAction("Quit");
	quit->setShortcuts(QKeySequence::Quit);
	quit->setStatusTip("Quit the application");

	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::prepareGitList()
{
	gitList->verticalHeader()->setVisible(false);
	gitList->setSelectionBehavior(QAbstractItemView::SelectRows);
	gitList->setSelectionMode(QAbstractItemView::SingleSelection);
	gitList->setShowGrid(false);
	COL_GITLIST_COUNT = 6;  // how many columns in this list
	COL_GITLIST_ITEM = 0;
	COL_GITLIST_AUTHOR = 1;
	COL_GITLIST_COMMITS = 2;
	COL_GITLIST_INSERTIONS = 3;
	COL_GITLIST_DELETIONS = 4;
	COL_GITLIST_PER_CHANGES = 5;
	gitList->setColumnCount(COL_GITLIST_COUNT);
	gitList->setColumnWidth(COL_GITLIST_ITEM, 60);
	gitList->setColumnWidth(COL_GITLIST_AUTHOR, 110);
	gitList->setColumnWidth(COL_GITLIST_COMMITS, 90);
	gitList->setColumnWidth(COL_GITLIST_INSERTIONS, 90);
	gitList->setColumnWidth(COL_GITLIST_DELETIONS, 90);
	gitList->setColumnWidth(COL_GITLIST_PER_CHANGES, 90);
	gitList->horizontalHeader()->setStretchLastSection(true); // make the table fills the window

	gitList->setHorizontalHeaderItem(COL_GITLIST_ITEM, new QTableWidgetItem("ITEM"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_AUTHOR, new QTableWidgetItem("AUTHOR"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_COMMITS, new QTableWidgetItem("COMMITS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_INSERTIONS, new QTableWidgetItem("INSERTIONS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_DELETIONS, new QTableWidgetItem("DELETIONS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_PER_CHANGES, new QTableWidgetItem("% CHANGES"));

}

void MainWindow::prepareGitListTotals()
{
	COL_GITLIST_COUNT = 4;  // how many columns in this list
	COL_GITLIST_ITEM = 0;
	COL_GITLIST_COMMITS = 1;
	COL_GITLIST_INSERTIONS = 2;
	COL_GITLIST_DELETIONS = 3;
	gitList->setColumnCount(COL_GITLIST_COUNT);
	gitList->setColumnWidth(COL_GITLIST_ITEM, 60);
	gitList->setColumnWidth(COL_GITLIST_COMMITS, 150);
	gitList->setColumnWidth(COL_GITLIST_INSERTIONS, 150);
	gitList->setColumnWidth(COL_GITLIST_DELETIONS, 150);
	gitList->horizontalHeader()->setStretchLastSection(true); // make the table fills the window

	gitList->setHorizontalHeaderItem(COL_GITLIST_ITEM, new QTableWidgetItem("ITEM"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_COMMITS, new QTableWidgetItem("COMMITS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_INSERTIONS, new QTableWidgetItem("INSERTIONS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_DELETIONS, new QTableWidgetItem("DELETIONS"));
}
