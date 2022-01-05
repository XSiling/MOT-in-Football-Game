#ifndef EDIT_DIA_1_H
#define EDIT_DIA_1_H

#include <QDialog>
#include <QMessageBox>
namespace Ui {
class edit_dia_1;
}

class edit_dia_1 : public QDialog
{
    Q_OBJECT

public:
    explicit edit_dia_1(QWidget *parent = nullptr);
    ~edit_dia_1();
private slots:
    void slotButtonA();
    void slotButtonB();
private:
    Ui::edit_dia_1 *ui;
signals:
    void completechange(int completeclass,int,int,int);
};

#endif // EDIT_DIA_1_H
