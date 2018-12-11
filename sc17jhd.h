#include <QWidget>
#include <QListWidget>


class QPushButton;

class BranchHandler : public QWidget{
    Q_OBJECT

    public:
        BranchHandler();
        int branchIndex=0;

    private slots:
        void on_pushButton_clicked();
        void populateList();
    private:
        QListWidget *listWidget;
};
