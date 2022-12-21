#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    void Replied(QNetworkReply *reply);
    void queryW(QString city);

    //右键退出所需
    QMenu * exitMenu;
    QAction* exitAction;

    QPoint clickOffset;//点击拖拽时鼠标位置与窗口左上角的偏移量

    QNetworkAccessManager* networkAM;

protected:
    void contextMenuEvent(QContextMenuEvent *rightevent);

    //实现鼠标点击拖动窗口
    void mousePressEvent(QMouseEvent* event);//鼠标点击获取鼠标位置
    void mouseMoveEvent(QMouseEvent* event);
};
#endif // MAINWINDOW_H
