#include <QWidget>

class QPushButton;

class BranchHandler : public QWidget{
    Q_OBJECT

  public:
    BranchHandler();

  private slots:
    void on_pushButton_clicked();
};
