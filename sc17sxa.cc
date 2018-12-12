//formerly Create_Config.cc
#include "globals.h"
#include "gitpp7.h"
#include "sc17sxa.h"

#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QStatusBar>
#include <window.h>
#include <QWidget>
#include <QMessageBox>


namespace config{

	GitConfig::GitConfig(){

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
		formLayout->addRow(tr("&Change Username:"), lnEditUsername);
		formLayout->addRow(tr("&Change Email:"), lnEditEmail);
		formLayout->addRow(setConfig);
		//formLayout->addRow(tr("&Browse to Directory to locate/create git repo:"), button);//assign/change remote

		setLayout(formLayout);

		//connects click event to function
    connect(browse, SIGNAL (released()),this, SLOT (on_browse_clicked()));
		connect(setConfig, SIGNAL (released()),this, SLOT (on_setConfig_clicked()));


	}

	void GitConfig::on_setConfig_clicked()
	{
		//need to do input validation
		//get text from user
		QString username = lnEditUsername->text();
		QString user_email = lnEditEmail->text();


		GITPP::REPO r(myDirStr);
		auto c=r.config();

		//set user defined config to repo config

		//if there is no config entry create them
		try{
			GITPP::CONFIG::ITEM email=c["user.email"] = user_email.toStdString();
		}catch(GITPP::EXCEPTION_CANT_FIND const&){
			c.create("user.email");
			GITPP::CONFIG::ITEM email=c["user.email"] = user_email.toStdString();
		}

		try{
				 GITPP::CONFIG::ITEM name=c["user.name"] = username.toStdString();
	 	}catch(GITPP::EXCEPTION_CANT_FIND const&){
			c.create("user.name");
			GITPP::CONFIG::ITEM name=c["user.name"] = username.toStdString();
		}


		//update placeholders
		lnEditEmail->setPlaceholderText(user_email);
		lnEditUsername->setPlaceholderText(username);

		//this will notify the user that changes have been mad successfully once this works properly
		QMessageBox::question(
				this,
				tr("Configuration applied!"),
				tr("Git repo configuration settings applied"),

				QMessageBox::Ok
		 );
	}

	void GitConfig::on_browse_clicked()
	{
		//opens file browse dialog and saves directory to dir
		 QString myDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

		 myDirStr = myDir.toUtf8().constData();
		 myBranchStr = "master";

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
				lnEditUsername->setPlaceholderText(username);
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
				lnEditEmail->setPlaceholderText(user_email);
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

		INSTALL_TAB(GitConfig, "Select/ Configure git repo");
	}
