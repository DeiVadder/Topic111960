#include "setup.h"
#include "ui_setup.h"

Setup::Setup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup)
{
    ui->setupUi(this);

    connect(ui->Connect, &QPushButton::toggled, this, &Setup::onOffSerialPort);
}

Setup::~Setup()
{
    delete ui;
}

void Setup::onNewSerialData(QString data)
{
    ui->data->setText(data);
}

void Setup::onConnectStateChanged(bool connected)
{
    ui->state->setText(connected ? "Connected" : "Disconnected");
}
