#include <QWidget>
#include <QListWidget>

class QPushButton;

class BranchHandler : public QWidget{
    Q_OBJECT

  public:
    BranchHandler();

  private slots:
    void on_pushButton_clicked();
  private:
    QListWidget *listWidget;
    int choice=0; // Set master branch as default
};
