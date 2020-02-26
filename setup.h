#ifndef SETUP_H
#define SETUP_H

#include <QWidget>

namespace Ui {
class Setup;
}

class Setup : public QWidget
{
    Q_OBJECT

public:
    explicit Setup(QWidget *parent = nullptr);
    ~Setup();

public slots:
     void onNewSerialData(QString data);
     void onConnectStateChanged(bool connected);

signals:
      void onOffSerialPort(bool checked);


private:
    Ui::Setup *ui;
};

#endif // SETUP_H
