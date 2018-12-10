#include "gitpp7.h"

#include <QLineEdit>
#include <QString>
#include <QGridLayout>

class SearchTab : public QWidget{
  Q_OBJECT

  public:
  SearchTab();

  private:
    QGridLayout *Search;
    QLineEdit *keyword;

  private slots:
    void SearchClicked();
};
