
#include "widget.h"
#include "ui_widget.h"
#include "edit_dia_1.h"
#include "edit_2.h"
#include <QPushButton>
#include <QTimer>

QTimer * timer;
int maxValue = 67;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->slider_progress->setEnabled(false);
    ui->progressBar->setVisible(false);
    //ui->groupBox->setStyleSheet("QGroupBox{border:none}");
    //ui->groupBox->setTitle("");

    //默认选择player_1
    ui->player_1->setChecked(true);

    left_play_widget = new QVideoWidget();
    right_play_widget = new QVideoWidget();
    //设置视频大小位置
    left_play_widget->setFixedSize(400,230);
    right_play_widget->setFixedSize(400,230);
    left_play_widget->move(80,120);
    right_play_widget->move(480,120);
    left_play_widget->setParent(this);
    right_play_widget->setParent(this);


    //设置变量值
    player_choice=1;
    current_frame=0;
    loaded = false;

    /*
    //set the background picture
    pixmap1 = new QPixmap("background.jpg");
    //QPixmap pixmap1("background.jpg");
    //pixmap1->scaled(ui->football_2D->size(), Qt::KeepAspectRatio);
    ui->football_2D->setScaledContents(true);
    ui->football_2D->setPixmap(*pixmap1);
    ui->football_2D->show();
    */
    //pixmap2 = new QPixmap("background.jpg");
    //ui->label_12->setScaledContents(true);
    //ui->label_12->setPixmap(*pixmap2);
    //ui->label_12->show();


    //按下load开始读取
    //connect(ui->edit_1,&QPushButton::clicked,this,&Widget::on_edit1_clicked);
    //edit_1
    //connect(ui->edit_1,SIGNAL(clicked()),this,SLOT(on_edit1_clicked()));
    connect(ui->load,SIGNAL(clicked()),this,SLOT(start_load()));

    connect(ui->edit_1,&QPushButton::clicked,this,&Widget::on_edit1_clicked);
    //每一秒更新右边的信息
    //connect(left_player,&QMediaPlayer::positionChanged,this,&Widget::fresh_information);
    connect(ui->player_1,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_2,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_3,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_4,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_5,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_6,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_7,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_8,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_9,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_10,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_11,&QRadioButton::toggled,this,&Widget::set_player_choice);
    connect(ui->player_12,&QRadioButton::toggled,this,&Widget::set_player_choice);
    //the set part

    connect(this,SIGNAL(begin_heatmap()),ui->widget,SLOT(begintimer()));
    connect(this,SIGNAL(begin_roadheatmap()),ui->widget_2,SLOT(begintimer()));
    connect(this,SIGNAL(stop_maptimer()),ui->widget,SLOT(stoptimer()));
    connect(this,SIGNAL(stop_maptimer()),ui->widget_2,SLOT(stoptimer()));
    connect(this,SIGNAL(continue_maptimer()),ui->widget,SLOT(continuetimer()));
    connect(this,SIGNAL(continue_maptimer()),ui->widget_2,SLOT(continuetimer()));

    connect(this,&Widget::heatmap_signal,ui->widget,&my_heatmap::fresh);
    connect(this,&Widget::heatmap_signal,ui->widget_2,&my_roadheatmap::fresh);
    //void stop_maptimer();
    //void continue_maptimer();
    //connect()

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_edit1_clicked()
{
    //修改球员信息
    if (!loaded)
    {
        QMessageBox::information(NULL,"ERROR","You haven't loaded players' information yet!");
        return;
    }
    if (timer && loaded)
    {
        timer->stop();
    }
    if (timer1 && loaded)
    {
        timer1->stop();
    }
    emit stop_maptimer();
    edit_dia1 = new edit_dia_1(this);
    edit_dia1->setModal(true);
    left_player->pause();
    right_player->pause();
    //connect(ui->edit_dia1,)
    connect(edit_dia1,&edit_dia_1::completechange,this,&Widget::completechangeslot);
    edit_dia1->show();
}

void Widget::completechangeslot(int classification, int name,int lor,int new_id)
{

    if (!loaded)
    {
        QMessageBox::information(NULL,"ERROR","You haven't loaded players' information yet!");
        if (left_player && right_player)
        {
            left_player->play();
            right_player->play();
            if (timer)
            {
                timer->start();
            }
            if (timer1)
            {
                timer1->start();
            }
            emit continue_maptimer();
        }
        return;
    }
    if (classification==1)
    {
        //no change yet
        left_player->play();
        right_player->play();
        if (timer)
        {
            timer->start();
        }
        if (timer1)
        {
            timer1->start();
        }
        emit continue_maptimer();
        return;
    }
    else
    {
        //check whether the player is in the frame or not
        for (int i=0;i<12;++i)
        {
            if (my_players[i].frame_class[current_frame] ==lor && my_players[i].player_id_in_output[current_frame]==new_id)
            {
                QMessageBox::information(NULL,"ERROR","The information belongs to another person, please check!");
                left_player->play();
                right_player->play();
                if (timer)
                {
                    timer->start();
                }
                if (timer1)
                {
                    timer1->start();
                }
                emit continue_maptimer();
                return;
            }
        }
        //check whether there is such person in current frame
        if (lor==0)
        {
            if (left_frame[current_frame].player_ids.indexOf(new_id)==-1)
            {
                QMessageBox::information(NULL,"ERROR","The information isn't correct, please check!");
                left_player->play();
                right_player->play();
                if (timer)
                {
                    timer->start();
                }
                if(timer1)
                {
                    timer1->start();
                }
                emit continue_maptimer();
                return;
            }
        }
        else
        {
            if (right_frame[current_frame].player_ids.indexOf(new_id)==-1)
            {
                QMessageBox::information(NULL,"ERROR","The information isn't correct, please check!");
                left_player->play();
                right_player->play();
                if (timer)
                {
                    timer->start();
                }
                if (timer1)
                {
                    timer1->start();
                }
                emit continue_maptimer();
                return;
            }
        }
        int lrnum = 100;
        int m;
        bool compare1,compare2,found_after;
        QVector<int> left_occupy_id;
        QVector<int> right_occupy_id;
        for (int i=0;i<current_frame;++i)
        {
            for (int j=0;j<12;++j)
            {
                if (my_players[j].frame_class[i]==0)
                {
                    if (left_occupy_id.indexOf(my_players[j].player_id_in_output[i])==-1)
                    {
                        left_occupy_id.append(my_players[j].player_id_in_output[i]);
                    }
                }
                else
                {
                    if (right_occupy_id.indexOf(my_players[j].player_id_in_output[i])==-1)
                    {
                        right_occupy_id.append(my_players[j].player_id_in_output[i]);
                    }
                }
            }
        }

        for (int i=current_frame;i<Frame_num;++i)
        {
            if (i==current_frame)
            {
                //make change of information right now
                my_players[name].frame_class[current_frame] = lor;
                my_players[name].player_id_in_output[current_frame] = new_id;
            }
            else
            {
                //processing i-th frame!!!!!!!!!!!!!!!!!!!!!!!
                //make change of information from now on!
                //similar to load_player function
                compare1 = true;    // check whether players in the last left frame is in the next frame, all in then true
                compare2 = true;    // check whether players in the last right frame is in the next frame, all in then true
                for (int tmp=0;tmp<left_frame[i-1].player_ids.length();++tmp)
                {
                    if (left_frame[i].player_ids.indexOf(left_frame[i-1].player_ids[tmp])==-1)
                    {
                        compare1 = false;
                        break;
                    }
                }
                for (int tmp=0;tmp<right_frame[i-1].player_ids.length();++tmp)
                {
                    if (right_frame[i].player_ids.indexOf(right_frame[i-1].player_ids[tmp])==-1)
                    {
                        compare2 = false;
                        break;
                    }
                }

                if (compare1 && compare2)
                {
                    //they are the same as i-1 I think
                    for (int j=0;j<12;++j)
                    {
                        my_players[j].frame_class[i] = my_players[j].frame_class[i-1];
                        my_players[j].player_id_in_output[i] = my_players[j].player_id_in_output[i-1];
                    }
                }
                else
                {
                    for (int j=0;j<12;++j)
                    {
                        if (my_players[j].player_id_in_output[i-1]==-1)
                        {
                            my_players[j].frame_class[i] = -1;
                            my_players[j].player_id_in_output[i] = -1;
                            continue;
                        }

                        if (my_players[j].frame_class[i-1]==0)//player just in the left video
                        {
                            // player last seen in the left video
                            if (left_frame[i].player_ids.indexOf(my_players[j].player_id_in_output[i-1])!=-1)
                            {
                                //the player_id could still be seen in the left video,then the best!
                                my_players[j].frame_class[i] = 0;
                                my_players[j].player_id_in_output[i] = my_players[j].player_id_in_output[i-1];
                            }
                            else//player not being seen in the left video!search in the right video
                            {
                                //first search current right video
                                for (m=0;m<right_frame[i].player_ids.length();++m)
                                {
                                    if (right_occupy_id.indexOf(right_frame[i].player_ids[m])==-1)
                                    {
                                        //qDebug()<<"find right more!";
                                        right_occupy_id.append(right_frame[i].player_ids[m]);
                                        my_players[j].frame_class[i] = 1;
                                        my_players[j].player_id_in_output[i] = right_frame[i].player_ids[m];
                                    }
                                }
                                //search in the before and after right video
                                if (m==right_frame[i].player_ids.length())
                                {
                                    //my_players
                                    for (int k=1;k<=lrnum;++k)
                                    {
                                        found_after=false;
                                        if((i+k)<right_frame.length())
                                        {
                                            for(m=0;m<right_frame[i+k].player_ids.length();++m)
                                            {
                                                if (right_occupy_id.indexOf(right_frame[i+k].player_ids[m])==-1)
                                                {
                                                    //qDebug()<<"find right more! in right after!";
                                                    found_after=true;
                                                    right_occupy_id.append(right_frame[i+k].player_ids[m]);
                                                    my_players[j].frame_class[i] = 1;
                                                    my_players[j].player_id_in_output[i] = right_frame[i+k].player_ids[m];
                                                    break;
                                                }
                                            }
                                        }

                                        if ((i-k)>=0 && found_after==false)
                                        {
                                            for(m=0;m<right_frame[i-k].player_ids.length();++m)
                                            {
                                                if (right_occupy_id.indexOf(right_frame[i-k].player_ids[m])==-1)
                                                {
                                                    //qDebug()<<"find right more! in right before!";
                                                    right_occupy_id.append(right_frame[i-k].player_ids[m]);
                                                    my_players[j].frame_class[i] = 1;
                                                    my_players[j].player_id_in_output[i] = right_frame[i-k].player_ids[m];
                                                    break;
                                                }
                                            }
                                        }

                                    }
                                }
                                //not find either in left nor right (lost!)
                                if (my_players[j].frame_class.length()<(i+1))
                                {
                                    my_players[j].frame_class[i] = -1;
                                    my_players[j].player_id_in_output[i] = -1;
                                }
                            }

                        }
                        else//player just seen in the right video
                        {
                            // player last seen in the right video
                            if (right_frame[i].player_ids.indexOf(my_players[j].player_id_in_output[i-1])!=-1)
                            {
                                //the player_id could still be seen in the right video,then the best!
                                my_players[j].frame_class[i]=1;
                                my_players[j].player_id_in_output[i]=my_players[j].player_id_in_output[i-1];
                            }
                            else//player not being seen in the right video!search in the left video
                            {
                                    //first search current left video
                                    for (m=0;m<left_frame[i].player_ids.length();++m)
                                    {
                                        if (left_occupy_id.indexOf(left_frame[i].player_ids[m])==-1)
                                        {
                                            //qDebug()<<"find left more!";
                                            left_occupy_id.append(left_frame[i].player_ids[m]);
                                            my_players[j].frame_class[i]=0;
                                            my_players[j].player_id_in_output[i] = left_frame[i].player_ids[m];
                                        }
                                    }
                                    //search in the before and after right video
                                    if (m==left_frame[i].player_ids.length())
                                    {
                                        //my_players
                                        for (int k=1;k<=lrnum;++k)
                                        {
                                            found_after=false;
                                            if ((i+k)<left_frame.length())
                                            {
                                                for(m=0;m<left_frame[i+k].player_ids.length();++m)
                                                {
                                                    if (left_occupy_id.indexOf(left_frame[i+k].player_ids[m])==-1)
                                                    {
                                                        //qDebug()<<"find left more! in left after!";
                                                        found_after=true;
                                                        left_occupy_id.append(left_frame[i+k].player_ids[m]);
                                                        my_players[j].frame_class[i] =0;
                                                        my_players[j].player_id_in_output[i] = left_frame[i+k].player_ids[m];
                                                        break;
                                                    }
                                                }

                                            }

                                            if ((i-k)>=0 && found_after==false)
                                            {
                                                for(m=0;m<left_frame[i-k].player_ids.length();++m)
                                                {
                                                    if (left_occupy_id.indexOf(left_frame[i-k].player_ids[m])==-1)
                                                    {
                                                        //qDebug()<<"find left more! in left before!";
                                                        left_occupy_id.append(left_frame[i-k].player_ids[m]);
                                                        my_players[j].frame_class[i] =0;
                                                        my_players[j].player_id_in_output[i] = left_frame[i-k].player_ids[m];
                                                        break;
                                                    }
                                                }
                                            }

                                        }
                                    }
                                    //not find either in left nor right (lost!)
                                    if (my_players[j].frame_class.length()<(i+1))
                                    {
                                        my_players[j].frame_class[i] = -1;
                                        my_players[j].player_id_in_output[i] = -1;
                                    }
                                }
                        }
                    }
                }
            }
        }

        left_player->play();
        right_player->play();
        if (timer)
        {
            timer->start();
        }
        if (timer1)
        {
            timer1->start();
        }
        emit continue_maptimer();
        //timer->start();
        return;
    }
}

void Widget::fresh_information()
{
    // I want to fresh the information every 1 second!
    double distance;
    double hightest_speed;
    double player_realtime_speed = calculate_speed(player_choice-1,current_frame);
    double player_global_speed = calculate_global_speed(player_choice-1,current_frame,&distance,&hightest_speed);
    if (my_players[player_choice-1].player_speed<player_realtime_speed)
    {
        my_players[player_choice-1].player_speed = player_realtime_speed;
    }
    //qDebug()<<"freshing";
    //current_frame+=30;
    //information:姓名，ID，实时速度 跑动距离 最高速度 平均速度
    ui->name->setText(QString::number(player_choice));
    if (current_frame<Frame_num)
    {
        if (my_players[player_choice-1].player_id_in_output[current_frame]==-1)
        {
            ui->ID->setText("lost");
            //ui->label_19->setText("lost");
            ui->label_17->setText(QString::number(player_global_speed));// to total speed
            ui->label_20->setText("lost");
            ui->label->setText("lost");
        }
        else
        {
            ui->ID->setText(QString::number(my_players[player_choice-1].player_id_in_output[current_frame]));
            ui->label->setText(QString::number(player_realtime_speed));
            ui->label_17->setText(QString::number(player_global_speed));
            //ui->label_17->setText(QString::number(player_realtime_speed-double((qrand()%100)/100)));
            ui->label_20->setText(QString::number(player_global_speed*current_frame/30));
            ui->label_19->setText(QString::number(my_players[player_choice-1].player_speed));
            //set the team for player
            if (my_players[player_choice-1].frame_class[current_frame]==0)
            {
                ui->label_18->setText("左");
            }
            else
            {
                ui->label_18->setText("右");
            }
        }


    }
}

double Widget::calculate_global_speed(int player_id,int frame_id,double *total_distance,double *highest_speed)
{
    double total = 0.0;
    int frame_count = 0;
    double x_,x,y_,y;//the x axis and y axis of player-mid-point
    int tmp_id_in_frame;
    //double *xy,*xy1;
    double newx,newy,newx1,newy1;
    double accumulatedistance=0.0;
    int accumulated_frame_count=0;
    double existed_highestspeed=0.0;
    double tmp_high;

    for (int i=0;i<=frame_id;++i)
    {
        if ((frame_id+i)>=0 && (frame_id+i)<(Frame_num-1) && my_players[player_id].player_id_in_output[frame_id+i]!=-1 && my_players[player_id].player_id_in_output[frame_id+i+1]!=-1)
        {
            if (my_players[player_id].frame_class[frame_id+i]==0)
            {
                tmp_id_in_frame = left_frame[frame_id+i].player_ids.indexOf(my_players[player_id].player_id_in_output[frame_id+i]);
                if(tmp_id_in_frame==-1)
                    continue;
                x_ = left_frame[frame_id+i].t[tmp_id_in_frame]+left_frame[frame_id+i].w[tmp_id_in_frame]/2;
                y_ = left_frame[frame_id+i].l[tmp_id_in_frame]+left_frame[frame_id+i].h[tmp_id_in_frame]/2;
                trans_to_2D(0,x_,y_,&newx,&newy);
            }
            else
            {
                tmp_id_in_frame = right_frame[frame_id+i].player_ids.indexOf(my_players[player_id].player_id_in_output[frame_id+i]);
                if (tmp_id_in_frame==-1)
                    continue;
                x_ = right_frame[frame_id+i].t[tmp_id_in_frame]+right_frame[frame_id+i].w[tmp_id_in_frame]/2;
                y_ = right_frame[frame_id+i].l[tmp_id_in_frame]+right_frame[frame_id+i].h[tmp_id_in_frame]/2;
                //xy = trans_to_2D(1,x_,y_);
                trans_to_2D(1,x_,y_,&newx,&newy);
            }

            if (my_players[player_id].frame_class[frame_id+i+1]==0)
            {
                tmp_id_in_frame = left_frame[frame_id+i+1].player_ids.indexOf(my_players[player_id].player_id_in_output[frame_id+i+1]);
                if (tmp_id_in_frame==-1)
                    continue;
                x = left_frame[frame_id+i+1].t[tmp_id_in_frame]+left_frame[frame_id+i+1].w[tmp_id_in_frame]/2;
                y = left_frame[frame_id+i+1].l[tmp_id_in_frame]+left_frame[frame_id+i+1].h[tmp_id_in_frame]/2;
                //xy1 = trans_to_2D(0,x,y);
                trans_to_2D(0,x,y,&newx1,&newy1);
            }
            else
            {
                tmp_id_in_frame = right_frame[frame_id+i+1].player_ids.indexOf(my_players[player_id].player_id_in_output[frame_id+i+1]);
                if (tmp_id_in_frame==-1)
                    continue;
                x = right_frame[frame_id+i+1].t[tmp_id_in_frame]+right_frame[frame_id+i+1].w[tmp_id_in_frame]/2;
                y = right_frame[frame_id+i+1].l[tmp_id_in_frame]+right_frame[frame_id+i+1].h[tmp_id_in_frame]/2;
                //xy1 = trans_to_2D(1,x,y);
                trans_to_2D(1,x,y,&newx1,&newy1);
            }

            total += point_distance(newx,newy,newx1,newy1);
            accumulatedistance += point_distance(newx,newy,newx1,newy1);
            accumulated_frame_count += 1;
            frame_count += 1;
        }

        if (frame_id%30==0)
        {
            if (accumulated_frame_count==0)
            {
                tmp_high = 0.0;
            }
            else
            {
                tmp_high = 30*accumulatedistance/accumulated_frame_count;
            }
            if (tmp_high>existed_highestspeed)
            {
                existed_highestspeed = tmp_high;
            }
            accumulated_frame_count = 0;
            accumulatedistance = 0.0;
        }
    }
    *total_distance = total;
    qDebug()<<total;
    *highest_speed = existed_highestspeed;
    if (frame_count==0)
    {
        return 0;
    }
    else {
        return (30*total/frame_count);
    }
}

double Widget::calculate_speed(int player_id,int frame_id)
{
    //the calculate slot is between -30 -> +30
    double total = 0.0;
    int frame_count = 0;
    double x_,x,y_,y;//the x axis and y axis of player-mid-point
    int tmp_id_in_frame;
    //double *xy,*xy1;
    double newx,newy,newx1,newy1;
    for (int i=-30;i<=30;++i)
    {
        if ((frame_id+i)>=0 && (frame_id+i)<(Frame_num-1) && my_players[player_id].player_id_in_output[frame_id+i]!=-1 && my_players[player_id].player_id_in_output[frame_id+i+1]!=-1)
        {
            if (my_players[player_id].frame_class[frame_id+i]==0)
            {
                tmp_id_in_frame = left_frame[frame_id+i].player_ids.indexOf(my_players[player_id].player_id_in_output[frame_id+i]);
                if(tmp_id_in_frame==-1)
                    continue;
                x_ = left_frame[frame_id+i].t[tmp_id_in_frame]+left_frame[frame_id+i].w[tmp_id_in_frame]/2;
                y_ = left_frame[frame_id+i].l[tmp_id_in_frame]+left_frame[frame_id+i].h[tmp_id_in_frame]/2;
                //xy = trans_to_2D(0,x_,y_);
                trans_to_2D(0,x_,y_,&newx,&newy);

            }
            else
            {
                tmp_id_in_frame = right_frame[frame_id+i].player_ids.indexOf(my_players[player_id].player_id_in_output[frame_id+i]);
                if (tmp_id_in_frame==-1)
                    continue;
                x_ = right_frame[frame_id+i].t[tmp_id_in_frame]+right_frame[frame_id+i].w[tmp_id_in_frame]/2;
                y_ = right_frame[frame_id+i].l[tmp_id_in_frame]+right_frame[frame_id+i].h[tmp_id_in_frame]/2;
                //xy = trans_to_2D(1,x_,y_);
                trans_to_2D(1,x_,y_,&newx,&newy);
            }

            if (my_players[player_id].frame_class[frame_id+i+1]==0)
            {

                tmp_id_in_frame = left_frame[frame_id+i+1].player_ids.indexOf(my_players[player_id].player_id_in_output[frame_id+i+1]);
                if (tmp_id_in_frame==-1)
                    continue;
                x = left_frame[frame_id+i+1].t[tmp_id_in_frame]+left_frame[frame_id+i+1].w[tmp_id_in_frame]/2;
                y = left_frame[frame_id+i+1].l[tmp_id_in_frame]+left_frame[frame_id+i+1].h[tmp_id_in_frame]/2;
                //xy1 = trans_to_2D(0,x,y);
                trans_to_2D(0,x,y,&newx1,&newy1);
            }
            else
            {
                tmp_id_in_frame = right_frame[frame_id+i+1].player_ids.indexOf(my_players[player_id].player_id_in_output[frame_id+i+1]);
                if (tmp_id_in_frame==-1)
                    continue;
                x = right_frame[frame_id+i+1].t[tmp_id_in_frame]+right_frame[frame_id+i+1].w[tmp_id_in_frame]/2;
                y = right_frame[frame_id+i+1].l[tmp_id_in_frame]+right_frame[frame_id+i+1].h[tmp_id_in_frame]/2;
                //xy1 = trans_to_2D(1,x,y);
                trans_to_2D(1,x,y,&newx1,&newy1);
            }

            total += point_distance(newx,newy,newx1,newy1);
            frame_count += 1;
        }
    }
    if (frame_count==0)
    {
        return 0;
    }
    else {
        return (30*total/frame_count);
    }
}

void Widget::set_player_choice()
{
    if (ui->player_1->isChecked())
    {
        player_choice=1;
        return;
    }
    if (ui->player_2->isChecked())
    {
        player_choice=2;
        return;
    }
    if (ui->player_3->isChecked())
    {
        player_choice=3;
        return;
    }
    if (ui->player_4->isChecked())
    {
        player_choice=4;
        return;
    }
    if (ui->player_5->isChecked())
    {
        player_choice=5;
        return;
    }
    if (ui->player_6->isChecked())
    {
        player_choice=6;
        return;
    }
    if (ui->player_7->isChecked())
    {
        player_choice=7;
        return;
    }
    if (ui->player_8->isChecked())
    {
        player_choice=8;
        return;
    }
    if (ui->player_9->isChecked())
    {
        player_choice=9;
        return;
    }
    if (ui->player_10->isChecked())
    {
        player_choice=10;
        return;
    }
    if (ui->player_11->isChecked())
    {
        player_choice=11;
        return;
    }
    if (ui->player_12->isChecked())
    {
        player_choice=12;
        return;
    }
}

void Widget::on_edit_2_clicked()
{
    edit_dia2 = new edit_2(this);
    qDebug()<<"oh0";
    edit_dia2->setModal(true);
    edit_dia2->show();
}

void Widget::load()
{
    //filePath_list -> the path of files
    QStringList filePath_list = QFileDialog::getOpenFileNames(this,"Open","","All File(*)");
    QStringList fileName_list;
    QVector<int> tmp_player_ids;
    QVector<double>tmp_t;
    QVector<double>tmp_l;
    QVector<double>tmp_w;
    QVector<double>tmp_h;
    QStringList tmp_strlist;
    int processing_f;
    int current_f;
    QFileInfo info;

    for(int i=0;i<filePath_list.size();i++)
    {
        qDebug()<<filePath_list[i];
        info = QFileInfo(filePath_list[i]);
        fileName_list << info.fileName();
    }
    //filename_list -> the name of files
    for(int i=0;i<fileName_list.size();i++)
        qDebug()<<fileName_list[i];

    //check whether the files are not suitable
    if (fileName_list.indexOf("left.txt")==-1||fileName_list.indexOf("right.txt")==-1||fileName_list.indexOf("left.mp4")==-1||fileName_list.indexOf("right.mp4")==-1)
    {
        QMessageBox::information(NULL,"ERROR","File Error!Please Open the right files(left.txt, right.txt, left.mp4, right.mp4)");
        return;
    }

    //suitable then ready to read files

    QFile file_left(filePath_list[fileName_list.indexOf("left.txt")]);
    if (!file_left.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::information(NULL,"ERROR","Can't read the file! Check the form of file!");
        return;
    }
    while(!file_left.atEnd()) {
        QByteArray line = file_left.readLine();
        QString str(line);
        left_data.append(str);
    }

    QFile file_right(filePath_list[fileName_list.indexOf("right.txt")]);
    if (!file_right.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::information(NULL,"ERROR","Can't read the file! Check the form of file!");
        return;
    }
    while(!file_right.atEnd()) {
        QByteArray line = file_right.readLine();
        QString str(line);
        right_data.append(str);
    }

    //try to resolve left_data and right_data
    processing_f = 1;
    for (int i =0;i<left_data.length();++i)
    {
        tmp_strlist = left_data[i].split(",");
        current_f = tmp_strlist[0].toInt();
        if (current_f!=processing_f)
        {
            left_frame.append(Frame(processing_f,tmp_player_ids,tmp_t,tmp_l,tmp_w,tmp_h));
            processing_f = current_f;
            tmp_player_ids.clear();
            tmp_t.clear();
            tmp_l.clear();
            tmp_w.clear();
            tmp_h.clear();
        }
        if (check_region(0,tmp_strlist[2].toDouble(),tmp_strlist[3].toDouble(),tmp_strlist[4].toDouble(),tmp_strlist[5].toDouble()))
        {
            tmp_player_ids.append(tmp_strlist[1].toInt());
            tmp_t.append(tmp_strlist[2].toDouble());
            tmp_l.append(tmp_strlist[3].toDouble());
            tmp_w.append(tmp_strlist[4].toDouble());
            tmp_h.append(tmp_strlist[5].toDouble());
        }

    }

    tmp_player_ids.clear();
    tmp_t.clear();
    tmp_l.clear();
    tmp_w.clear();
    tmp_h.clear();

    processing_f = 1;
    for (int i =0;i<right_data.length();++i)
    {

        tmp_strlist = right_data[i].split(",");
        current_f = tmp_strlist[0].toInt();
        if (current_f!=processing_f)
        {
            right_frame.append(Frame(processing_f,tmp_player_ids,tmp_t,tmp_l,tmp_w,tmp_h));
            processing_f = current_f;
            tmp_player_ids.clear();
            tmp_t.clear();
            tmp_l.clear();
            tmp_w.clear();
            tmp_h.clear();
        }
        if (check_region(1,tmp_strlist[2].toDouble(),tmp_strlist[3].toDouble(),tmp_strlist[4].toDouble(),tmp_strlist[5].toDouble()))
        {
            tmp_player_ids.append(tmp_strlist[1].toInt());
            tmp_t.append(tmp_strlist[2].toDouble());
            tmp_l.append(tmp_strlist[3].toDouble());
            tmp_w.append(tmp_strlist[4].toDouble());
            tmp_h.append(tmp_strlist[5].toDouble());
        }
    }
    Frame_num = left_frame.length()<right_frame.length()?left_frame.length():right_frame.length();

    //left_frame_get right_frame_get
    process_heatmap_file();

    left_movie_path = filePath_list[fileName_list.indexOf("left.mp4")];
    right_movie_path = filePath_list[fileName_list.indexOf("right.mp4")];

    filePath_list.clear();
    fileName_list.clear();
    loaded = true;
}

void Widget::video_play()
{
    ui->progressBar->setValue(100);
    if (left_movie_path==nullptr ||right_movie_path==nullptr)
    {
        QMessageBox::information(NULL,"ERROR","You haven't got videos yet!");
        return;
    }
    left_player->setMedia(QMediaContent(QUrl::fromLocalFile(left_movie_path)));
    //put the video in the video-play widget
    left_player->setVideoOutput(left_play_widget);
    right_player->setMedia(QMediaContent(QUrl::fromLocalFile(right_movie_path)));
    right_player->setVideoOutput(right_play_widget);
    //connect(left_player,&QMediaPlayer::positionChanged,this,&Widget::fresh_information);
    //connect(right_player,&QMediaPlayer::positionChanged,this,&Widget)
    ui->progressBar->setVisible(false);
    left_player->play();
    right_player->play();
}

void Widget::start_load()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    load();
    if(loaded)
    {
        LoadPlayer();
        video_play();
        ui->slider_progress->setEnabled(true);
        ui->slider_progress->setRange(0,maxValue);
        ui->time->setText("01:05");
        timer = new QTimer();
        timer->setInterval(1000);
        timer->start();

        timer1 = new QTimer();
        timer1->setInterval(100);
        timer1->start();

        emit begin_heatmap();
        emit begin_roadheatmap();

        connect(timer,SIGNAL(timeout()),this,SLOT(onTimerOut()));
        connect(timer1,SIGNAL(timeout()),this,SLOT(onTimer1Out()));
        connect(this,SIGNAL(timer1_signal(double*,double*,double*,double*,int,int)),ui->football_2D,SLOT(timer1_slot(double*,double*,double*,double*,int,int)));
    }
    else
    {
        //ui->progressBar->setValue(0);
        ui->progressBar->setVisible(false);
    }


}

void Widget::trans_to_2D(int frame_class, double t,double l,double* newx,double* newy)
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

void Widget::onTimer1Out()
{
    current_frame += 3;
    if (current_frame>=Frame_num)
    {
        return;
    }
    //send signal with proper location of the player point
    int ln = left_frame[current_frame].player_ids.length();
    int rn = right_frame[current_frame].player_ids.length();
    //left_frame[current_frame].t;

    if (left_frame[current_frame].l.length()>0 && right_frame[current_frame].l.length()>0)
    {
        emit timer1_signal(&left_frame[current_frame].t[0],&left_frame[current_frame].l[0],&right_frame[current_frame].t[0],&right_frame[current_frame].l[0],ln,rn);
        emit heatmap_signal(&left_frame[current_frame].t[0],&left_frame[current_frame].l[0],&right_frame[current_frame].t[0],&right_frame[current_frame].l[0],ln,rn);
    }
    else
    {
        if (left_frame[current_frame].l.length()<=0)
        {
            if (right_frame[current_frame].l.length()<=0)
            {
                emit timer1_signal(nullptr,nullptr,nullptr,nullptr,0,0);
                emit heatmap_signal(nullptr,nullptr,nullptr,nullptr,0,0);
            }
            else
            {
                emit timer1_signal(nullptr,nullptr,&right_frame[current_frame].t[0],&right_frame[current_frame].l[0],0,rn);
                emit heatmap_signal(nullptr,nullptr,&right_frame[current_frame].t[0],&right_frame[current_frame].l[0],0,rn);
            }
        }
        else
        {
            emit timer1_signal(&left_frame[current_frame].t[0],&left_frame[current_frame].l[0],nullptr,nullptr,ln,0);
            emit heatmap_signal(&left_frame[current_frame].t[0],&left_frame[current_frame].l[0],nullptr,nullptr,ln,0);
        }
    }


}

void Widget::onTimerOut()
{
    static int curtime_s=-2;
    static int curtime_m=0;
    if ((curtime_m*60+curtime_s)<maxValue)
    {
        curtime_s+=1;
    }
    if(curtime_s==60)
    {
        curtime_s=0;
        curtime_m+=1;
    }

    int position=left_player->position()/1000;
    QString str1=QString::number(curtime_m);
    QString str2=QString::number(curtime_s);
    if (curtime_m==0&&curtime_s!=-2)
    {
        if (curtime_s<10)
        {
            ui->ctime->setText("00:0"+str2+"/");
        }
        else {
            ui->ctime->setText("00:"+str2+"/");
        }
    }
    else if(curtime_s!=-2)
    {
        if (curtime_s<10)
        {
            ui->ctime->setText("0"+str1+":0"+str2+"/");
        }
        else {
            ui->ctime->setText("0"+str1+":"+str2+"/");
        }
    }

    ui->slider_progress->setValue(position);
    fresh_information();

    //每一秒更新



}

double Widget::point_distance(double x_,double y_,double x,double y)
{
    //having no APIs for position translation
    double result;
    double rx,rx_,ry,ry_;
    rx = x/1280*105;
    rx_ = x_/1280*105;
    ry = y/720*68;
    ry_ = y_/720*68;

    result = sqrt((rx_-rx)*(rx_-rx)+(ry_-ry)*(ry_-ry));
    if (result<0)
    {
        qDebug()<<"?";
    }
    return result;
}

bool Widget::check_region(int f_class, double t, double l,double w,double h)
{
    bool result = true;
    //check in the left video
    if (f_class==0)
    {
        if ((l+h)>(3/16*t+540))
        {
            result = false;
        }

    }
    else//check for the right video
    {
        if (l<300||(l+h>730)||(t+w)>1280)
        {
            result = false;
        }
    }
    return result;
}

bool Widget::compare_num_pool(QVector<int> v1, QVector<int> v2)
{
    bool result = true;
    for(int tmp=0;tmp<v2.length();++tmp)
    {
        if (v1.indexOf(v2[tmp])==-1)
        {
            result = false;
        }
    }
    return result;
}

void Widget::LoadPlayer()
{
    int lrnum=100;
    int m=0;
    bool compare1;
    bool compare2;
    bool found_after=false;
    qDebug()<<left_frame.length();
    if (my_players)
    {
        delete my_players;
    }
    my_players = new FootballPlayer[12];
    QVector<int> left_occupy_id;
    QVector<int> right_occupy_id;
    //initilization the information of players, that's important work I think
    for (int i =0;i<Frame_num;++i)
    {
        ui->progressBar->setValue(i/Frame_num);
        if (i==0)
        {
            //left
            int j=0,k=0;
            for (j=0;j<left_frame[0].player_ids.length();++j)
            {
                my_players[j].player_name = left_frame[0].player_ids[j];
                my_players[j].frame_class.append(0);
                my_players[j].player_id_in_output.append(my_players[j].player_name);
                left_occupy_id.append(left_frame[0].player_ids[j]);
                //left_num_pool.append(my_players[j].player_name);
            }
            for (k=0;k<right_frame[0].player_ids.length();++k)
            {
                my_players[j+k].player_name = right_frame[0].player_ids[k];
                my_players[j+k].frame_class.append(1);
                my_players[j+k].player_id_in_output.append(my_players[j+k].player_name);
                right_occupy_id.append(right_frame[0].player_ids[k]);
            }
        }
        else
        {
            compare1 = true;
            compare2 = true;
            for (int tmp=0;tmp<left_frame[i-1].player_ids.length();++tmp)
            {
                if (left_frame[i].player_ids.indexOf(left_frame[i-1].player_ids[tmp])==-1)
                {
                    compare1 = false;
                    break;
                }
            }
            for (int tmp=0;tmp<right_frame[i-1].player_ids.length();++tmp)
            {
                if (right_frame[i].player_ids.indexOf(right_frame[i-1].player_ids[tmp])==-1)
                {
                    compare2 = false;
                    break;
                }
            }

            if (compare1 && compare2)
            {
                //they are the same as i-1 I think
                for (int j=0;j<12;++j)
                {
                    my_players[j].frame_class.append(my_players[j].frame_class[i-1]);
                    my_players[j].player_id_in_output.append(my_players[j].player_id_in_output[i-1]);
                }
            }
            else
            {
                for (int j=0;j<12;++j)
                {
                    if (my_players[j].player_id_in_output[i-1]==-1)
                    {
                        my_players[j].frame_class.append(-1);
                        my_players[j].player_id_in_output.append(-1);
                        //qDebug()<<i<<j<<"keep losting";
                        continue;
                    }

                    if (my_players[j].frame_class[i-1]==0)//player just in the left video
                    {
                        // player last seen in the left video
                        if (left_frame[i].player_ids.indexOf(my_players[j].player_id_in_output[i-1])!=-1)
                        {
                            //the player_id could still be seen in the left video,then the best!
                            my_players[j].frame_class.append(0);
                            my_players[j].player_id_in_output.append(my_players[j].player_id_in_output[i-1]);
                        }
                        else//player not being seen in the left video!search in the right video
                        {
                            //first search current right video
                            for (m=0;m<right_frame[i].player_ids.length();++m)
                            {
                                if (right_occupy_id.indexOf(right_frame[i].player_ids[m])==-1)
                                {
                                    //qDebug()<<"find right more!";
                                    right_occupy_id.append(right_frame[i].player_ids[m]);
                                    my_players[j].frame_class.append(1);
                                    my_players[j].player_id_in_output.append(right_frame[i].player_ids[m]);
                                }
                            }
                            //search in the before and after right video
                            if (m==right_frame[i].player_ids.length())
                            {
                                //my_players
                                for (int k=1;k<=lrnum;++k)
                                {
                                    found_after=false;
                                    if((i+k)<right_frame.length())
                                    {
                                        for(m=0;m<right_frame[i+k].player_ids.length();++m)
                                        {
                                            if (right_occupy_id.indexOf(right_frame[i+k].player_ids[m])==-1)
                                            {
                                                //qDebug()<<"find right more! in right after!";
                                                found_after=true;
                                                right_occupy_id.append(right_frame[i+k].player_ids[m]);
                                                my_players[j].frame_class.append(1);
                                                my_players[j].player_id_in_output.append(right_frame[i+k].player_ids[m]);
                                                break;
                                            }
                                        }
                                    }

                                    if ((i-k)>=0 && found_after==false)
                                    {
                                        for(m=0;m<right_frame[i-k].player_ids.length();++m)
                                        {
                                            if (right_occupy_id.indexOf(right_frame[i-k].player_ids[m])==-1)
                                            {
                                                //qDebug()<<"find right more! in right before!";
                                                right_occupy_id.append(right_frame[i-k].player_ids[m]);
                                                my_players[j].frame_class.append(1);
                                                my_players[j].player_id_in_output.append(right_frame[i-k].player_ids[m]);
                                                break;
                                            }
                                        }
                                    }

                                }
                            }
                            //not find either in left nor right (lost!)
                            if (my_players[j].frame_class.length()<(i+1))
                            {
                                my_players[j].frame_class.append(-1);
                                my_players[j].player_id_in_output.append(-1);
                            }
                        }

                    }
                    else//player just seen in the right video
                    {
                        // player last seen in the right video
                        if (right_frame[i].player_ids.indexOf(my_players[j].player_id_in_output[i-1])!=-1)
                        {
                            //the player_id could still be seen in the right video,then the best!
                            my_players[j].frame_class.append(1);
                            my_players[j].player_id_in_output.append(my_players[j].player_id_in_output[i-1]);
                        }
                        else//player not being seen in the right video!search in the left video
                        {
                                //first search current left video
                                for (m=0;m<left_frame[i].player_ids.length();++m)
                                {
                                    if (left_occupy_id.indexOf(left_frame[i].player_ids[m])==-1)
                                    {
                                        //qDebug()<<"find left more!";
                                        left_occupy_id.append(left_frame[i].player_ids[m]);
                                        my_players[j].frame_class.append(0);
                                        my_players[j].player_id_in_output.append(left_frame[i].player_ids[m]);
                                    }
                                }
                                //search in the before and after right video
                                if (m==left_frame[i].player_ids.length())
                                {
                                    //my_players
                                    for (int k=1;k<=lrnum;++k)
                                    {
                                        found_after=false;
                                        if ((i+k)<left_frame.length())
                                        {
                                            for(m=0;m<left_frame[i+k].player_ids.length();++m)
                                            {
                                                if (left_occupy_id.indexOf(left_frame[i+k].player_ids[m])==-1)
                                                {
                                                    //qDebug()<<"find left more! in left after!";
                                                    found_after=true;
                                                    left_occupy_id.append(left_frame[i+k].player_ids[m]);
                                                    my_players[j].frame_class.append(0);
                                                    my_players[j].player_id_in_output.append(left_frame[i+k].player_ids[m]);
                                                    break;
                                                }
                                            }

                                        }

                                        if ((i-k)>=0 && found_after==false)
                                        {
                                            for(m=0;m<left_frame[i-k].player_ids.length();++m)
                                            {
                                                if (left_occupy_id.indexOf(left_frame[i-k].player_ids[m])==-1)
                                                {
                                                    //qDebug()<<"find left more! in left before!";
                                                    left_occupy_id.append(left_frame[i-k].player_ids[m]);
                                                    my_players[j].frame_class.append(0);
                                                    my_players[j].player_id_in_output.append(left_frame[i-k].player_ids[m]);
                                                    break;
                                                }
                                            }
                                        }

                                    }
                                }
                                //not find either in left nor right (lost!)
                                if (my_players[j].frame_class.length()<(i+1))
                                {
                                    my_players[j].frame_class.append(-1);
                                    my_players[j].player_id_in_output.append(-1);
                                }
                            }
                    }
                }
            }
        }
    }
    loaded = true;
    qDebug()<<loaded;
    return;
}

void Widget::process_heatmap_file()
{

    QFile file(QDir::currentPath()+"/heatmap.txt");
    qDebug()<<QDir::currentPath();
    QString tmp;
    bool ok = file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate);
    if (!ok)
    {
        qDebug()<<"can't write file!";
        return;
    }


    for (int i=0;i<Frame_num;++i)
    {
        for (int j=0;j<left_frame[i].player_ids.length();++j)
        {
            double newx,newy;
            trans_to_2D(0,left_frame[i].t[j]+left_frame[i].w[j]/2,left_frame[i].l[j]+left_frame[i].h[j]/2,&newx,&newy);
            tmp = QString::number(newx)+","+QString::number(newy)+"\n";
            file.write(tmp.toUtf8());
        }
        for (int j=0;j<right_frame[i].player_ids.length();++j)
        {
            double newx,newy;
            trans_to_2D(1,right_frame[i].t[j]+right_frame[i].w[j]/2,right_frame[i].l[j]+right_frame[i].h[j]/2,&newx,&newy);
            //fprintf(fpWrite,"%lf,%lf\n",newx,newy);
            tmp = QString::number(newx)+","+QString::number(newy)+"\n";
            file.write(tmp.toUtf8());
        }
    }
    file.close();
    return ;


}
