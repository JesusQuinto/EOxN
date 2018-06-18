#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainctrl.h"
#include "zodiacgraph/scene.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    QString name;
    explicit Dialog(QWidget *parent = 0, QString name = "default");
    void read(MainCtrl *);
    ~Dialog();

public slots:
    void save();

private:
    MainCtrl* m_mainCtrl;
    zodiac::Scene* zodiacScene;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
