#pragma once

#include <QDialog>

class QTabWidget;
class QDialogButtonBox;

class TabDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TabDialog(QWidget *parent = 0);

private:
	QTabWidget *_tabWidget;
	QDialogButtonBox *buttonBox;
};
