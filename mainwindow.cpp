#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *rightevent)
{
    //点击右键弹出菜单
    exitMenu->exec(QCursor::pos());
    rightevent->accept();
}


