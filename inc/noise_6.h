#ifndef NOISE_6_H
#define NOISE_6_H

#include <QDialog>
#include "audiowrapper.h"
#include "Audeus.h"

namespace Ui {
class Noise_6;
}

class Noise_6 : public QDialog
{
    Q_OBJECT

public:
    explicit Noise_6(QWidget *parent = nullptr);
    ~Noise_6();

    PaStream *stream;

private slots:
    void on_PBSkip_clicked();

    void on_PBStart_clicked();

    void on_PBNext_clicked();

public slots:
    void onNoise6IsComplete(QString);

private:
    Ui::Noise_6 *ui;

};

#endif // NOISE_6_H
