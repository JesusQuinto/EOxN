#ifndef DIALOG_H
#define DIALOG_H

#include "mainctrl.h"
#include "file.h"
#include "zodiacgraph/scene.h"

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
     Dialog(
            QWidget *parent = 0,
            QString _pName =QString::null,
            QString _pId = QString::null
            );

    QString pName;
    QString pId;

    void read();

    ~Dialog();

public slots:
    void save();

private:
    Ui::Dialog *ui;

    MainCtrl* m_mainCtrl;
    File* file;
    zodiac::Scene* zodiacScene;
};

#endif // DIALOG_H
