
#include "tabs.h"
#include "globals.h"

#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <iostream>


TabDialog::TabDialog(QWidget *parent)
    : QDialog(parent)
{

    _tabWidget = new QTabWidget;

	 // addTab passes ownership
	 for(WidgetDispatcher::const_iterator i=myTabs.begin(); i!=myTabs.end(); ++i){
		 _tabWidget->addTab(i->second->clone_instance(), tr(i->first.c_str()));
	 }

	 QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_tabWidget);

	 // transfers ownership
    setLayout(mainLayout);
}
