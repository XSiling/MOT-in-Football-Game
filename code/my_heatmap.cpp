#include "my_heatmap.h"
#include <QPainter>
#include <QRadialGradient>
#include <QImage>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include "heatmapper.h"
#include "gradientpalette.h"
#include <QDir>
#include <QString>
#include <QFile>
#include <QDebug>
QStringList strlist_road;
int num_road=0;

my_heatmap::my_heatmap(QWidget *parent)
    : QWidget(parent)
{
    palette_ = new GradientPalette(DEFAULT_WIDTH);
    palette_->setColorAt(0.45, Qt::blue);
    palette_->setColorAt(0.55, Qt::cyan);
    palette_->setColorAt(0.85, Qt::yellow);
    palette_->setColorAt(0.95, Qt::magenta);
    palette_->setColorAt(1.0, Qt::red);

    canvas_ = new QImage(CANVAS_WIDTH, CANVAS_HEIGHT, QImage::Format_ARGB32);
    canvas_->fill(QColor(0, 0, 0, 0));
    mapper_ = new HeatMapper(canvas_, palette_, DEFAULT_RADIUS, DEFAULT_OPACITY);
    this->setFixedSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    QFile file("D:/PROGRAM_QT/Football_ui(3)/Football_ui/heatmap_origin.txt");
    //QTextStream shuru(&file);
    if (file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"openok";
        while (!file.atEnd())
        {
        QString line=file.readLine();
        //QStringList str=line.split(',');
        //double a=strlist_road[0].toDouble();
        strlist_road.append(line.split(','));
        }

    file.close();
    }

    //QTimer *timer=new QTimer();
    //timer->setInterval(4);//每隔n 加载一次1ms-18s,4ms-65s;
    //timer->start();
    //connect(timer,SIGNAL(timeout()),this,SLOT(fresh()));

}

my_heatmap::~my_heatmap()
{
    delete mapper_;
    delete palette_;
}

void my_heatmap::stoptimer()
{
    if (timer)
    {
        //timer->stop();
    }
}

void my_heatmap::continuetimer()
{
    if (timer)
    {
        timer->start();
    }
}

void my_heatmap::begintimer()
{
    timer = new QTimer();
    timer->setInterval(4);
    timer->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(useless()));
}

/*
 * 绘图方法，当窗口刷新时该方法会被调用
 */
void my_heatmap::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap("background.jpg"));
    painter.drawImage(0, 0, *canvas_);
}

/*
 * 该方法用于向绘图对象添加一个数据点
 */
void my_heatmap::mouseReleaseEvent(QMouseEvent *e)
{
    if (NULL == mapper_)
        return;

    mapper_->addPoint(e->x(), e->y());
    this->update();
}

void my_heatmap::freshpoint(int x,int y)
{
    if (NULL == mapper_)
        return;
    //mapper_->addPoint(100,100);
    mapper_->addPoint(x,y);
    this->update();
}

void my_heatmap::fresh(double* x,double*y,double*x1,double*y1,int n,int n1)
{
    double neww,newh;
    for (int i=0;i<n;++i)
    {
        trans_to_2D(0,x[i],y[i],&neww,&newh);
        //p.drawEllipse(neww/1280*this->width(),newh/720*this->height(),10,10);

        mapper_->addPoint(neww/1280*this->width(),newh/720*this->height());
    }
    for (int i=0;i<n1;++i)
    {
        trans_to_2D(1,x1[i],y1[i],&neww,&newh);
        mapper_->addPoint(neww/1280*this->width(),newh/720*this->height());
    }
    this->update();
    //mapper_->addPoint(x[i]/1280*368,y[i]/720*207);
    //this->update();
}

void my_heatmap::trans_to_2D(int frame_class, double t,double l,double *newx,double* newy)
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

void my_heatmap::useless()
{
//    //每秒读取30帧
//    framenum_road+=30;//当前第几帧
//    //int playernum_road=leftframe[frame].player_ids.length;
//    int num_road=0;//当前
//    double position[playernum_road+1][3];//多设置一位
//    QFie file(QDir::currentPath()+"/heatmap.txt");
    //    QTextStream f(&file);
    //    while(num_road<30)
    //    {
    //        QString str=file.readLine();//保存一行
    //        QStringList strlist_road=str.split(",");
    //        for (int k=0;k<30;++k)
    //            for (int j=0;j<2;++j)
    //            {
    //                position[k][j]=strlist_road[n].toDouble();
    //                n++;//leftframe数组是每一帧,leftframe[0]是第一帧，读leftframe[0]中playerids长度得到球员数
    //            }
    //    }
    //    QFile file(QDir::currentPath()+"/heatmap.txt");
    //    QTextStream shuru(&file);
    //    int m=0;
    //    while ((m*2)!=num_road)
    //    {

    //    }
    //    while (!shuru.atEnd())
    //    {
    //        QString line=shuru.readLine();
    //        //QStringList str=line.split(',');
    //        //double a=strlist_road[0].toDouble();
    //        strlist_road.append(line.split(','));
    //    }
    //    file.close();l
    /*if (num_road==32588)
    {
       return;
    }
    double x=200,y=200;
    x=strlist_road[num_road].toDouble();
    y=strlist_road[num_road+1].toDouble();
    qDebug()<<x<<y;
    num_road+=2;
    mapper_->addPoint(x/1280*368,y/720*207);
    this->update();*/
}
