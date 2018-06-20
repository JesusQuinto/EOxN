#ifndef VALIDATIOR_H
#define VALIDATIOR_H

#include "mainctrl.h"
#include <QObject>

class Validator : public QObject
{
    Q_OBJECT
public:
    explicit Validator(QObject *parent = nullptr, MainCtrl * mainCtr =nullptr);

    MainCtrl* m_mainCtrl;

    bool hasCicle();
    void DFS();

signals:

public slots:
};

#endif // VALIDATIOR_H
