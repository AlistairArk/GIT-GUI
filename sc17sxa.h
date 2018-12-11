#include <QWidget>
#include <QLineEdit>

class QPushButton;

namespace config{
class HelloWorldLabel : public QWidget{
    Q_OBJECT

  public:
    HelloWorldLabel();

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
