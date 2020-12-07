#ifndef NOISE_5_H
#define NOISE_5_H

#include <QDialog>
#include "audiowrapper.h"
#include "Audeus.h"

namespace Ui {
class Noise_5;
}

class Noise_5 : public QDialog
{
    Q_OBJECT

public:
    explicit Noise_5(QWidget *parent = nullptr);
    ~Noise_5();

    PaStream *stream;

private slots:
    void on_PBSkip_clicked();

    void on_PBStart_clicked();

    void on_PBNext_clicked();

public slots:
    void onNoise5IsComplete(QString);

private:
    Ui::Noise_5 *ui;

};

#endif // NOISE_5_H
