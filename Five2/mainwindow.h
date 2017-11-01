#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include<QMouseEvent>
#include<QMessageBox>
#include<QSound>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    int check(); //检查是否有五子成线
    int chess[100][100]; //棋盘状态 1黑子 -1白字 0空子
    QPoint p[100][100]; //保存棋盘上每个位置的中心坐标
    QPoint start,end; //记录五子成线时的，起始和终止坐标
    bool game_over;//判断游戏是否结束
    int count; //记录已下棋子的数量
    QSound* song;// 指向加载wav对象的指针
    void paintEvent(QPaintEvent *); //用户窗口的重绘
    void mousePressEvent(QMouseEvent *event);// 响应落子事件
    void mouseReleaseEvent(QMouseEvent *event); //未用到
private slots:
    void on_pushButton_clicked(); //响应开始游戏事件
    void on_pushButton_2_clicked();//响应背景音乐播放和暂停事件

private:
    Ui::MainWindow *ui; // 程序界面
};

#endif // MAINWINDOW_H
