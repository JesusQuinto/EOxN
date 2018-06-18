#include "dialog.h"
#include "ui_dialog.h"

#include "nodectrl.h"
#include "mainctrl.h"
#include "propertyeditor.h"
#include "zodiacgraph/baseedge.h"
#include "zodiacgraph/edgearrow.h"
#include "zodiacgraph/edgelabel.h"
#include "zodiacgraph/node.h"
#include "zodiacgraph/nodelabel.h"
#include "zodiacgraph/perimeter.h"
#include "zodiacgraph/plug.h"
#include "zodiacgraph/pluglabel.h"
#include "zodiacgraph/scene.h"
#include "zodiacgraph/view.h"

#include <QSplitter>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QToolBar>
#include <QString>
#include <QMap>
#include <QFile>
#include <QDebug>

Dialog::Dialog(QWidget *parent, QString name) :
    QDialog(parent),
    ui(new Ui::Dialog)
{


     this->name = name;

     setWindowTitle("Productor: "+ this->name);
    setWindowIcon(QIcon(":/icons/zodiac_logo.png"));

     this->setGeometry(0,0,800,500);

    QGridLayout* mainGrid = new QGridLayout;
    QVBoxLayout* topLayout = new QVBoxLayout;
    QVBoxLayout* MedLayout = new QVBoxLayout;

    // create the Zodiac graph
        zodiacScene = new zodiac::Scene(this);
        zodiac::View* zodiacView = new zodiac::View(this);
        zodiacView->setScene(zodiacScene);

        // create the Property Editor
        PropertyEditor* propertyEditor = new PropertyEditor(this,false);

        // create the Main Controller
        m_mainCtrl = new MainCtrl(this, zodiacScene, propertyEditor);

        // setup the main splitter
       QSplitter* m_mainSplitter = new QSplitter(Qt::Horizontal, this);
        m_mainSplitter->addWidget(propertyEditor);
        m_mainSplitter->addWidget(zodiacView);
        m_mainSplitter->setSizes({200, 800});

        // create the main toolbar
        QToolBar* mainToolBar = new QToolBar(this);
        mainToolBar->setStyleSheet("QToolBar {border: 0px;}");
        mainToolBar->setIconSize(QSize(12,12));
        mainToolBar->setMovable(false);
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        // create global actions
        QAction* newNodeAction = new QAction(QIcon(":/icons/plus.svg"), tr("&Agg Proceso"), this);
        newNodeAction->setShortcuts(QKeySequence::New);
        newNodeAction->setStatusTip(tr("Crear un nuevo Proceso"));
        mainToolBar->addAction(newNodeAction);
        connect(newNodeAction, SIGNAL(triggered()), m_mainCtrl, SLOT(createDefaultNode()));

         QAction* saveAction = new QAction(QIcon(":/icons/plus.svg"), tr("&Guardar"), this);
        saveAction->setShortcuts(QKeySequence::New);
        saveAction->setStatusTip(tr("Guardar Cambios"));
        mainToolBar->addAction(saveAction);
        connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

        topLayout->addWidget(mainToolBar);
        MedLayout->addWidget(m_mainSplitter);
        mainGrid->addLayout(topLayout,0,0);
        mainGrid->addLayout(MedLayout,1,0);
         setLayout(mainGrid);

         zodiacScene->updateStyle();
         zodiacView->updateStyle();
         read(m_mainCtrl);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::save()
{
    QList<QPair<QString,QPair<QPoint,QList<QPair<QString,bool> >>>> nodeList;
    QList<QPair<QPair<QString,QString>,QPair<QString,QString>>> edgeList;
    QList<zodiac::PlugHandle> HList_this, HList_other;
    QList<QPair<QString,bool> > plugList;
    QList<zodiac::Node*>lstNodes = zodiacScene->getNodes();
     zodiac::NodeHandle* nodeHandle;

    for (auto node: lstNodes)
    {
        nodeHandle = new zodiac::NodeHandle(node);

        HList_this = nodeHandle->getPlugs();

        for(auto item: HList_this)
        {
                plugList.append(qMakePair(item.getName(),item.isOutgoing()));
                HList_other =  item.getConnectedPlugs();

                for(auto item2: HList_other)
                {
                    if(item.isOutgoing())
                    {
                        edgeList.append(
                                    qMakePair(
                                        qMakePair(nodeHandle->getName(),item.getName()),
                                        qMakePair(item2.getNode().getName(),item2.getName())
                                        )
                                    );
                    }

                 }
        }

        nodeList.append(
                    qMakePair(
                        nodeHandle->getName(),
                        qMakePair(
                               nodeHandle->getPos().toPoint(),
                                plugList)
                        )
                    );
    }

    QFile file("./productor_"+this->name+".txt");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open file";
        return;
    }

    qDebug() <<" nodeList" << nodeList;
    qDebug() <<" edgeList" << edgeList;

    QDataStream out (&file);
    out.setVersion(QDataStream::Qt_5_10);
    out <<  nodeList << edgeList;

    file.flush();
    file.close();

    accept();
}


void Dialog::read(MainCtrl *mainCtrl)
{
    QList<QPair<QString,QPair<QPoint,QList<QPair<QString,bool> >>>> nodeList;
   QList<QPair<QPair<QString,QString>,QPair<QString,QString>>> edgeList;

    QFile file("./productor_"+ this->name+".txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open file";
        return;
    }

    QDataStream in (&file);

    in.setVersion(QDataStream::Qt_5_10);

    in  >> nodeList >> edgeList;

    file.close();


    qDebug() <<" nodeList" << nodeList;
    qDebug() <<" edgeList" << edgeList;

    QMap<QString, NodeCtrl*> mapNodes;


    for(auto node: nodeList)
    {
         mapNodes[node.first] = mainCtrl->createNode(node.first);
         mapNodes[node.first]
                 ->getNodeHandle()
                 .setPos(
                     node.second.first.rx(),
                     node.second.first.ry()
                     );

         for(auto plug:node.second.second)
         {
             if(plug.second) mapNodes[node.first]->addOutgoingPlug(plug.first);
             else mapNodes[node.first]->addIncomingPlug(plug.first);
         }

    }

    for(auto edge: edgeList)
    {
        mapNodes[edge.first.first]
                ->getNodeHandle().getPlug(edge.first.second)
                .connectPlug(mapNodes[edge.second.first]
                ->getNodeHandle().getPlug(edge.second.second));
    }

}

