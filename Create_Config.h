#include <QWidget>

class QPushButton;

class HelloWorldLabel : public QWidget{
    Q_OBJECT

  public:
    HelloWorldLabel();

  private slots:
    void on_pushButton_clicked();
};
