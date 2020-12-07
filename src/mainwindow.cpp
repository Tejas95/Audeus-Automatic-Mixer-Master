#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gettingstarted.h"
#include "audiowrapper.h"

extern CAudeus AUD;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gainLevelMeter1 = 0;
    gainLevelMeter2 = 0;

    //Wallpaper
    QPixmap bkgnd("/home/pi/Desktop/Audeus GUI Master/Audeus/DarkBlueWallpaper.jpg");
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    //Channel Label Colors
    ui->Ch1Label->setStyleSheet("QLabel { color : white; }");
    ui->Ch2Label->setStyleSheet("QLabel { color : white; }");
    ui->Ch3Label->setStyleSheet("QLabel { color : white; }");
    ui->Ch4Label->setStyleSheet("QLabel { color : white; }");
    ui->Ch5Label->setStyleSheet("QLabel { color : white; }");
    ui->Ch6Label->setStyleSheet("QLabel { color : white; }");
    ui->MasterLabel->setStyleSheet("QLabel { color : white; }");



    //Added Gain Label Colors
    ui->label_9->setStyleSheet("QLabel { color : grey; }");
    ui->label_12->setStyleSheet("QLabel { color : grey; }");
    ui->label_14->setStyleSheet("QLabel { color : grey; }");
    ui->label_16->setStyleSheet("QLabel { color : grey; }");
    ui->label_18->setStyleSheet("QLabel { color : grey; }");
    ui->label_20->setStyleSheet("QLabel { color : grey; }");

    //Slider Loudness Color
    ui->label_64->setStyleSheet("QLabel { color : grey; }");
    ui->label_65->setStyleSheet("QLabel { color : grey; }");
    //ui->label_66->setStyleSheet("QLabel { color : grey; }");
    ui->label_67->setStyleSheet("QLabel { color : grey; }");
    ui->label_68->setStyleSheet("QLabel { color : grey; }");
    ui->label_69->setStyleSheet("QLabel { color : grey; }");

    ui->label_34->setStyleSheet("QLabel { color : grey; }");
    //ui->label_35->setStyleSheet("QLabel { color : grey; }");
    ui->label_36->setStyleSheet("QLabel { color : grey; }");
    ui->label_37->setStyleSheet("QLabel { color : grey; }");
    ui->label_38->setStyleSheet("QLabel { color : grey; }");
    ui->label_39->setStyleSheet("QLabel { color : grey; }");

    ui->label_40->setStyleSheet("QLabel { color : grey; }");
    ui->label_41->setStyleSheet("QLabel { color : grey; }");
    ui->label_42->setStyleSheet("QLabel { color : grey; }");
    //ui->label_43->setStyleSheet("QLabel { color : grey; }");
    ui->label_44->setStyleSheet("QLabel { color : grey; }");
    ui->label_45->setStyleSheet("QLabel { color : grey; }");

    ui->label_46->setStyleSheet("QLabel { color : grey; }");
    ui->label_47->setStyleSheet("QLabel { color : grey; }");
    ui->label_48->setStyleSheet("QLabel { color : grey; }");
    //ui->label_49->setStyleSheet("QLabel { color : grey; }");
    ui->label_50->setStyleSheet("QLabel { color : grey; }");
    ui->label_51->setStyleSheet("QLabel { color : grey; }");

    ui->label_52->setStyleSheet("QLabel { color : grey; }");
    ui->label_53->setStyleSheet("QLabel { color : grey; }");
    ui->label_54->setStyleSheet("QLabel { color : grey; }");
    //ui->label_55->setStyleSheet("QLabel { color : grey; }");
    ui->label_56->setStyleSheet("QLabel { color : grey; }");
    ui->label_57->setStyleSheet("QLabel { color : grey; }");

    ui->label_58->setStyleSheet("QLabel { color : grey; }");
    ui->label_59->setStyleSheet("QLabel { color : grey; }");
    ui->label_60->setStyleSheet("QLabel { color : grey; }");
    //ui->label_61->setStyleSheet("QLabel { color : grey; }");
    ui->label_62->setStyleSheet("QLabel { color : grey; }");
    ui->label_63->setStyleSheet("QLabel { color : grey; }");
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_PBAutoSetGain_clicked()
{
    GettingStarted noiseIntro;
    noiseIntro.setModal(true);
    noiseIntro.exec();
}


void MainWindow::on_PBStartAudio_clicked()
{
    QFuture<void> err1 = QtConcurrent::run(&this->ap, &audioWrapper::startAudio);

    connect(&LM, &LevelMeter::on_gain, this, &MainWindow::gainNumber);
    QFuture<void> err2 = QtConcurrent::run(&this->LM, &LevelMeter::start);
}

void MainWindow::gainNumber(float *number)
{
    qDebug() << "BOO";
    gainLevelMeter1 = number[0];
    gainLevelMeter2 = number[1];

    update();
}


void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter levelM1(this);
    QPainter levelM2(this);
    QPainter levelM3(this);
    QPainter levelM4(this);
    QPainter levelM5(this);
    QPainter levelM6(this);
    QPainter levelMeterMaster(this);

    QRect lm1(430, 405, 30, 400);
    QRect lm2(630, 405, 30, 400);
    QRect lm3(830, 405, 30, 400);
    QRect lm4(1030, 405, 30, 400);
    QRect lm5(1230, 405, 30, 400);
    QRect lm6(1430, 405, 30, 400);
    QRect lmMaster(1630, 510, 25, 400);


    levelM1.drawRect(lm1);
    levelM2.drawRect(lm2);
    levelM3.drawRect(lm3);
    levelM4.drawRect(lm4);
    levelM5.drawRect(lm5);
    levelM6.drawRect(lm6);
    levelMeterMaster.drawRect(lmMaster);

    levelM1.fillRect(lm1, Qt::white);
    levelM2.fillRect(lm2, Qt::white);
    levelM3.fillRect(lm3, Qt::white);
    levelM4.fillRect(lm4, Qt::white);
    levelM5.fillRect(lm5, Qt::white);
    levelM6.fillRect(lm6, Qt::white);
    levelMeterMaster.fillRect(lmMaster, Qt::white);

    QBrush fill1(Qt::darkGreen, Qt::SolidPattern);
    QBrush fill2(Qt::darkGreen, Qt::SolidPattern);
    QBrush fill3(Qt::darkGreen, Qt::SolidPattern);
    QBrush fill4(Qt::darkGreen, Qt::SolidPattern);
    QBrush fill5(Qt::darkGreen, Qt::SolidPattern);
    QBrush fill6(Qt::darkGreen, Qt::SolidPattern);
    QBrush fillMaster(Qt::darkGreen, Qt::SolidPattern);

    //connect(&Clim, &Limiter::on_gain, this, &MainWindow::gainNumber);

    levelM1.fillRect(430, 805, 30, -(100*gainLevelMeter1), fill1);
    levelM2.fillRect(630, 805, 30, -(100*gainLevelMeter2), fill2);
//    levelMeter3.fillRect(430, 805, 30, -30, fill3);
//    levelMeter4.fillRect(430, 805, 30, -30, fill4);
//    levelMeter5.fillRect(430, 805, 30, -30, fill5);
//    levelMeter6.fillRect(430, 805, 30, -30, fill6);
    //    levelMeterMaster.fillRect(430, 805, 30, -30, fillMaster);
}
