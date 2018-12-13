#include <QWidget>
#include <QListWidget>


class QPushButton;

class BranchHandler : public QWidget{
    Q_OBJECT

    public:
        BranchHandler();
        int branchIndex=0;
        std::string lastMyDirStr;



    private slots:
        int repoCheck();
        void doBranch();
        int doRefresh(bool);
        void doRefreshBtn();
        void populateList();
    private:
        QListWidget *listWidget;
};
