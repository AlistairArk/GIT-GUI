#include "globals.h"

#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <window.h>
#include <QWidget>
#include "Create_Config.h"


// namespace{
// 	class HelloWorldLabel;
// 	INSTALL_TAB(HelloWorldLabel, "Create/ Select");
// }
	INSTALL_TAB(HelloWorldLabel, "Create/ Select");

	HelloWorldLabel::HelloWorldLabel(){

		QPushButton *browse = new QPushButton("&Select Directory", this);
		QLineEdit *lnEditUsername = new QLineEdit;
		QLineEdit *lnEditEmail = new QLineEdit;

		lnEditEmail->setPlaceholderText("<Email>");
		lnEditUsername->setPlaceholderText("<Username>");

		QFormLayout *formLayout = new QFormLayout;
		formLayout->addRow(tr("&Browse to Directory to locate/create git repo:"), browse);
		formLayout->addRow(tr("&Change Username:"), lnEditUsername);//username
		formLayout->addRow(tr("&BChange Email:"), lnEditEmail);//email
		//formLayout->addRow(tr("&Browse to Directory to locate/create git repo:"), button);//assign/change remote

		//QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		setLayout(formLayout);

		//connect browse button to fileDialog
		//connect(browse,SIGNAL (clicked()),on_pushButton_clicked);
		//connect(m_button, SIGNAL (released()),this, SLOT (handleButton()));
    connect(browse, SIGNAL (released()),this, SLOT (on_pushButton_clicked()));

		//connect(browse, &QPushButton::clicked, this, &QTTest::OnClickedPushButton);


	}

	void HelloWorldLabel::on_pushButton_clicked()
	{
		 QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	}
