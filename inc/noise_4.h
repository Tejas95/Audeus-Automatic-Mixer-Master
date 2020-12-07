#ifndef NOISE_4_H
#define NOISE_4_H

#include <QDialog>
#include "audiowrapper.h"
#include "Audeus.h"

namespace Ui {
class Noise_4;
}

class Noise_4 : public QDialog
{
    Q_OBJECT

public:
    explicit Noise_4(QWidget *parent = nullptr);
    ~Noise_4();

    PaStream *stream;


private slots:
    void on_PBSkip_clicked();

    void on_PBStart_clicked();

    void on_PBNext_clicked();

public slots:
    void onNoise4IsComplete(QString);

private:
    Ui::Noise_4 *ui;

};

#endif // NOISE_4_H
