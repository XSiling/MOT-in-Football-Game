#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "edit_2.h"
#include "edit_dia_1.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QtGlobal>
#include <math.h>
#include "my_painter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class FootballPlayer
{
public:
    int player_name;//the own id of a player
    QVector<int> frame_class;//0 for left, 1 for right
    QVector<int> player_id_in_output;//the player_id in output.txt
    double player_speed=0.0;
};

class LoadHelpPlayer
{
public:
    int frame_id;
    int frame_class;
    int player_id;
    LoadHelpPlayer(int x,int y,int z)
    {
        frame_id = x;
        frame_class = y;
        player_id = z;
    }
    ~LoadHelpPlayer()
    {

    }
};

class Frame
{
public:
    int frame_id;
    QVector<int> player_ids;
    //t l w h;
    QVector<double>t;
    QVector<double>l;
    QVector<double>w;
    QVector<double>h;
public:
    Frame(int frame_id_,QVector<int>player_ids_,QVector<double>t_,QVector<double>l_,QVector<double>w_,QVector<double>h_)
    {
        frame_id = frame_id_;
        player_ids = player_ids_;
        t = t_;
        l = l_;
        w = w_;
        h = h_;
    }

    Frame()
    {

    }

    ~Frame()
    {

    }
};


class Widget : public QWidget
{
    Q_OBJECT
private:
    bool compare_num_pool(QVector<int> v1, QVector<int> v2);
    bool check_region(int f_class, double t, double l, double w, double h);
    double calculate_speed(int player_id,int frame_id);
    double calculate_global_speed(int player_id,int frame_id,double *total_distance,double *highest_speed);
    double point_distance(double x_,double y_,double x,double y);
    void trans_to_2D(int,double,double,double*,double*);
    void process_heatmap_file();
    int player_choice;
    int current_frame;
    bool loaded;
    QPixmap *pixmap1,*pixmap2;
    QTimer *timer,*timer1;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QMediaPlayer *left_player = new QMediaPlayer(this);
    QMediaPlayer *right_player = new QMediaPlayer(this);


    void load();
    void video_play();

signals:
    void timer1_signal(double*,double*,double*,double*,int,int);
    void begin_heatmap();
    void begin_roadheatmap();
    void stop_maptimer();
    void continue_maptimer();
    void heatmap_signal(double*,double*,double*,double*,int,int);

private slots:
    void on_edit1_clicked();
    void on_edit_2_clicked();
    void start_load();
    void onTimerOut();
    void onTimer1Out();
    void LoadPlayer();
    void fresh_information();//fresh the information on the right widget;
    void set_player_choice();
    void completechangeslot(int,int,int,int);

private:
    Ui::Widget *ui;
    edit_dia_1 * edit_dia1;
    edit_2 * edit_dia2;
private:
    QString left_movie_path;
    QString right_movie_path;
    QVector<QString> left_data;
    QVector<QString> right_data;
    QVector<Frame> left_frame;
    QVector<Frame> right_frame;
    QVideoWidget *left_play_widget,*right_play_widget;
    FootballPlayer *my_players;
    int Frame_num;

};
#endif // WIDGET_H
