#include "gitpp7.h"
#include <QGridLayout>
#include <QTableWidget>
#include <QStringList>
#include <QPushButton>
#include <QLabel>
#include <QWidgetItem>

namespace display{

  class displayCommit : public QWidget{
    Q_OBJECT

    public:
    displayCommit();

    private:
      QGridLayout *display;
      QTableWidget* commits = new QTableWidget();
      QStringList Header;
      QPushButton *refreshButton;
      QLabel *label;
      QLabel *label2;

    private slots:
      void refresh();
  };
}
