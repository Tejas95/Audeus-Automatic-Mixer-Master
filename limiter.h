#ifndef LIMITER_H
#define LIMITER_H

#include <QObject>
#include <math.h>
#include "Definitions.h"
#include "Loudness.h"

using namespace std;

class Limiter : public QObject
{
    Q_OBJECT
public:
    explicit Limiter(QObject *parent = nullptr);

    void compute_limiter(float** input, float* loudness_ref, float* threshold, float* gain, float* gainToBe);

    float* loudness_current;
    float* gainToBe;

    CLoudness cloudness1;

signals:
    void on_gain(float *numberGain);
    void on_loudness(float *numberLoudness);

public slots:
};

#endif // LIMITER_H
