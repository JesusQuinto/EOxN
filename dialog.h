#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainctrl.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    MainCtrl* m_mainCtrl;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
