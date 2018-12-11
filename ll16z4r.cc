// ::QT Git
// Operations on repository

#include <QApplication>
#include <QMessageBox>
#include <QShortcut>
#include <QTableWidget>
#include <QDebug>
#include <QFile>
#include <git2/types.h>
#include <iostream>
#include <QHeaderView>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QTextStream>
#include "window.h"
#include "tabs.h"
#include "ll16z4r.h"
#include "globals.h"
#include "gitpp7.h"

using namespace std;

typedef struct {
    size_t insertions;
    size_t deletions;
} diff_file_stats;

struct git_diff_stats {
    git_diff *diff;
    diff_file_stats *filestats;

    size_t files_changed;
    size_t insertions;
    size_t deletions;
    size_t renames;

    size_t max_name;
    size_t max_filestat;
    int max_digits;
};

struct log_state {
    git_repository *repo;
    const char *repodir;
    git_revwalk *walker;
    int hide;
    int sorting;
    int revisions;
};

int repoCheck();	// check if valid repo

namespace stats{

/**
 * Check libgit2 error code, printing error to stderr on failure and
 * exiting the program.
 */
void check_lg2(int error, const char *message, const char *extra)
{
    const git_error *lg2err;
    const char *lg2msg = "", *lg2spacer = "";

    if (!error)
        return;

    if ((lg2err = giterr_last()) != NULL && lg2err->message != NULL) {
        lg2msg = lg2err->message;
        lg2spacer = " - ";
    }
    char tmpbuf[2048];

    if (extra)
        sprintf(tmpbuf, "%s '%s' [%d]%s%s\n",
            message, extra, error, lg2spacer, lg2msg);
    else
        sprintf(tmpbuf, "%s [%d]%s%s\n",
            message, error, lg2spacer, lg2msg);
    QMessageBox::critical(0, "warning!", tmpbuf);
    exit(1);
}

/* The 'opts' struct captures all the various parsed command line options. */
struct opts {
    git_diff_options diffopts;
    git_diff_find_options findopts;
    int color;
    int cache;
    int output;
    git_diff_format_t format;
    const char *treeish1;
    const char *treeish2;
    const char *dir;
};

// class constructor. Widget initialization
StatsHandler::StatsHandler()
{
    // Initialize the summary, totals and save buttons

    QPushButton*	summary_pushButton;
    QPushButton*	totals_pushButton;
    QPushButton*	save_pushButton;

    // create
    summary_pushButton = new QPushButton;
    totals_pushButton = new QPushButton;
    save_pushButton = new QPushButton;
    const QSize btnSize = QSize(100, 25);

    // set size
    summary_pushButton->setFixedSize(btnSize);
    totals_pushButton->setFixedSize(btnSize);
    save_pushButton->setFixedSize(btnSize);

    // set the label
    summary_pushButton->setText("Summary");
    totals_pushButton->setText("Totals");
    save_pushButton->setText("Save");

    // slots
    connect(summary_pushButton, SIGNAL(clicked()), this, SLOT(summary()));
    connect(totals_pushButton, SIGNAL(clicked()), this, SLOT(totals()));
    connect(save_pushButton, SIGNAL(clicked()), this, SLOT(save()));

    // prepare the layout
    QHBoxLayout* centralLayout = new QHBoxLayout();
    QVBoxLayout* verticalLayout = new QVBoxLayout();
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    QHBoxLayout* saveButtonsLayout = new QHBoxLayout();

    gitList = new QTableWidget;

    // set the layout and assign the widget to each layout in order
    buttonsLayout->addWidget(summary_pushButton);
    buttonsLayout->addWidget(totals_pushButton);
    saveButtonsLayout->addWidget(save_pushButton);
    buttonsLayout->setAlignment(Qt::AlignRight);
    saveButtonsLayout->setAlignment(Qt::AlignRight);
    verticalLayout->addLayout(buttonsLayout);
    verticalLayout->addWidget(gitList);
    verticalLayout->addLayout(saveButtonsLayout);
    centralLayout->addLayout(verticalLayout);
    setLayout(centralLayout);
    repoInit();
}

// Check if the repo exist and initialize the std::map with the authors names
void StatsHandler::repoInit ()
{
    std::string path=".";
    QString qitFolderName(QString(path.c_str())+"/.git");
    QFile gitFolder (qitFolderName);

    if (!gitFolder.exists()) {	// check if valid repo
            QMessageBox::critical(0, "warning!", ".git repo not found");
	    QApplication::instance()->quit();
	    exit (1);
    }
    GITPP::REPO repository(path.c_str());
    // clear the std::map first
    m_commits.erase (m_commits.begin (), m_commits.end ());
    // initialize
    for(auto i : repository.branches())
    {
        repository.checkout(i.name());
        for(auto i : repository.commits())
        {
            m_commits[i.author()].ids.push_back (i.id());
        }
    }
    summary ();
}

// Load and display the summary listing
void StatsHandler::summary ()
{
    // set the sorting method to the fields in the listing
    QHeaderView* head = gitList->horizontalHeader();
    head->setSortIndicator(-1, Qt::AscendingOrder);
    gitList->setSortingEnabled(false);
    std::map<std::string,CommitData>::iterator mapit;
    for (mapit = m_commits.begin(); mapit != m_commits.end();mapit++) {
        // reset totals by user

        mapit->second.insertions = 0;
        mapit->second.deletions = 0;
        mapit->second.files_changed = 0;
    }
    prepareGitList();   // initialize the listing (headers)
    loadData ();        // load the data and display
    gitList->setSortingEnabled(true);
}

// initialize the listing (headers)
void StatsHandler::prepareGitList()
{
    // set general settings of the listing: no headers, no grid
	gitList->verticalHeader()->setVisible(false);
	gitList->setSelectionBehavior(QAbstractItemView::SelectRows);
	gitList->setSelectionMode(QAbstractItemView::SingleSelection);
	gitList->setShowGrid(false);
    // initialize variables storing the fields order and numbers of fields.
	COL_GITLIST_COUNT = 6;  // how many columns in this list
	COL_GITLIST_ITEM = 0;
	COL_GITLIST_AUTHOR = 1;
	COL_GITLIST_COMMITS = 2;
	COL_GITLIST_INSERTIONS = 3;
	COL_GITLIST_DELETIONS = 4;
	COL_GITLIST_PER_CHANGES = 5;
    // assign columns width
	gitList->setColumnCount(COL_GITLIST_COUNT);
	gitList->setColumnWidth(COL_GITLIST_ITEM, 60);
	gitList->setColumnWidth(COL_GITLIST_AUTHOR, 140);
	gitList->setColumnWidth(COL_GITLIST_COMMITS, 90);
	gitList->setColumnWidth(COL_GITLIST_INSERTIONS, 100);
	gitList->setColumnWidth(COL_GITLIST_DELETIONS, 90);
	gitList->setColumnWidth(COL_GITLIST_PER_CHANGES, 90);
	gitList->horizontalHeader()->setStretchLastSection(true); // make the table fills the window
    // set header labels
	gitList->setHorizontalHeaderItem(COL_GITLIST_ITEM, new QTableWidgetItem("ITEM"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_AUTHOR, new QTableWidgetItem("AUTHOR"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_COMMITS, new QTableWidgetItem("COMMITS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_INSERTIONS, new QTableWidgetItem("INSERTIONS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_DELETIONS, new QTableWidgetItem("DELETIONS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_PER_CHANGES, new QTableWidgetItem("% CHANGES"));

}

// initialize the listing of totals (headers)
void StatsHandler::prepareGitListTotals()
{
    // initialize variables storing the fields order and numbers of fields.

	COL_GITLIST_COUNT = 4;  // how many columns in this list
	COL_GITLIST_ITEM = 0;
	COL_GITLIST_COMMITS = 1;
	COL_GITLIST_INSERTIONS = 2;
	COL_GITLIST_DELETIONS = 3;
    // assign columns width
    gitList->setColumnCount(COL_GITLIST_COUNT);
	gitList->setColumnWidth(COL_GITLIST_ITEM, 60);
	gitList->setColumnWidth(COL_GITLIST_COMMITS, 150);
	gitList->setColumnWidth(COL_GITLIST_INSERTIONS, 150);
	gitList->setColumnWidth(COL_GITLIST_DELETIONS, 150);
	gitList->horizontalHeader()->setStretchLastSection(true); // make the table fills the window

    // set header labels

	gitList->setHorizontalHeaderItem(COL_GITLIST_ITEM, new QTableWidgetItem("ITEM"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_COMMITS, new QTableWidgetItem("COMMITS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_INSERTIONS, new QTableWidgetItem("INSERTIONS"));
	gitList->setHorizontalHeaderItem(COL_GITLIST_DELETIONS, new QTableWidgetItem("DELETIONS"));


}

/* Push object (for hide or show) onto revwalker. */
static void push_rev(struct log_state *s, git_object *obj, int hide)
{
    hide = s->hide ^ hide;

    /* Create revwalker on demand if it doesn't already exist. */
    if (!s->walker) {
        check_lg2(git_revwalk_new(&s->walker, s->repo),
            "Could not create revision walker", NULL);
        git_revwalk_sorting(s->walker, s->sorting);
    }

    if (!obj)
        check_lg2(git_revwalk_push_head(s->walker),
            "Could not find repository HEAD", NULL);
    else if (hide)
        check_lg2(git_revwalk_hide(s->walker, git_object_id(obj)),
            "Reference does not refer to a commit", NULL);
    else
        check_lg2(git_revwalk_push(s->walker, git_object_id(obj)),
            "Reference does not refer to a commit", NULL);

    git_object_free(obj);
}

// load the data from the repository
void StatsHandler::loadData ()
{
    git_repository *repo = NULL;
    struct git_diff_stats *stats;

    // set options (not all used)
    struct opts o = {
        GIT_DIFF_OPTIONS_INIT, GIT_DIFF_FIND_OPTIONS_INIT,
        -1, 0, 0, GIT_DIFF_FORMAT_PATCH, NULL, NULL, "."
    };

    // initialize the library
    git_libgit2_init();
    // open the repository
    check_lg2(git_repository_open(&repo, o.dir), "Could not open repository", o.dir);
    // initialize variables
    total_insertions = 0;
    total_deletions = 0;
    total_files_changed = 0;
    total_commits = 0;

    std::map<std::string,CommitData>::iterator mapit;

    // prepare to travel the repository
    struct log_state s;
    git_oid oid;
    int parents;
    int hide = 0;
    git_commit *commit = NULL;
    s.repo = repo;
    s.walker = NULL;
    push_rev(&s, NULL, hide);   // initialize the walker
    for (; !git_revwalk_next(&oid, s.walker); git_commit_free(commit)) {
        // get the commit data
        check_lg2(git_commit_lookup(&commit, repo, &oid),
            "Failed to look up commit", NULL);
        git_tree *a = NULL, *b = NULL;
        git_diff *diff = NULL;

        // get parents count
        parents = (int)git_commit_parentcount(commit);

        if (parents > 1)
            continue;
        // get the tree related to this commit
        check_lg2(git_commit_tree(&b, commit), "Get tree", NULL);
        if (parents == 1) {
            git_commit *parent;
            check_lg2(git_commit_parent(&parent, commit, 0), "Get parent", NULL);
            check_lg2(git_commit_tree(&a, parent), "Tree for parent", NULL);
            git_commit_free(parent);
        }
        // we have both tree now get the difference (this generates the stats)
        check_lg2(git_diff_tree_to_tree(
            &diff, git_commit_owner(commit), a, b, NULL),
            "Diff commit with parent", NULL);

        // get the stats itself
        check_lg2(git_diff_get_stats(&stats, diff), "generating stats for diff", NULL);

        // get the author data
        const git_signature *author = git_commit_author(commit);

        // validate we already know who is this author (must be in our std::map)
        mapit = m_commits.find (author->name);
        if (mapit == m_commits.end ()) {
            QMessageBox::critical(0, "warning!", "Author not found");
            return;
        }
        // finally update variables
        mapit->second.insertions += stats->insertions;
        mapit->second.deletions += stats->deletions;
        mapit->second.files_changed += stats->files_changed;
        // and also update global totals
        total_insertions += stats->insertions;
        total_deletions += stats->deletions;
        total_files_changed += stats->files_changed;


    }

    // now display the data
    // clear the list
    m_skip_list = true;
    while (gitList->rowCount() > 0) {
        gitList->removeRow(0);
    }
    m_skip_list = false;
    int row = 0;
    int itemidx = 1;
    // travel the std::map
    for (mapit = m_commits.begin(); mapit != m_commits.end();mapit++) {
        // insert a row in the table
        QTableWidgetItem* item;
        gitList->insertRow(row);
        gitList->setRowHeight(row, 18);
        // create the columns
        for (int idx = 0; idx < COL_GITLIST_COUNT; idx++) {
            item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
            item->setFlags(item->flags()^Qt::ItemIsEditable);
            gitList->setItem(row, idx, item);
        }
        // extract and initialize variables
        QString author = mapit->first.c_str ();
        size_t insertions = mapit->second.insertions;
        size_t deletions = mapit->second.deletions;
        size_t files_changed = mapit->second.files_changed;
        size_t commits_cnt = mapit->second.ids.size ();
        total_commits += commits_cnt;
        size_t per_changes = (((float) files_changed) / ((float)total_files_changed) * 100.0f);

        // set the columns in the table
        gitList->item (row, COL_GITLIST_ITEM)->setText(QString::number(itemidx));
        gitList->item (row, COL_GITLIST_AUTHOR)->setText(author);
        gitList->item (row, COL_GITLIST_COMMITS)->setText(QString::number(commits_cnt));
        gitList->item (row, COL_GITLIST_INSERTIONS)->setText(QString::number(insertions));
        gitList->item (row, COL_GITLIST_DELETIONS)->setText(QString::number(deletions));
        gitList->item (row, COL_GITLIST_PER_CHANGES)->setText(QString::number(per_changes));
        row++;
        itemidx++;
    }

}

// display the totals listing
void StatsHandler::totals ()
{
    // clear the list
    m_skip_list = true;
    while (gitList->rowCount() > 0) {
        gitList->removeRow(0);
    }
    m_skip_list = false;
    int row = 0;
    // prepare the table, headers
    prepareGitListTotals();
    QTableWidgetItem* item;
    // insert a row
    gitList->insertRow(row);
    gitList->setRowHeight(row, 18);
    // create the columns
    for (int idx = 0; idx < COL_GITLIST_COUNT; idx++) {
        item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        gitList->setItem(row, idx, item);
    }
    // set the values in each column
    gitList->item (row, COL_GITLIST_ITEM)->setText(QString::number(1));
    gitList->item (row, COL_GITLIST_COMMITS)->setText(QString::number(total_commits));
    gitList->item (row, COL_GITLIST_INSERTIONS)->setText(QString::number(total_insertions));
    gitList->item (row, COL_GITLIST_DELETIONS)->setText(QString::number(total_deletions));
}

// save to csv the data
void StatsHandler::save ()
{
    QString file = QFileDialog::getSaveFileName(this, "Save File" ,"", tr("CSV Files (*.csv)"));
    if(file.isEmpty()){
        return;
    }

    QString textData;

    // get number of cols and rows
    int rows = gitList->rowCount();
    int columns = gitList->columnCount();

    // extract the data and store in the textData string
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            textData += gitList->item(i,j)->text();
            textData += ", ";      // for .csv file format
        }
        textData += "\n";             // (optional: for new line segmentation)
    }
    // [Save to file] (header file <QFile> needed)
    // .csv
    file = file + ".csv";
    QFile csvFile(file);
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&csvFile);
        out << textData;
        csvFile.close();
        // dialog for success
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Notification");
        msgBox.setText("File has been succssfully saved!");

        msgBox.setIcon(QMessageBox::Information);
        msgBox.addButton(tr("Continue"), QMessageBox::ActionRole);
        msgBox.exec();

    }
}

    INSTALL_TAB(StatsHandler, "Statistics");
}
