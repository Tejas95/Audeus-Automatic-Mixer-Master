#pragma once

#ifndef AUDIOWRAPPER_H
#define AUDIOWRAPPER_H

#include <QObject>
#include "Audeus.h"
#include <QThread>
class audioWrapper : public QObject
{
    Q_OBJECT

public:
    explicit audioWrapper(QObject *parent = 0);

    int startAudioFreeze();
    int startAudioNoise();
    int startAudioInput();
    int startAudio();

    bool next;

    PaStream *stream;


signals:
    void on_gain(float *numberGain);
    void on_loudness(float *numberLoudness);

    //void on_noise1(QString status);
//    void noiseIsComplete(QString);
//    void noise2IsComplete(QString);
//    void noise3IsComplete(QString);
//    void noise4IsComplete(QString);
//    void noise5IsComplete(QString);
//    void noise6IsComplete(QString);
//    void inputLoudnessIsComplete(QString);

public slots:
    void stopAudio();

private:
  bool mStopAudio;

};

#endif // AUDIOWRAPPER_H
