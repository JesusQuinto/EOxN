#include "detailwindow.h"

#include <QToolBar>
#include <QVBoxLayout>
#include <QSplitter>

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

detailwindow::detailwindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Productor");
    setWindowIcon(QIcon(":/icons/zodiac_logo.png"));

    // create the main toolbar
    QToolBar* mainToolBar = new QToolBar(this);
    mainToolBar->setStyleSheet("QToolBar {border: 0px;}");
    mainToolBar->setIconSize(QSize(12,12));
    mainToolBar->setMovable(false);
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // create the Zodiac graph
    zodiac::Scene* zodiacScene = new zodiac::Scene(this);
    zodiac::View* zodiacView = new zodiac::View(this);
    zodiacView->setScene(zodiacScene);

    // create the Property Editor
    PropertyEditor* propertyEditor = new PropertyEditor(this);

    // create the Main Controller
    MainCtrl *m_mainCtrl = new MainCtrl(this, zodiacScene, propertyEditor);

    // setup the main splitter
    QSplitter *m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(propertyEditor);
    m_mainSplitter->addWidget(zodiacView);
    m_mainSplitter->setSizes({100, 900});

    // create global actions
    QAction *newNodeAction = new QAction(QIcon(":/icons/plus.svg"), tr("&Add Node"), this);
    newNodeAction->setShortcuts(QKeySequence::New);
    newNodeAction->setStatusTip(tr("Create a new Node"));
    mainToolBar->addAction(newNodeAction);
    connect(newNodeAction, SIGNAL(triggered()), m_mainCtrl, SLOT(createDefaultNode()));

    QWidget* emptySpacer = new QWidget();
    emptySpacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    mainToolBar->addWidget(emptySpacer);

    // initialize the GUI
    zodiacScene->updateStyle();
    zodiacView->updateStyle();
}