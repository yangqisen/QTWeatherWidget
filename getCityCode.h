#ifndef GETCITYCODE_H
#define GETCITYCODE_H
#include <QMap>
#include <QFile>
#include <QDebug>

class getCityCode{
public:
    QMap<QString, QString> cityCodeMap;

    //读取城市编码并放入map中
    getCityCode(){
        QFile file("E:/QT/code/WeatherWidget/cityCode.txt");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"Can't open the file!"<<endl;
        }
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString s = QString(line);
            QString city = s.split("=").at(0);
            city = city.mid(0, city.length()-1);
            qDebug() << "" << city;
            QString code = s.split("=").at(1);
            code = code.mid(1, code.length()-2);
            qDebug() << "" << code;
            cityCodeMap.insert(city, code);
        }
    }


};

#endif // GETCITYCODE_H


