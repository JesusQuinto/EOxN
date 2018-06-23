#include "gooddialog.h"
#include "ui_gooddialog.h"

GoodDialog::GoodDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoodDialog)
{
    ui->setupUi(this);
    setWindowTitle("Bien");

}

GoodDialog::~GoodDialog()
{
    delete ui;
}
