#ifndef MY_PAINTER_H
#define MY_PAINTER_H
#include <QWidget>
#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class my_painter; }
QT_END_NAMESPACE

class my_painter: public QLabel
{
    Q_OBJECT
private:
    double *lx,*ly,*rx,*ry;
    int lnum,rnum;
    QPixmap *my_pix;
    void paintEvent(QPaintEvent *);
    void trans_to_2D(int c,double x,double y,double *newx,double *newy);
public slots:
    void timer1_slot(double*,double *,double*,double*,int,int);
public:
    my_painter(QWidget *parent = nullptr);
    ~my_painter();
};




#endif // MY_PAINTER_H
