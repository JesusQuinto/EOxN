#ifndef FILE_H
#define FILE_H

#include "zodiacgraph/nodehandle.h"
#include "zodiacgraph/scenehandle.h"
#include "mainctrl.h"

#include <QObject>


namespace zodiac {
    class Scene;
}

class File : public QObject
{
    Q_OBJECT
public:
    File(QObject *parent = nullptr, MainCtrl * mainCtr =nullptr);

private:
    MainCtrl* m_mainCtrl;

signals:

public slots:
    void saveAs(QString fileName = QString::null);
    void open(QString fileName = QString::null);
    void  load(QString fileName);
    void save(QString fileName);
};

#endif // FILE_H
