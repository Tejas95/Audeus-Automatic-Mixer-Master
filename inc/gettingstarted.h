#ifndef GETTINGSTARTED_H
#define GETTINGSTARTED_H

#include <QDialog>

namespace Ui {
class GettingStarted;
}

class GettingStarted : public QDialog
{
    Q_OBJECT

public:
    explicit GettingStarted(QWidget *parent = nullptr);
    ~GettingStarted();

private slots:
    void on_PBNext_clicked();

private:
    Ui::GettingStarted *ui;
};

#endif // GETTINGSTARTED_H
