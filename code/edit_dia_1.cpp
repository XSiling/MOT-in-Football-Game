#include "edit_dia_1.h"
#include "ui_edit_dia_1.h"

edit_dia_1::edit_dia_1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_dia_1)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotButtonA()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slotButtonB()));
}

edit_dia_1::~edit_dia_1()
{
    delete ui;
}

void edit_dia_1::slotButtonB()
{
    //click cancel
    emit completechange(1,0,0,0);
    return;
}


void edit_dia_1::slotButtonA()
{
    //click the ok, then try to get the information of other box.
    //get the information first
    QString newid,name,lor;
    int newid_int,name_int,lor_int;
    newid = ui->textEdit->toPlainText();
    name = ui->textEdit_2->toPlainText();
    lor = ui->textEdit_3->toPlainText();


    newid_int = newid.toInt();
    name_int = name.toInt();
    lor_int = lor.toInt();
    if(name_int<=0 || name_int>12)
    {
        QMessageBox::information(NULL,"ERROR","You can only input name between 1 and 12!");
        emit completechange(1,0,0,0);
        return;
    }

    if (lor_int!=0 && lor_int!=1)
    {
        QMessageBox::information(NULL,"ERROR","You can only input frame class with 0 or 1!");
        emit completechange(1,0,0,0);
        return;
    }
    //then I can set the change, wow such a big work....
    emit completechange(0,name_int-1,lor_int,newid_int);

}
