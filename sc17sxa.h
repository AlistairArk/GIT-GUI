#include <QWidget>
#include <QLineEdit>

class QPushButton;

namespace config{
class GitConfig : public QWidget{
    Q_OBJECT

  public:
    GitConfig();

  private:
    QLineEdit *lnEditEmail;
    QLineEdit *lnEditUsername;
    QPushButton *setConfig;
    //std::string myDirStr;
  private slots:
    void on_browse_clicked();
    void on_setConfig_clicked();
};
}
