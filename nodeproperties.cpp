#include "nodeproperties.h"
#include "mainwindow.h"
#include "dialog.h"
#include "gooddialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QDialog>

#include "collapsible.h"
#include "nodectrl.h"

QString NodeProperties::s_defaultPlugName = "plug";

NodeProperties::NodeProperties(NodeCtrl *node, Collapsible *parent, bool isRed = false)
    : QWidget(parent)
    , m_node(node)
    , m_nextPlugIsIncoming(true)
{
    // define the main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(2,2,2,2);
    mainLayout->setSpacing(2);
    setLayout(mainLayout);

    // update the title of the collapsible container
    parent->updateTitle(m_node->getName());

    // define the name edit
    QHBoxLayout* nameLayout = new QHBoxLayout();
    m_nameEdit = new QLineEdit(m_node->getName(), this);
    connect(m_nameEdit, SIGNAL(editingFinished()), this, SLOT(renameNode()));
    nameLayout->addWidget(new QLabel("Nombre", this));
    nameLayout->addWidget(m_nameEdit);
    nameLayout->setContentsMargins(0, 4, 0, 0);
    mainLayout->addLayout(nameLayout);

        // define the add plug button
        m_plugLayout = new QGridLayout();
        m_plugLayout->setContentsMargins(0, 8, 0, 0);   // leave space between the plug list and the name
        m_plugLayout->setColumnStretch(1,1); // so the add-plug button always stays on the far right
        m_addPlugButton = new QPushButton(this);
        m_addPlugButton->setIconSize(QSize(8, 8));
        m_addPlugButton->setIcon(QIcon(":/icons/plus.svg"));
        m_addPlugButton->setFlat(true);
        m_plugLayout->addWidget(new QLabel("Arco", this), 0, 0, 1, 2, Qt::AlignLeft);
        m_plugLayout->addWidget(m_addPlugButton, 0, 2);
        connect(m_addPlugButton, SIGNAL(pressed()), this, SLOT(createNewPlug()));

    // define the plugs
    for(zodiac::PlugHandle& plug : m_node->getPlugHandles()){
        addPlugRow(plug);
    }
    mainLayout->addLayout(m_plugLayout);

        // define the detail button
        m_detailLayout = new QGridLayout();
        m_detailLayout->setContentsMargins(0, 8, 0, 0);   // leave space between the plug list and the name
        m_detailLayout->setColumnStretch(1,1); // so the add-plug button always stays on the far right

    if (isRed){
        m_detaillButton = new QPushButton("Detalle",this);
        m_detaillButton->setFlat(true);
        m_detailLayout->addWidget(m_detaillButton, 0, 0);
        connect(m_detaillButton, SIGNAL(pressed()), this, SLOT(opendetail()));
     }
    else{
        m_detaillButton = new QPushButton("Propiedades",this);
        m_detaillButton->setFlat(true);
        m_detailLayout->addWidget(m_detaillButton, 0, 0);
        connect(m_detaillButton, SIGNAL(pressed()), this, SLOT(properties()));
    }

    QPushButton * m_removeButton = new QPushButton("Eliminar",this);
    m_removeButton->setFlat(true);
    m_detailLayout->addWidget(m_removeButton, 0, 2);
    connect(m_removeButton, SIGNAL(pressed()), this, SLOT(removeNode()));

    mainLayout->addLayout(m_detailLayout);
}

void NodeProperties::removeNode(){
   m_node->remove();
}

void NodeProperties::opendetail()
{
    Dialog  detail( this,
                m_node->getName(),
                m_node->getNodeHandle().getId().toString());

    if(bool value = detail.exec())
    {
        qDebug() << value;
    }
}

void NodeProperties::properties()
{

    GoodDialog good(this);

    if(bool value = good.exec())
    {
        qDebug() << value;
    }
}

void NodeProperties::renameNode()
{
    QString newName = m_nameEdit->text();
    if(m_node->getName() == newName){
        return;
    }
    m_node->rename(newName);
    qobject_cast<Collapsible*>(parent())->updateTitle(newName);
}

void NodeProperties::createNewPlug()
{
    // duplicate plug names are automatically resolved by the zodiac::Node
    if(m_nextPlugIsIncoming){
        addPlugRow(m_node->addIncomingPlug(s_defaultPlugName));
    } else {
        addPlugRow(m_node->addOutgoingPlug(s_defaultPlugName));
    }
    m_nextPlugIsIncoming = !m_nextPlugIsIncoming;
}

void NodeProperties::addPlugRow(zodiac::PlugHandle plug)
{
    int row = m_plugLayout->rowCount();

    QPushButton* directionButton = new QPushButton(this);
    directionButton->setIconSize(QSize(16, 16));
    directionButton->setFlat(true);
    directionButton->setStatusTip("Toggle the direction of the Plug from 'incoming' to 'outoing' and vice versa.");
    m_plugLayout->addWidget(directionButton, row, 0);

    QLineEdit* plugNameEdit = new QLineEdit(plug.getName(), this);
    m_plugLayout->addWidget(plugNameEdit, row, 1);

    QPushButton* removalButton = new QPushButton(this);
    removalButton->setIcon(QIcon(":/icons/minus.svg"));
    removalButton->setIconSize(QSize(8, 8));
    removalButton->setFlat(true);
    removalButton->setStatusTip("Delete the Plug from its Node");
    m_plugLayout->addWidget(removalButton, row, 2);

    m_plugRows.insert(plug.getName(), new PlugRow(this, plug, plugNameEdit, directionButton, removalButton));
}

void NodeProperties::removePlugRow(const QString& plugName)
{
    Q_ASSERT(m_plugRows.contains(plugName));
    m_plugRows.remove(plugName);
}


PlugRow::PlugRow(NodeProperties* editor, zodiac::PlugHandle plug,
                 QLineEdit* nameEdit, QPushButton* directionToggle, QPushButton* removalButton)
    : QObject(editor)
    , m_editor(editor)
    , m_plug(plug)
    , m_nameEdit(nameEdit)
    , m_directionToggle(directionToggle)
    , m_removalButton(removalButton)
{
    connect(m_nameEdit, SIGNAL(editingFinished()), this, SLOT(renamePlug()));
    connect(m_directionToggle, SIGNAL(clicked()), this, SLOT(togglePlugDirection()));
    connect(m_removalButton, SIGNAL(clicked()), this, SLOT(removePlug()));

    updateDirectionIcon();
}

void PlugRow::renamePlug()
{
    m_nameEdit->setText(m_editor->getNode()->renamePlug(m_plug.getName(), m_nameEdit->text()));
}


void PlugRow::updateDirectionIcon()
{
    if(m_plug.isIncoming()){
        m_directionToggle->setIcon(QIcon(":/icons/incoming.svg"));
    } else {
        m_directionToggle->setIcon(QIcon(":/icons/outgoing.svg"));
    }
}

void PlugRow::togglePlugDirection()
{
    if(!m_editor->getNode()->togglePlugDirection(m_plug.getName())){
        return;
    }
    updateDirectionIcon();
}

void PlugRow::removePlug()
{
    // do nothing, if the plug cannot be removed
    if(!m_plug.isRemovable()){
        return;
    }

    // unregister from the editor
    m_editor->removePlugRow(m_plug.getName());

    // remove widgets from the editor
    QGridLayout* plugLayout = m_editor->getPlugLayout();
    plugLayout->removeWidget(m_directionToggle);
    plugLayout->removeWidget(m_nameEdit);
    plugLayout->removeWidget(m_removalButton);

    // delete the widgets, they are no longer needed
    m_directionToggle->deleteLater();
    m_nameEdit->deleteLater();
    m_removalButton->deleteLater();

    // finally, remove the plug from the logical node
    m_editor->getNode()->removePlug(m_plug.getName());
}
