#include "dialog.h"
#include "ui_dialog.h"

#include "nodectrl.h"
#include "mainctrl.h"
#include "file.h"
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
#include <QGridLayout>
#include <QTextEdit>
#include <QToolBar>
#include <QString>
#include <QDebug>

Dialog::Dialog(QWidget *parent, QString _pName, QString _pId) :
    QDialog(parent),
    ui(new Ui::Dialog),
    pName(_pName),
    pId(_pId)
{

    setWindowTitle("Productor: "+ _pName);
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
        m_mainCtrl = new MainCtrl(this, zodiacScene, propertyEditor,true);

        //create the File Manager
        file = new File(this,m_mainCtrl);

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
        connect(newNodeAction, SIGNAL(triggered()), m_mainCtrl, SLOT(createProcessNode()));

        QAction* newBienAction = new QAction(QIcon(":/icons/plus.svg"), tr("&Agg bien"), this);
        newBienAction->setShortcuts(QKeySequence::New);
        newBienAction->setStatusTip(tr("Crear un nuevo bien"));
        mainToolBar->addAction(newBienAction);
        connect(newBienAction, SIGNAL(triggered()), m_mainCtrl, SLOT(createDefaultNode()));

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
         read();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::save()
{
    file->save(pName+"_pro_"+pId+".exn");
    accept();
}


void Dialog::read()
{
     file->load(pName+"_pro_"+pId+".exn");
}

