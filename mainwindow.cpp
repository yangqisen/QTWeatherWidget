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
#include "getCityCode.h"
#include <QHostInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCodec>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    m_pNet(new QNetworkAccessManager(this))
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

    //控件数组初始化
    weeks << ui->lblWeek0 << ui->lblWeek1 << ui->lblWeek2 << ui->lblWeek3<< ui->lblWeek4 << ui->lblWeek5;
    weathers << ui->lblType0 << ui->lblType1 << ui->lblType2 << ui->lblType3<< ui->lblType4 << ui->lblType5;
    dates << ui->lblDate0 << ui->lblDate1 << ui->lblDate2 << ui->lblDate3<< ui->lblDate4 << ui->lblDate5;
    weatherIcons << ui->lblTypeIcon0 << ui->lblTypeIcon1 << ui->lblTypeIcon2 << ui->lblTypeIcon3<< ui->lblTypeIcon4 << ui->lblTypeIcon5;
    kongqis << ui->lblQuality0 << ui->lblQuality1 << ui->lblQuality2 << ui->lblQuality3<< ui->lblQuality4 << ui->lblQuality5;
    highs << ui->lblHigh0 << ui->lblHigh1 << ui->lblHigh2 << ui->lblHigh3<< ui->lblHigh4 << ui->lblHigh5;
    lows << ui->lblLow0 << ui->lblLow1 << ui->lblLow2 << ui->lblLow3<< ui->lblLow4 << ui->lblLow5;
    //关联获取到天气数据后的槽函数
    networkAM = new QNetworkAccessManager(this);
    connect(networkAM, &QNetworkAccessManager::finished, this,&MainWindow::Replied);

    getLocalCity();


    //封装图标
    weatherIconMap.insert("暴雪" , ":/res/icon/BaoXue.png");
    weatherIconMap.insert("暴雨" , ":/res/icon/BaoYu.png");
    weatherIconMap.insert("暴雨到大暴雨" , ":/res/icon/BaoYuDaoDaBaoYu.png");
    weatherIconMap.insert("大暴雨" , ":/res/icon/DaBaoYu.png");
    weatherIconMap.insert("大暴雨到特大暴雨" , ":/res/icon/DaBaoYuDaoTeDaBaoYu.png");
    weatherIconMap.insert("大到暴雪" , ":/res/icon/DaDaoBaoXue.png");
    weatherIconMap.insert("大雪" , ":/res/icon/DaXue.png");
    weatherIconMap.insert("大雨" , ":/res/icon/DaYu.png");
    weatherIconMap.insert("多云" , ":/res/icon/DuoYun.png");
    weatherIconMap.insert("拂尘" , ":/res/icon/FuChen.png");
    weatherIconMap.insert("雷阵雨" , ":/res/icon/LeiZhenYu.png");
    weatherIconMap.insert("雷阵雨伴有冰雹" , ":/res/icon/LeiZhenYuBanYouBingBao.png");
    weatherIconMap.insert("霾" , ":/res/icon/Mai.png");
    weatherIconMap.insert("强沙尘暴" , ":/res/icon/QiangShaChenBao.png");
    weatherIconMap.insert("晴" , ":/res/icon/Qing.png");
    weatherIconMap.insert("沙尘暴" , ":/res/icon/ShaChenBao.png");
    weatherIconMap.insert("特大暴雨" , ":/res/icon/TeDaBaoYu.png");
    weatherIconMap.insert("雾" , ":/res/icon/Wu.png");
    weatherIconMap.insert("小到中雪" , ":/res/icon/XiaoDaoZhongXue.png");
    weatherIconMap.insert("小到中雨" , ":/res/icon/XiaoDaoZhongYu.png");
    weatherIconMap.insert("雪" , ":/res/icon/Xue.png");
    weatherIconMap.insert("扬沙" , ":/res/icon/YangSha.png");
    weatherIconMap.insert("阴" , ":/res/icon/Yin.png");
    weatherIconMap.insert("暴雪" , ":/res/icon/BaoXue.png");
    weatherIconMap.insert("雨" , ":/res/icon/Yu.png");
    weatherIconMap.insert("雨夹雪" , ":/res/icon/YuJiaXue.png");
    weatherIconMap.insert("阵雪" , ":/res/icon/ZhenXue.png");
    weatherIconMap.insert("阵雨" , ":/res/icon/ZhenYu.png");
    weatherIconMap.insert("中到大雪" , ":/res/icon/ZhongDaoDaXue.png");
    weatherIconMap.insert("中到大雨" , ":/res/icon/ZhongDaoDaYu.png");
    weatherIconMap.insert("中到大雪" , ":/res/icon/ZhongDaoDaXue.png");
    weatherIconMap.insert("中雨" , ":/res/icon/ZhongYu.png");
    weatherIconMap.insert("中雪" , ":/res/icon/ZhongXue.png");



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

    //解析forcast中前五天的数据并放入dayright[1-5]中
    QJsonArray forecastArray = dataObj.value("forecast").toArray();
    for(int i = 0; i < 5; i++){
        QJsonObject rightdayObj = forecastArray[i].toObject();
        dayright[i+1].date = rightdayObj.value("ymd").toString();
        dayright[i+1].week = rightdayObj.value("week").toString();
        dayright[i+1].kongqi = rightdayObj.value("aqi").toDouble();
        dayright[i+1].Weather = rightdayObj.value("type").toString();
        //最高温和最低温字符串需要进行分割处理
        QString str;
        str = rightdayObj.value("high").toString().split(" ").at(1);
        str = str.mid(0, str.length() - 1);

        dayright[i+1].HighestT = str.toInt();
        str = rightdayObj.value("low").toString().split(" ").at(1);
        str = str.mid(0, str.length() - 1);
        dayright[i+1].lowestT = str.toInt();
    }

    //解析今天的数据
    dayleft.city = obj.value("cityInfo").toObject().value("city").toString();
    dayleft.pm25 = dataObj.value("pm25").toDouble();
    dayleft.kongqi = dataObj.value("quality").toString();
    dayleft.shidu = dataObj.value("shidu").toString();
    QJsonObject rightdayObj = forecastArray[1].toObject();
    dayleft.fx = rightdayObj.value("fx").toString();
    dayleft.fl = rightdayObj.value("fl").toString();
    dayleft.wendu = dataObj.value("wendu").toString().toDouble();
    qDebug() << "" << dayleft.wendu;
    dayleft.ganmaozhishu = dataObj.value("ganmao").toString();
    dayleft.HighestT = dayright[1].HighestT;
    dayleft.lowestT = dayright[1].lowestT;
    dayleft.Weather = dayright[1].Weather;
    dayleft.date = dayright[1].date;

    reply->deleteLater();
    //更新
    update();
}

//获取某个城市天气数据 http://t.weather.itboy.net/api/weather/city/
void MainWindow::queryW(QString cityName)
{
    QString cityCode = queryCityCode(cityName);
    if(cityCode.isEmpty()){
        QMessageBox::warning(this, "错误", "请检查城市名是否正确!", QMessageBox::Ok);
        return;
    }
    QUrl url("http://t.weather.itboy.net/api/weather/city/" + cityCode);
    networkAM->get(QNetworkRequest(url));
}

//更新数据到ui上
void MainWindow::update()
{
    ui->lblCity->setText(dayleft.city);
    ui->lblDate->setText(dayleft.date + " " + dayright[1].week);
    ui->lblGanMao->setText(dayleft.ganmaozhishu);
    ui->lblPM25->setText(QString::number(dayleft.pm25));
    ui->lblLowHigh->setText(QString::number(dayleft.lowestT) + "~" + QString::number(dayleft.HighestT) + "℃");
    ui->lblType->setText(dayleft.Weather);
    ui->lblTemp->setText(QString::number(dayleft.wendu) + "℃");
    ui->lblShiDu->setText(dayleft.shidu);
    ui->lblQuality->setText(dayleft.kongqi);
    ui->lblWindFx->setText(dayleft.fx);
    ui->lblWindFl->setText(dayleft.fl);
    ui->lblTypeIcon->setPixmap(weatherIconMap[dayleft.Weather]);

    for(int i = 0; i < 6; i++){
        weeks[i]->setText(dayright[i].week);
        dates[i]->setText(dayright[i].date.mid(5, dayright[i].date.length()));
        qDebug() << "" << dates[i];
        weathers[i]->setText(dayright[i].Weather);
        weatherIcons[i]->setStyleSheet("border-image: url(" + weatherIconMap[dayright[i].Weather] + ");");
        highs[i]->setText("高温 " + QString::number(dayright[i].HighestT) + "℃" );
        lows[i]->setText("低温 " + QString::number(dayright[i].lowestT) + "℃" );
        if(dayright[i].kongqi > 0 && dayright[i].kongqi <= 50){
            kongqis[i]->setStyleSheet("color: rgb(85, 255, 0);");
            kongqis[i]->setText("优");
        }
        if(dayright[i].kongqi > 50 && dayright[i].kongqi <= 100){
            kongqis[i]->setStyleSheet("color: rgb(205, 255, 207);");
            kongqis[i]->setText("良");
        }
        if(dayright[i].kongqi > 100 && dayright[i].kongqi <= 150){
            kongqis[i]->setStyleSheet("color: rgb(255, 0, 127);");
            kongqis[i]->setText("轻度污染");
        }

        if(dayright[i].kongqi > 150 && dayright[i].kongqi <= 200){
            kongqis[i]->setStyleSheet("color: rgb(255, 0, 0);");
            kongqis[i]->setText("中度污染");
        }
        if(dayright[i].kongqi > 200 && dayright[i].kongqi <= 250){
            kongqis[i]->setStyleSheet("color: rgb(170, 85, 0);");
            kongqis[i]->setText("重度污染");
        }
        if(dayright[i].kongqi > 250 && dayright[i].kongqi <= 300){
            kongqis[i]->setStyleSheet("color: rgb(170, 0, 0);");
            kongqis[i]->setText("严重污染");
        }
    }
    weeks[1]->setText("今天");
}

void MainWindow::getLocalCity()
{
    const QString& strUrl=QString("http://api.map.baidu.com/location/ip?ak=W1f9o2ZdMGtbjy8Vb5j6D8NX5GeCbDfO&coor=bd09ll");

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setUrl(QUrl(strUrl));
    QNetworkReply* reply =m_pNet->get(request);
    connect(reply,SIGNAL(finished()),this,SLOT(CityReplyFinished()));
    qDebug() << "" << cityName;
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

QString MainWindow::queryCityCode(QString cityName)
{
    QMap<QString, QString>::iterator iterator = getCityCode.cityCodeMap.find(cityName);
    if(iterator == getCityCode.cityCodeMap.end()) iterator = getCityCode.cityCodeMap.find(cityName + "市");

    if(iterator != getCityCode.cityCodeMap.end()) return iterator.value();

    return "";
}

void MainWindow::CityReplyFinished()
{
    QNetworkReply* reply=qobject_cast<QNetworkReply *>(sender());
    qDebug() << "11111";
    if(reply->error()!=QNetworkReply::NoError){
            QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("请求出错:%1").arg(reply->errorString()));
    }

    //解析城市数据
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QByteArray arr = reply->readAll();
    QByteArray msgUtf8 = code->fromUnicode(arr);//转码后的数据msgUtf8
    qDebug() << "" << msgUtf8.data();
    QJsonParseError error;
    QJsonDocument docu = QJsonDocument::fromJson(arr, &error);
    if(error.error!=QJsonParseError::NoError){
        QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("JSON:数据结构有问题！"));
        return;
    }
    QJsonObject obj = docu.object();
    QJsonObject contentObj = obj.value("content").toObject();
    QJsonObject detailObj = contentObj.value("address_detail").toObject();
    cityName = detailObj.value("city").toString();
    cityName = cityName.mid(0, cityName.length() - 1);
    reply->deleteLater();
    queryW(cityName);
    qDebug() << "" << cityName;
}

//查询事件
void MainWindow::on_btnSearch_clicked()
{
    QString cityName = ui->leCity->text();
    queryW(cityName);
}
