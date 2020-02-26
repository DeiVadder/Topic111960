#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Setup;
class QSerialPort;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newSerialData(QString data);
    void connectStateChanged(bool connected);

public slots:
    void toggleSerial(bool checked);

private slots:
    void serialReceived();
    void onSetupClicked();

private:
    Ui::MainWindow *ui;

    QSerialPort *serial = nullptr;
    Setup *setup = nullptr;
};
#endif // MAINWINDOW_H
