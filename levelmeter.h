#pragma once
#ifndef LEVELMETER_H
#define LEVELMETER_H

#include <QObject>
#include "Audeus.h"

class LevelMeter : public QObject
{
    Q_OBJECT
public:
    explicit LevelMeter(QObject *parent = nullptr);

    void start();

signals:
    void on_gain(float *numberGain);
    void on_loudness(float *numberLoudness);

public slots:
};

#endif // LEVELMETER_H
