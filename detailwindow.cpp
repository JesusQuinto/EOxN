#include <QToolBar>
#include <QVBoxLayout>

#include "detailwindow.h"

detailwindow::detailwindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Productor");
    setWindowIcon(QIcon(":/icons/zodiac_logo.png"));

       QVBoxLayout *mainLayout = new QVBoxLayout(this);

       QToolBar *toolBar = new QToolBar();
       mainLayout->addWidget(toolBar);
}
