#ifndef VALIDATIOR_H
#define VALIDATIOR_H

#include "mainctrl.h"
#include  <QMap>
#include <QObject>
#include <QString>

class Validator : public QObject
{
    Q_OBJECT
public:
    explicit Validator(
            QObject *parent = nullptr,
            MainCtrl * mainCtr =nullptr);

    MainCtrl* m_mainCtrl;
    QMap<QString,bool> visitados;
    int amountNodes;
    int nroNodos;


    void hasCicle();
    void _dfs(zodiac::NodeHandle node);

signals:

public slots:
};

#endif // VALIDATIOR_H
