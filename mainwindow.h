#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    //右键退出所需
    QMenu * exitMenu;
    QAction* exitAction;

protected:
    void contextMenuEvent(QContextMenuEvent *rightevent);

};
#endif // MAINWINDOW_H
