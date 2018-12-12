#include "gitpp7.h"
#include <QGridLayout>
#include <QTableWidget>
#include <QStringList>

namespace display{

  class displayCommit : public QWidget{
    Q_OBJECT

    public:
    displayCommit();

    private:
      QGridLayout *display;
      QTableWidget* commits = NULL;
      QStringList Header;
  };
}
