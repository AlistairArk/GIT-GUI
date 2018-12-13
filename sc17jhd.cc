/*
Branching Tab For Repo GUI Program
*/


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

int BranchHandler::repoCheck(){ // Checks to see if valid repo path entered
    try{
        GITPP::REPO r(myDirStr);
        return(1);
    }catch(const std::exception& e){
        return(0); // No repo loaded
    }
}





void BranchHandler::populateList(){     // Fills the branch listWidget
    GITPP::REPO r(myDirStr);
    int counter = -1;

    // Populate list widget
    for(GITPP::BRANCH i : r.branches()){
        counter ++;                     // Increment counter
        std::string str = i.name();
        const char * c = str.c_str();
        listWidget->addItem(c);

        if ("master" == i.name()){
            branchIndex = counter;
            // Set Default branch color
            listWidget->item(branchIndex)->setForeground(Qt::black);
            listWidget->item(branchIndex)->setBackground(Qt::green);
        }
    }
}




int BranchHandler::doRefresh(bool silent){
    // Checks to see if the repo has switched since last called
    // Refreshes tab if a change was detected

    if ((lastMyDirStr!=myDirStr) && repoCheck()){
        lastMyDirStr = myDirStr;
        listWidget->clear();        // Clear the listWidget
        populateList();             // Populate the listWidget
        return 1;                   // Repo Switched
    }else{
        if (!silent)
            QMessageBox::information(nullptr/*or parent*/, "NOTICE",
                QString("Nothing to update!"),
                QMessageBox::Ok);
        return 0;
    }
}



BranchHandler::BranchHandler(){
    myBranchStr = "master";     // Set default branch at runtime

    lastMyDirStr = myDirStr;    // Initialize Last dir string

    // Make Widgets
    QPushButton *branch = new QPushButton("&Confirm Selection", this);
    QPushButton *refresh = new QPushButton("&Update Branch List", this);
    listWidget = new QListWidget;

    if (repoCheck()){
        populateList();
    }

    // Add Widgets
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(refresh);
    formLayout->addRow(tr("&Branch Repository:"), branch);
    formLayout->addRow(listWidget);

    setLayout(formLayout);
    connect(branch, SIGNAL (released()),this, SLOT (doBranch()));
    connect(refresh, SIGNAL (released()),this, SLOT (doRefreshBtn()));
}




void BranchHandler::doRefreshBtn(){
    doRefresh(0);
}




void BranchHandler::doBranch(){
    if (repoCheck()){
        if (doRefresh(1)) { // If repo has not been switched
            QMessageBox::information(nullptr/*or parent*/, "NOTICE",
                QString("Repository change detected!\n\nThe branch list will now be updated. Please make a selection from the list of new branches before proceeding."),
                QMessageBox::Ok);
        }else{
            GITPP::REPO r(myDirStr);                        // connect to repo

            int selectedBranch = listWidget->currentRow();  // Get index of selected branch from "listWidget"
            int branchCounter = -1;                         // Reset branchCounter

            for(GITPP::BRANCH i : r.branches()){            // Loop till chosen branch found
                branchCounter+=1;
                if (selectedBranch == branchCounter){

                    // Ensue repo is not bare before checking out
                    auto c=r.config();
                    if (c["core.bare"].value()=="true"){
                            QMessageBox::warning(nullptr/*or parent*/, "WARNING",
                                QString("Cannot checkout. This operation is not allowed against bare repositories.\nPlease change modes and try again."),
                                QMessageBox::Ok);

                    }else{

                        // Error check required as conflicts can prevent checkout of branches
                        try{
                            myBranchStr = i.name();
                            r.checkout(myBranchStr);            // Switch to branch
                           // listCommit(0);                    // Now list commits in this branch

                            std::string str = i.name();
                            const char * c = str.c_str();

                            listWidget->item(branchIndex)->setForeground(Qt::black);   // Revert color of last item
                            listWidget->item(branchIndex)->setBackground(Qt::white);   // Revert color of last item
                            listWidget->item(selectedBranch)->setForeground(Qt::black);   // Set color of New item
                            listWidget->item(selectedBranch)->setBackground(Qt::green);   // Set color of New item
                            branchIndex = selectedBranch;
                            QMessageBox::information(nullptr/*or parent*/, "SUCCESS",
                                QString("You have connected to the following branch: %1")
                                .arg(c),

                                QMessageBox::Ok);
                            break;                          // Break out of loop

                        }catch(const std::exception& e){
                            std::string str = "e.what()";

                            QMessageBox::critical(nullptr/*or parent*/, "WARNING",
                                QString("The following error has occurred: \n %1 \nPlease resolve the aforementioned conflicts before trying again.")
                                .arg(e.what()),

                                QMessageBox::Ok);

                        }
                    }
                }
            }
        }

    }else{ // If not connected to a repo
        QMessageBox::information(nullptr/*or parent*/, "NOTICE",
            QString("You must be connected to a repo before doing that!"),

            QMessageBox::Ok);
    }
}
