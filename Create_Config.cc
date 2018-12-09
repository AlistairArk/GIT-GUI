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



	INSTALL_TAB(HelloWorldLabel, "Select/ Configure git repo");

	HelloWorldLabel::HelloWorldLabel(){

		//Creates form items
		QPushButton *browse = new QPushButton("&Select Directory", this);
		setConfig = new QPushButton("&Set Username and Email", this);
		lnEditUsername = new QLineEdit;
		lnEditEmail = new QLineEdit;

		lnEditEmail->setPlaceholderText("<Email>");
		lnEditUsername->setPlaceholderText("<Username>");
		setConfig->setEnabled(false);

		//sets form layout
		QFormLayout *formLayout = new QFormLayout;
		formLayout->addRow(tr("&Browse to Directory to locate/create git repo:"), browse);
		formLayout->addRow(tr("&Change Username:"), lnEditUsername);//username
		formLayout->addRow(tr("&Change Email:"), lnEditEmail);//email
		formLayout->addRow(setConfig);
		//formLayout->addRow(tr("&Browse to Directory to locate/create git repo:"), button);//assign/change remote

		setLayout(formLayout);

		//connects click event to function
    connect(browse, SIGNAL (released()),this, SLOT (on_browse_clicked()));
		connect(setConfig, SIGNAL (released()),this, SLOT (on_setConfig_clicked()));


	}

	void HelloWorldLabel::on_setConfig_clicked()
	{
		//need to do input validation

		QString username = lnEditUsername->text();
		QString user_email = lnEditEmail->text();

		GITPP::REPO r(myDirStr);
		auto c=r.config();

		//TODO :: stop this from crashig!
		//maybe try settinig git config manually through command line first then try though the app?
		GITPP::CONFIG::ITEM email=c["user.email"] = user_email.toStdString();
		GITPP::CONFIG::ITEM name=c["user.name"] = username.toStdString();

		lnEditEmail->setPlaceholderText(user_email);
		lnEditUsername->setPlaceholderText(username);

		QMessageBox::question(
				this,
				tr("It's working!"),
				tr("Is this working?"),

				QMessageBox::Ok
		 );
	}

	void HelloWorldLabel::on_browse_clicked()
	{
		//opens file browse dialog and saves directory to dir
		 QString myDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

		 myDirStr = myDir.toUtf8().constData();

		 //git repo verificatiton
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



			GITPP::REPO r(myDirStr);
			auto c=r.config();

			//try to retreive the username
			try{
				GITPP::CONFIG::ITEM name=c["user.name"];
				QString username = QString::fromStdString(name.value());
			}catch(GITPP::EXCEPTION_CANT_FIND const&){
				//if no user name exists then notify
				QMessageBox::question(
            this,
            tr("No username set!"),
            tr("No username exist for this repo"),

            QMessageBox::Ok
        );
			}

			//try to retreive the email
			try{
				GITPP::CONFIG::ITEM email=c["user.email"];
				QString user_email = QString::fromStdString(email.value());
			}catch(GITPP::EXCEPTION_CANT_FIND const&){
				//if no email exists then notify
				QMessageBox::question(
            this,
            tr("No email set!"),
            tr("No email exist for this repo"),

            QMessageBox::Ok
        );
			}
			setConfig->setEnabled(true);
	}
