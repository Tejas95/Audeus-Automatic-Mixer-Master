#include "limiter.h"
#include <iostream>
#include "Audeus.h"
#include "mainwindow.h"

using namespace std;

extern CAudeus AUD;

Limiter::Limiter(QObject *parent) : QObject(parent)
{
    loudness_current = new float[NUM_CHANNELS];
    gainToBe = new float[NUM_CHANNELS];

    memset(loudness_current, 0, sizeof(float) * NUM_CHANNELS);
    memset(gainToBe, 0, sizeof(float) * NUM_CHANNELS);
}


void Limiter::compute_limiter(float** input, float* loudness_ref, float* threshold, float* gain, float* gainToBe)
{
    cloudness1.compute_loudness(input, loudness_current);
    qDebug() << loudness_current[0];

    //Gain Calculation for limiter
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        if (loudness_current[i] > threshold[i])
        {
            gainToBe[i] = loudness_ref[i] - loudness_current[i];
            gainToBe[i] = pow(10, gainToBe[i] / 20);
            qDebug() << "TRIGGER";
        }
        else
            gainToBe[i] = gain[i];

        //emit on_gain(gainToBe);
    }


    //emit on_loudness(loudness_current);
}
