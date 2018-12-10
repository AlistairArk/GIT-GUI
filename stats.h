#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <vector>
#include <map>
#include <string>

class QLabel;
class QWidget;

struct CommitData {
	size_t insertions;
	size_t deletions;
	size_t files_changed;
	std::vector<std::string>    ids;
};

class MainWindow: public QMainWindow {
	Q_OBJECT

public:
	MainWindow();

private slots:
	void            summary();
	void            totals();
	void            save();

private:
	void createWindowContent();
	void createMenuAndStatusBar();

private: // widgets needed
	QLabel*			_label;
	QWidget*		_mainWidget;
	QTableWidget*	gitList;
	void            prepareGitList();
	void            prepareGitListTotals();
	void            repoInit();
	void            loadData();
	bool            m_skip_list;    // avoid updates to the list
	std::map<std::string, CommitData>     m_commits;
	size_t          total_insertions;
	size_t          total_deletions;
	size_t          total_files_changed;
	size_t          total_commits;
	int COL_GITLIST_COUNT;
	int COL_GITLIST_ITEM;
	int COL_GITLIST_AUTHOR;
	int COL_GITLIST_COMMITS;
	int COL_GITLIST_INSERTIONS;
	int COL_GITLIST_DELETIONS;
	int COL_GITLIST_PER_CHANGES;
};
