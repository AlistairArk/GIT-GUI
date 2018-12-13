#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <vector>
#include <map>
#include <string>

class QLabel;
class QWidget;

namespace stats{

// store the totals by each author
struct CommitData {
	size_t insertions;
	size_t deletions;
	size_t files_changed;
	std::vector<std::string>    ids;
};

class StatsHandler: public QWidget {
	Q_OBJECT

public:
	StatsHandler();

private slots:
	void            summary();
	void            totals();
	void            save();

private: // widgets needed
	QLabel*			_label;
	QWidget*		_mainWidget;
	QTableWidget*	gitList;
	void            prepareGitList();
	void            prepareGitListTotals();
	void            repoInit();
	int 			repoCheck();
	void            loadData();
	bool            m_skip_list;    // avoid updates to the list
    // std::map to store the totals by author
	std::map<std::string, CommitData>     m_commits;
	size_t          total_insertions;
	size_t          total_deletions;
	size_t          total_files_changed;
	size_t          total_commits;
    // number of columns of the listing and variables to store
	int COL_GITLIST_COUNT;
	int COL_GITLIST_ITEM;
	int COL_GITLIST_AUTHOR;
	int COL_GITLIST_COMMITS;
	int COL_GITLIST_INSERTIONS;
	int COL_GITLIST_DELETIONS;
	int COL_GITLIST_PER_CHANGES;
};
}
