#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

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

//获取天气数据后解析数据
void MainWindow::Replied(QNetworkReply *reply)
{
    QByteArray arr = reply->readAll();
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "" << arr.data();
    //判断是否请求数据成功
    if(status_code != 200){
        QMessageBox::warning(this, u8"warning", u8"请求出错", QMessageBox::Ok);
        return;
    }

    QJsonParseError error;
    QJsonDocument docu = QJsonDocument::fromJson(arr, &error);
    if(error.error != QJsonParseError::NoError){//如果解析出错
        QMessageBox::warning(this, u8"warning", u8"解析数据出错", QMessageBox::Ok);
        return;
    }

    QJsonObject obj = docu.object();

    //解析昨天的天气数据
    QJsonObject dataObj = obj.value("data").toObject();
    QJsonObject yesterdayObj = dataObj.value("yesterday").toObject();
    dayright[0].date = yesterdayObj.value("ymd").toString();
    dayright[0].week = yesterdayObj.value("week").toString();
    //最高温和最低温字符串需要进行分割处理
    QString str;
    str = yesterdayObj.value("high").toString().split(" ").at(1);
    str = str.mid(0, str.length() - 1);
    dayright[0].HighestT = str.toInt();
    qDebug() << "" << dayright[0].HighestT;
    str = yesterdayObj.value("low").toString().split(" ").at(1);
    str = str.mid(0, str.length() - 1);
    dayright[0].lowestT = str.toInt();
    qDebug() << "" << dayright[0].lowestT;
    dayright[0].kongqi = yesterdayObj.value("aqi").toDouble();
    dayright[0].Weather = yesterdayObj.value("type").toString();
    qDebug() << "" << dayright[0].kongqi;
    qDebug() << "" << dayright[0].Weather;

    //解析forcast中前五天的数据并放入dayright[1-5]中
    QJsonArray forecastArray = obj.value("forecast").toArray();
    for(int i = 0; i < 5; i++){
        QJsonObject rightdayObj = forecastArray[0].toObject();
        dayright[i+1].date = yesterdayObj.value("ymd").toString();
        dayright[i+1].week = yesterdayObj.value("week").toString();
        dayright[i+1].kongqi = yesterdayObj.value("aqi").toDouble();
        dayright[i+1].Weather = yesterdayObj.value("type").toString();
        //最高温和最低温字符串需要进行分割处理
        QString str;
        str = yesterdayObj.value("high").toString().split(" ").at(1);
        str = str.mid(0, str.length() - 1);
        dayright[i+1].HighestT = str.toInt();
        str = yesterdayObj.value("low").toString().split(" ").at(1);
        str = str.mid(0, str.length() - 1);
        dayright[i+1].lowestT = str.toInt();
    }

    //解析今天的数据
    dayleft.pm25 = dataObj.value("pm25").toDouble();
    dayleft.kongqi = dataObj.value("quality").toString();
    dayleft.shidu = dataObj.value("shidu").toString();

    dayleft.wendu = dataObj.value("wendu").toInt();
    dayleft.ganmaozhishu = dataObj.value("ganmao").toString();
    dayleft.HighestT = dayright[1].HighestT;
    dayleft.lowestT = dayright[1].lowestT;
    dayleft.Weather = dayright[1].Weather;
    dayleft.date = dayright[1].date;
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


