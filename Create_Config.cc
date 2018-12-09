#include "globals.h"
#include "gitpp7.h"
#include "Create_Config.h"

#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QStatusBar>
#include <window.h>
#include <QWidget>
#include <QMessageBox>



	INSTALL_TAB(HelloWorldLabel, "Create/ Select");

	HelloWorldLabel::HelloWorldLabel(){

		//Creates form items
		QPushButton *browse = new QPushButton("&Select Directory", this);
		QLineEdit *lnEditUsername = new QLineEdit;
		QLineEdit *lnEditEmail = new QLineEdit;

		lnEditEmail->setPlaceholderText("<Email>");
		lnEditUsername->setPlaceholderText("<Username>");

		//sets form layout
		QFormLayout *formLayout = new QFormLayout;
		formLayout->addRow(tr("&Browse to Directory to locate/create git repo:"), browse);
		formLayout->addRow(tr("&Change Username:"), lnEditUsername);//username
		formLayout->addRow(tr("&BChange Email:"), lnEditEmail);//email
		//formLayout->addRow(tr("&Browse to Directory to locate/create git repo:"), button);//assign/change remote

		setLayout(formLayout);

    connect(browse, SIGNAL (released()),this, SLOT (on_pushButton_clicked()));

	}

	void HelloWorldLabel::on_pushButton_clicked()
	{
		//opens file browse dialog and saves directory to dir
		 QString myDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

		 std::string myDirStr = myDir.toUtf8().constData();

		 try{
				GITPP::REPO r(myDirStr);
				
				QMessageBox::question(
            this,
				tr("Congratualtions!"),
				tr("A git repo was found!"),

				QMessageBox::Ok
				 );

			}catch(GITPP::EXCEPTION_CANT_FIND const&){
				GITPP::REPO r(GITPP::REPO::_create, myDirStr);
				QMessageBox::question(
            this,
            tr("Git repo not found"),
            tr("No git repo found. A git will be created for you in this directory"),

            QMessageBox::Ok
             );
			}
			//_ctx.set_status("Initalised new repo. [ENTER] to return");

	}
