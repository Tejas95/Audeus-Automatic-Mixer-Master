#ifndef NOISE_1_H
#define NOISE_1_H

#include <QDialog>
#include "audiowrapper.h"
#include "Audeus.h"

namespace Ui {
class Noise_1;
}

class Noise_1 : public QDialog
{
    Q_OBJECT

public:
    explicit Noise_1(QWidget *parent = nullptr);
    ~Noise_1();

    PaStream *stream;

private slots:
    void on_PBSkip_clicked();

    void on_PBStart_clicked();

    void on_PBNext_clicked();

public slots:
    void onNoise1IsComplete(QString);

private:
    Ui::Noise_1 *ui;


signals:
    void onStop();
};

#endif // NOISE_1_H
