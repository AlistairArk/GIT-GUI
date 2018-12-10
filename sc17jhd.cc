#include "globals.h"
#include "sc17jhd.h"
#include "gitpp7.h"


#include <QPushButton>
#include <QFormLayout>
#include <QFileDialog>
#include <window.h>
#include <QWidget>
#include <QListWidget>
#include <QMessageBox>



INSTALL_TAB(BranchHandler, "Branch Repository");

BranchHandler::BranchHandler(){

    // Make Widgets
    QPushButton *branch = new QPushButton("&Branch Repo", this);
    listWidget = new QListWidget;


    GITPP::REPO r(myDirStr); // add myDirStr to globals.h for compatibility?

    // Populate list widget
    for(GITPP::BRANCH i : r.branches()){
        std::string str = i.name();
        const char * c = str.c_str();
        listWidget->addItem(c);
    }

    choice = 0;
    // Set Default branch color
    listWidget->item(choice)->setForeground(Qt::black);
    listWidget->item(choice)->setBackground(Qt::green);

    // Add Widgets
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("&Branch Repository:"), branch);
    formLayout->addRow(listWidget);



    setLayout(formLayout);

    connect(branch, SIGNAL (released()),this, SLOT (on_pushButton_clicked()));

}



void BranchHandler::on_pushButton_clicked(){

    GITPP::REPO r(myDirStr); // connect to repo

    listWidget->item(choice)->setForeground(Qt::black);   // Revert color of last item
    listWidget->item(choice)->setBackground(Qt::white);   // Revert color of last item
    choice = listWidget->currentRow();                    // Get index of selected branch from "listWidget"
    listWidget->item(choice)->setForeground(Qt::black);   // Set color of New item
    listWidget->item(choice)->setBackground(Qt::green);   // Set color of New item

    int branchCounter = -1;         // Reset branchCounter


    for(GITPP::BRANCH i : r.branches()){ // Loop till chosen branch found
        branchCounter+=1;
        if (choice == branchCounter){

            // Ensue repo is not bare before checking out
            auto c=r.config();
            if (c["core.bare"].value()=="true"){

                    QMessageBox::information(nullptr/*or parent*/, "WARNING",
                        QString("Cannot checkout. This operation is not allowed against bare repositories.\nPlease change modes and try again."),
                        QMessageBox::Ok);


            }else{

                // Error check required as conflicts can prevent checkout of branches
                try{
                    r.checkout(i.name());           // Switch to branch
                   // listCommit(0);                // Now list commits in this branch


                    std::string str = i.name();
                    const char * c = str.c_str();

                    QMessageBox::information(nullptr/*or parent*/, "SUCCESS",
                        QString("You have connected to the following branch: %1")
                        .arg(c),

                        QMessageBox::Ok);


                    break;                          // Break out of loop
                }catch(const std::exception& e){
                    std::string str = "e.what()";

                    QMessageBox::information(nullptr/*or parent*/, "WARNING",
                        QString("The following error has occurred: \n %1 \nPlease resolve the aforementioned conflicts before trying again.")
                        .arg(e.what()),

                        QMessageBox::Ok);

                }
            }
        }
    }
}
