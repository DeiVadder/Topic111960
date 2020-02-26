#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "OperationStuff.h"
#include "setup.h"

#include <QSerialPort>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->SetupButton, &QPushButton::clicked, this, &MainWindow::onSetupClicked);
    connect(ui->StartButton, &QPushButton::toggled, this, &MainWindow::toggleSerial);

    serial = new QSerialPort(this);
    serial->setPortName("ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::serialReceived);

}

MainWindow::~MainWindow()
{
    delete ui;
    if(setup)
        setup->deleteLater();
}

void MainWindow::toggleSerial(bool checked)
{
    if (!checked)
    {
        serial->close();
        emit connectStateChanged(true);
        ui->ConnectState->setText("Disconnected");
        ui->StartButton->setChecked(false);
        ui->ConnectState->setStyleSheet(QString("QPushButton {background-color: rgb(115, 210, 22);}"));
    }
    else
    {
        emit connectStateChanged(false);
        ui->ConnectState->setText("Connected");
        ui->StartButton->setChecked(true);
        ui->ConnectState->setStyleSheet(QString("QPushButton {background-color: red;}"));

        if( serial->open(QIODevice::ReadWrite))
        {
            qDebug()<<"Port Open OK";
            connect(serial, &QSerialPort::readyRead, this, &MainWindow::serialReceived);
        }
        else
        {
            qDebug()<<"Can't Open Port : " << serial->error();
        }
        serialReceived();
    }
}

void MainWindow::serialReceived()
{
    QByteArray serialData;
        serialData = serial->readAll();
        QString static receivedData = QString::fromStdString(serialData.toStdString());

        char t0[512];
        static char t1[512];
        static int wp=0;
        unsigned int i=0;

        QString filename="/home/user/Desktop/Programs/MainCBOT2/entrydata.txt";
        QFile file( filename );

        if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            QTextStream stream( &file );
            strcpy(t0,serialData);
            for (i=0;i<strlen(t0);i++)
            {
                t1[wp++]=t0[i];
                wp%=512;
                t1[wp]=0;

                if(t0[i] == '\n')
                {
                    wp=0;
                    if(checksum_gps(t1))
                    {
                        if(memcmp("$GPRMC",t1,6)==0)
                        {
                            unsigned int latcnt1=0, log_cnt1=0,spdcnt=0, cogcnt=0;
                            char cog[20];
                            char spd[20];
                            char lat1[20], lg1[20];
                            for(i=20;i<28;i++)
                            {
                                lat1[latcnt1++]=t1[i];
                            }
                            float j=atof(lat1);
                            float k=(int)(j/100);
                            float minlat  = j - ( k * 100 );
                            float ddeglat = k + ( minlat / 60 );
                            ui->latitude->setText(QString::number(ddeglat,'f'));

                            for(i=33;i<42;i++)
                            {
                                lg1[log_cnt1++]=t1[i];
                            }
                            float p=atof(lg1);
                            float q=(int)(p/100);
                            float minlong  = p - ( q * 100 );
                            float ddeglong = q + ( minlong / 60 );
                            ui->longitude->setText(QString::number(ddeglong,'f'));

                            for (i=45;i<50;i++)
                            {
                                spd[spdcnt++]=t1[i];
                            }
                            float r=atof(spd);
                            ui->speed->setText(QString::number(r,'f',4));

                            for (i=50;i<56;i++)
                            {
                                cog[cogcnt++]=t1[i];
                            }
                            float u=atof((cog));
                            float w=u*0.514444;
                            ui->cog->setText(QString::number(w,'f',4));

                            stream<<strcat(t1,timestamp());
                            ui->rece->setText(t1);
                            emit newSerialData(t1);
                            fflush(stdout);
                        }
                    }
                }
            }
            file.close();
        }
        ui->timest->setText(timestamp());
}

void MainWindow::onSetupClicked()
{
    if(!setup){
        setup = new Setup();
        connect(this, &MainWindow::connectStateChanged, setup, &Setup::onConnectStateChanged);
        connect(this, &MainWindow::newSerialData, setup, &Setup::onNewSerialData);

        connect(setup, &Setup::onOffSerialPort, this, &MainWindow::toggleSerial);
        setup->resize(size());
        setup->onConnectStateChanged(ui->StartButton->isChecked());
    }
    setup->show();
}




