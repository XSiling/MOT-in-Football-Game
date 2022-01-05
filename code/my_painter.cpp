#include "my_painter.h"


my_painter::my_painter(QWidget *parent):QLabel(parent)
{
    lx = nullptr;
    ly = nullptr;
    rx = nullptr;
    ry = nullptr;
    lnum = 0;
    rnum = 0;
    my_pix = new QPixmap("background.jpg");
    this->setScaledContents(true);
    this->setPixmap(*my_pix);
    this->show();

}

my_painter::~my_painter()
{


}

void my_painter::timer1_slot(double *lx_,double *ly_, double *rx_,double *ry_,int ln,int rn)
{
    lx = lx_;
    ly = ly_;
    rx = rx_;
    ry = ry_;
    lnum = ln;
    rnum = rn;
    update();//update the painter with paintevent
}

void my_painter::trans_to_2D(int frame_class, double t,double l,double *newx,double* newy)
{
    double tmp;
    //double *xy;
    //xy = new double[2];
    double x,y;
    if (frame_class==0){//left side
        tmp=(-0.0003*t-0.0039*l+1);
        x=(-0.3649*t-2.4796*l+881.0767)/tmp;
        y=(0.0017*t-2.4417*l+664.2869)/tmp;
    }
    else {
        tmp=0.0006*t-0.0058*l+1;
        x=(-0.0244*t-3.7265*l+638.8885)/tmp;
        y=(0.297*t-4.4644*l+1317.7)/tmp;
    }
    *newx = x;
    *newy = y;
}

void my_painter::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap("background.jpg"));
    p.setPen(QPen(Qt::red,2));
    double neww,newh;
    for (int i=0;i<lnum;++i)
    {
        trans_to_2D(0,lx[i],ly[i],&neww,&newh);
        p.drawEllipse(neww/1280*this->width(),newh/720*this->height(),10,10);
        //qDebug()<<neww<<newh;

    }
    for (int i=0;i<rnum;++i)
    {
        trans_to_2D(1,rx[i],ry[i],&neww,&newh);
        p.drawEllipse(neww/1280*this->width(),newh/720*this->height(),10,10);
        //qDebug()<<neww<<newh;
    }
    //p.drawEllipse(50,150,15,15);
    this->show();
}
