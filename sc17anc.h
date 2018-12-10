#include "gitpp7.h"
#include <QLineEdit>
#include <QTableWidget>
#include <QGridLayout>
#include <QStringList>

namespace search{

class SearchTab : public QWidget{
  Q_OBJECT

  public:
  SearchTab();

  private:
    QGridLayout *Search;
    QLineEdit *keyword;
    QTableWidget* Commits = NULL;
    QStringList Header;

  private slots:
    void SearchClicked();
};
}
