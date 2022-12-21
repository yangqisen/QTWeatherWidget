#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //窗口无边框
    setWindowFlag(Qt::FramelessWindowHint);

    //右键退出的菜单
    exitMenu = new QMenu(this);
    exitAction = new QAction();
    exitAction->setText("退出");

    exitMenu->addAction(exitAction);
    connect(exitAction, QAction::triggered, this, [=]{
        qApp->exit(0);
    });

    //关联获取到天气数据后的槽函数
    networkAM = new QNetworkAccessManager(this);
    connect(networkAM, &QNetworkAccessManager::finished, this,&MainWindow::Replied);

    queryW("101010100");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Replied(QNetworkReply *reply)
{
    qDebug() << "haohao";
    QByteArray arr = reply->readAll();
//    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "" << arr.data();
}

//获取某个城市天气数据 http://t.weather.itboy.net/api/weather/city/
void MainWindow::queryW(QString city)
{
    QUrl url("http://t.weather.itboy.net/api/weather/city/" + city);
    networkAM->get(QNetworkRequest(url));
}

//右键菜单事件
void MainWindow::contextMenuEvent(QContextMenuEvent *rightevent)
{
    //点击右键弹出菜单
    exitMenu->exec(QCursor::pos());
    rightevent->accept();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //获取偏移量
    clickOffset = event->globalPos() - this->pos(); //鼠标点击的位置减去窗口左上角的位置

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //鼠标移动后设置窗口的位置
    this->move(event->globalPos() - clickOffset);
}


