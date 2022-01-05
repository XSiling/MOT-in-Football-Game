#ifndef MY_HEATMAP_H
#define MY_HEATMAP_H
#include <QWidget>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

class HeatMapper;
class GradientPalette;

class my_heatmap : public QWidget
{
    Q_OBJECT
public:
    enum {
        CANVAS_WIDTH = 1280,//分辨率
        CANVAS_HEIGHT = 720,
        DEFAULT_RADIUS = 10,
        DEFAULT_OPACITY = 128,
        DEFAULT_WIDTH = 255
    };

    explicit my_heatmap(QWidget *parent = 0);
    void trans_to_2D(int frame_class, double t,double l,double *newx,double* newy);
    virtual ~my_heatmap();
protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    // 绘图对象指针
    HeatMapper *mapper_;
    QTimer *timer;
    // 用于显示输出的图像
    QImage *canvas_;
    // 调色板
    GradientPalette *palette_;
public slots:
    void freshpoint(int x,int y);
    void fresh(double*,double*,double*,double*,int,int);
    void begintimer();
    void stoptimer();
    void continuetimer();
    void useless();
};
#endif // MY_HEATMAP_H
