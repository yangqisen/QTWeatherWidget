#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "DayWeatherFile.h"
#include <QLabel>
#include "getCityCode.h"

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
    void update();

    //右键退出所需
    QMenu * exitMenu;
    QAction* exitAction;

    //控件数组
    QList<QLabel*> weeks;
    QList<QLabel*> dates;
    QList<QLabel*> kongqis;
    QList<QLabel*> weathers;
    QList<QLabel*> weatherIcons;

    QPoint clickOffset;//点击拖拽时鼠标位置与窗口左上角的偏移量

    QNetworkAccessManager* networkAM;

    DayLeft dayleft;//左边天气数据
    DayRight dayright[6];    //右边六天天气数据

    QMap<QString, QString> weatherIconMap;
    getCityCode getCityCode;//获取城市编码


protected:
    void contextMenuEvent(QContextMenuEvent *rightevent);

    //实现鼠标点击拖动窗口
    void mousePressEvent(QMouseEvent* event);//鼠标点击获取鼠标位置
    void mouseMoveEvent(QMouseEvent* event);
    QString queryCityCode(QString cityName);
private slots:
    void on_btnSearch_clicked();
};
#endif // MAINWINDOW_H
