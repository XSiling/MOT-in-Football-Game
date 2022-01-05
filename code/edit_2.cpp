#include "edit_2.h"
#include "ui_edit_2.h"

edit_2::edit_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_2)
{
    ui->setupUi(this);
}

edit_2::~edit_2()
{
    delete ui;
}
