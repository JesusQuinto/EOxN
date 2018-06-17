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

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

     setWindowTitle("Productor");
    setWindowIcon(QIcon(":/icons/zodiac_logo.png"));


    QGridLayout* mainGrid = new QGridLayout;
    QVBoxLayout* topLayout = new QVBoxLayout;
    QVBoxLayout* MedLayout = new QVBoxLayout;

    // create the Zodiac graph
        zodiac::Scene* zodiacScene = new zodiac::Scene(this);
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
        m_mainSplitter->setSizes({100, 900});

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
        connect(saveAction, SIGNAL(triggered()), m_mainCtrl, SLOT(createDefaultNode()));

        topLayout->addWidget(mainToolBar);
        MedLayout->addWidget(m_mainSplitter);
        mainGrid->addLayout(topLayout,0,0);
        mainGrid->addLayout(MedLayout,1,0);
         setLayout(mainGrid);

         zodiacScene->updateStyle();
         zodiacView->updateStyle();
}

Dialog::~Dialog()
{
    delete ui;
}
