#ifndef EDIT_2_H
#define EDIT_2_H

#include <QDialog>

namespace Ui {
class edit_2;
}

class edit_2 : public QDialog
{
    Q_OBJECT

public:
    explicit edit_2(QWidget *parent = nullptr);
    ~edit_2();

private:
    Ui::edit_2 *ui;
};

#endif // EDIT_2_H
