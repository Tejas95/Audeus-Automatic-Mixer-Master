#include "levelmeter.h"
#include <QThread>

extern CAudeus AUD;

LevelMeter::LevelMeter(QObject *parent) : QObject(parent)
{
//    connect(&lim, &Limiter::on_gain, &MW, &MainWindow::gainNumber);
}

void LevelMeter::start()
{
    for (int i = 0; i < 999999999; i++)
    {
        emit on_gain(AUD.gain);
        QThread::currentThread()->msleep(2000);
    }
}
