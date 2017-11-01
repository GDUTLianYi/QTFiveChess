
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMovie>
#include <QSound>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    //ui->label->setText("abbcc");
    //加载资源文件中的两个动图
    QMovie *qm=new QMovie(":/ti.gif");
    QMovie *qm2=new QMovie(":/ti2.gif");
    //将动图显示在label上
    ui->label->setMovie(qm);
    ui->label_2->setMovie(qm2);
    //设置动图的大小
    ui->label->setFixedSize(300,400);
    ui->label_2->setFixedSize(300,400);
    qm->start();
    qm2->start();
    ui->label->show();
    ui->label_2->show();
    //加载声音文件
    song=new QSound(":/wav.wav");
    song->play();
    //设置音乐无限循环
    song->setLoops(QSound::Infinite);
    //设置窗口大小标题和图标
    this->resize( QSize( 1200, 700 ));
    this->setWindowIcon(QIcon(":/icon1.ico"));
    this->setWindowTitle("my game");
    //预处理棋盘上每个位置的中心坐标，为绘制做准备
    for(int y=0;y<10;y++){
        for(int x=0;x<10;x++){
            p[y][x]=QPoint(100+x*50+25,100+y*50+25);

        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::check(){

    /*
     * 检查是否有五子成线，枚举每一个位置，向右扩展四格检查
     * 向下拓展检查，向左下主，右下副对角线拓展检查
     */
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){

            if(chess[i][j]==0){
                continue;
            }
            int flag[4]={1,1,1,1};
            for(int k=1;k<5;k++){
                if(j+k<10 && chess[i][j+k]==chess[i][j]){

                }else{
                    flag[0]=0;break;
                }
            }
            if(flag[0]==1){
                game_over=true;start=p[i][j];end=p[i][j+4]; return 1;
            }
            for(int k=1;k<5;k++){
                if(i+k<10 && chess[i+k][j]==chess[i][j]){

                }else{
                    flag[1]=0;break;
                }
            }
            if(flag[1]==1){
                game_over=true;start=p[i][j];end=p[i+4][j]; return 1;
            }
            for(int k=1;k<5;k++){
                if(j+k<10 && i+k<10 && chess[i+k][j+k]==chess[i][j]){

                }else{
                    flag[2]=0;break;
                }
            }
            if(flag[2]==1){
                game_over=true;start=p[i][j];end=p[i+4][j+4]; return 1;
            }
            for(int k=1;k<5;k++){
                if(i-k>=0 && j+k<10 && chess[i-k][j+k]==chess[i][j]){

                }else{
                    flag[3]=0;break;
                }
            }
            if(flag[3]==1){
                game_over=true;start=p[i][j];end=p[i-4][j+4]; return 1;
            }

        }
    }
    return 0;
}

void MainWindow::paintEvent(QPaintEvent *){

    QPainter painter(this);
    //设置线为蓝色
    painter.setPen(Qt::blue);

    //绘制竖线
    for(int x=0;x<11;x++){
        painter.drawLine(QPoint(100+x*50,100),QPoint(100+x*50,600));
    }
    //绘制横线
    for(int y=0;y<11;y++){
        painter.drawLine(QPoint(100,100+y*50),QPoint(600,100+y*50));
    }

    //检查状态数组，为1时绘制黑色棋子，为0时绘制红色棋子
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(chess[i][j]==1){
                painter.setBrush(Qt::black);
                painter.setPen(Qt::black);
                painter.drawEllipse(p[i][j],10,10);
            }else if(chess[i][j]==-1){
                painter.setBrush(Qt::red);
                painter.setPen(Qt::red);
                painter.drawEllipse(p[i][j],10,10);
            }
        }
    }
    // painter.drwaLine(QPoint(100,100),QPoint(100,100));

   //如果游戏结束，用绿色的线 连出五子成线的 棋子
    if(game_over==true){
        painter.setBrush(Qt::green);
        painter.setPen(Qt::green);
        painter.drawLine(start,end);
        return ;
    }
   //检查游戏是否胜负已分
    if(game_over==false&&check()){

        update();
        QMessageBox::information(this,"提示","游戏结束");
    }
}


void MainWindow::on_pushButton_clicked()
{
    //处理开始游戏的事务 棋盘状态置空
    count=0;
    game_over=false;
    memset(chess,0,sizeof(chess));
    update();
}


void MainWindow::mousePressEvent(QMouseEvent *event){

    //当游戏结束时，直接返回不做处理
    if(game_over==true){
        return ;
    }
    //棋盘上已经没有位置可下了，提示平局
    if(count==100){
        QMessageBox::information(this,"提示","平局");
        return;
    }
    //获得落子的客户区位置
    int x=event->x();
    int y=event->y();

    // 估算落子坐标大概在第几行第几列
    // 然后枚举该位置及其上下左右，找到距离落子坐标的最小的为最终位置

    int dir[5][2]={{0,0},{-1,0},{1,0},{0,1},{0,-1}};
    int dist=1e8,res=-1;
    for(int i=0;i<5;i++){
        int nx=(x-100)/50+dir[i][0];
        int ny=(y-100)/50+dir[i][1];
        if(nx<0 || ny<0 || nx>9 || ny>9 || chess[ny][nx]!=0){
            continue;
        }
        long long d= (x-p[ny][nx].x())*(x-p[ny][nx].x())+(y-p[ny][nx].y())*(y-p[ny][nx].y());
        if(d<dist){
            dist=d;res=i;
        }
    }
    //超出棋盘外的要忽视
    if(res!=-1&&dist<200){
        count++;
        int gx=(x-100)/50+dir[res][0];
        int gy=(y-100)/50+dir[res][1];
        if(count&1){
            chess[gy][gx]=1;
        }else{
            chess[gy][gx]=-1;
        }
    }
    //更新窗口
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){

}

void MainWindow::on_pushButton_2_clicked()
{
    //当前正在播放音乐 则停止 否则反之
    if(song->isFinished()){
        song->play();
    }else{
        song->stop();

    }
}
