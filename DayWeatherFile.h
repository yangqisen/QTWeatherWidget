#ifndef DAYWEATHERFILE_H
#define DAYWEATHERFILE_H
#include <QString>


class DayLeft{
public:
    DayLeft(){
        date = "2022-12-21";
    }
    QString city;
    QString ganmaozhishu;//感冒指数
    QString date;
    int wendu;
    double pm25;
    QString shidu;
    QString kongqi;
    QString Weather; //气象，如多云、小雨等
    int HighestT;//最高温度
    int lowestT;//最低温度
    QString fx;
    QString fl;
};

class DayRight{
public:
    QString date;
    QString week; //星期几
    int kongqi;
    QString Weather; //气象，如多云、小雨等
    int HighestT;//最高温度
    int lowestT;//最低温度
};

#endif // DAYWEATHERFILE_H


