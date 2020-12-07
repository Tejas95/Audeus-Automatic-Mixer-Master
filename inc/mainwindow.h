#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>
#include <qline.h>
#include <qbuttongroup.h>
#include <Audeus.h>
#include <QtConcurrent>
#include <QtGui>
#include <QtCore>
#include <QDialog>
#include "levelmeter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     PaStream *stream;
     audioWrapper ap;
     LevelMeter LM;

     void paintEvent(QPaintEvent *e);
     float gainLevelMeter1;
     float gainLevelMeter2;


private slots:

    void on_PBAutoSetGain_clicked();

    void on_PBStartAudio_clicked();

public slots:
    void gainNumber(float *number);

private:
    Ui::MainWindow *ui;



protected:


};

#endif // MAINWINDOW_H
