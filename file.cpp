#include "file.h"
#include "zodiacgraph/node.h"
#include  "zodiacgraph/scene.h"
#include "nodectrl.h"
#include "mainctrl.h"

#include <QString>
#include <QPoint>
#include <QMap>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

File::File(QObject *parent, MainCtrl *mainCtr)
    : QObject(parent),
    m_mainCtrl(mainCtr)
{
}

void File::saveAs(QString fileName)
{
    if(fileName.isNull())
        fileName = "newGraph.exn";

    QString name = QFileDialog::getSaveFileName(nullptr,
           tr("Guardar Grafo"), fileName,
           tr("Address Book (*.exn);;All Files (*)"));

    if (name.isEmpty())
        return;

    save(name);
}

void File::open(QString fileName)
{
    if (fileName.isNull())
        fileName = "./";

    QString name = QFileDialog::getOpenFileName(nullptr,
          tr("Abrir Grafo"), fileName,
          tr("Address Grapth (*.exn);;All Files (*)"));

    if (name.isEmpty())
        return;

    load(name);
}

void File::save(QString fileName)
{
     QMap<QString,QPair<QString,QPoint>> nodeMap;
     QMap<QString,QList<QPair<QString,QPair<QString,QString>>>> nodePlugMap;
     QMap<QString, QPair<qint8,QPair<qint8,QString>>> nodeDataMap;

     QList<zodiac::NodeHandle>nodesList = m_mainCtrl->getSocialScene()->getNodes();
     QList<zodiac::PlugHandle> plugHandleList, plugHandleList_other;
     QList<QPair<QString,QPair<QString,QString>>> plugList;

     zodiac::PlugHandle otherPlug;

     for(auto  node: nodesList )
    {
                plugHandleList = node.getPlugs() ;
                plugList.clear();

                nodeMap[node.getId().toString()]
                        =  qMakePair(
                                node.getName(),
                                node.getPos().toPoint()
                             );

                for (auto plug: plugHandleList)
                {
                    if(!plug.getConnectedPlugs().empty())
                    {
                        if(plug.isOutgoing())
                        {
                                otherPlug
                                        = plug.getConnectedPlugs().first();

                                plugList.append(
                                            qMakePair(
                                                plug.getName(),
                                                qMakePair(
                                                    otherPlug.getNode().getId().toString(),
                                                    otherPlug.getName()
                                                    ))
                                            );
                        }
                        else
                        {
                            plugList.append(
                                        qMakePair(
                                            plug.getName(),
                                            qMakePair(
                                                QString::null,
                                                QString::null
                                                ))
                                        );
                        }
                    }

                }

                nodePlugMap[node.getId().toString()]
                        = plugList;
                
                if(m_mainCtrl->getIsProducer())
                {
                    //nodeDataMap[node.getId().toString()]
                }

    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open file";
        return;
    }

    QDataStream out (&file);
    out.setVersion(QDataStream::Qt_5_10);

    qDebug() << "nodeMap" << nodeMap;
        qDebug() << "nodePlugMap" << nodePlugMap;
            qDebug() << "nodeDataMap" << nodeDataMap;

   if (m_mainCtrl->getIsProducer())
       out <<  nodeMap << nodePlugMap << nodeDataMap;
   else
       out <<  nodeMap << nodePlugMap;

    file.flush();
    file.close();

}

void File::load(QString fileName)
{
    QMap<QString,QPair<QString,QPoint>> nodeMap;
    QMap<QString,QList<QPair<QString,QPair<QString,QString>>>> nodePlugMap;
    QMap<QString, QPair<qint8,QPair<qint8,QString>>> nodeDataMap;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open file";
        return;
    }

    QDataStream in (&file);
    in.setVersion(QDataStream::Qt_5_10);

    if (m_mainCtrl->getIsProducer())
        in  >> nodeMap >> nodePlugMap >> nodeDataMap;
    else
        in  >> nodeMap >> nodePlugMap;

    qDebug() << "\n\n\n\n";
    qDebug() << "nodeMap" << nodeMap;
        qDebug() << "nodePlugMap" << nodePlugMap;
            qDebug() << "nodeDataMap" << nodeDataMap;

    file.close();

    QMap<QString, NodeCtrl*> mapNodes;

    QMap<QString,QPair<QString,QPoint>>::iterator i;
    for (i = nodeMap.begin();  i  !=  nodeMap.end(); ++i)
    {
         mapNodes[i.key()] = m_mainCtrl->createNode(i.value().first);
         mapNodes[i.key()] ->getNodeHandle()
                 .setPos(
                     i.value().second.rx(),
                     i.value().second.ry()
                     );
    }

    QMap<QString,QList<QPair<QString,QPair<QString,QString>>>>::iterator j;
    for (j = nodePlugMap.begin();  j  !=  nodePlugMap.end(); ++j)
    {
        for(auto plug:j.value())
        {
            if(!plug.second.first.isNull())
            {
                mapNodes[j.key()]
                        ->addOutgoingPlug(plug.first);

                mapNodes[plug.second.first]
                        ->addIncomingPlug(plug.second.second);

                mapNodes[j.key()]
                        ->getNodeHandle().getPlug(plug.first)
                        .connectPlug(mapNodes[plug.second.first]
                        ->getNodeHandle().getPlug(plug.second.second));
            }

        }
    }

}
