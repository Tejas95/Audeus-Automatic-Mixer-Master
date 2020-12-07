#ifndef NOISE_3_H
#define NOISE_3_H

#include <QDialog>
#include "audiowrapper.h"
#include "Audeus.h"

namespace Ui {
class Noise_3;
}

class Noise_3 : public QDialog
{
    Q_OBJECT

public:
    explicit Noise_3(QWidget *parent = nullptr);
    ~Noise_3();

    PaStream *stream;


private slots:
    void on_PBSkip_clicked();

    void on_PBStart_clicked();

    void on_PBNext_clicked();

public slots:
    void onNoise3IsComplete(QString);

private:
    Ui::Noise_3 *ui;

};

#endif // NOISE_3_H
