#ifndef CMYAPRIORI_H
#define CMYAPRIORI_H


#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <QDebug>
#include <QVector>
#include <QMap>
#include <QPair>

using namespace std;

#define MAXLINE 100
#define PERCENT1 0.1

typedef struct {
    int i32Index;           ///< 序号
    QString Date;            ///< 时间
    double dPM2_5;          ///< PM2.5
    double DEWP;            ///< Dew Point
    double TEMP;            ///< Temperature
    double PRES;            ///< Pressure
    double dIs;             ///< Cumulated hours of snow
    double dIr;             ///< Cumulated hours of rain
}ST_PM_DATA;

/**
 * PM2.5与double数据，用于map，map的key和value之间的对应是，0，DEWP；1，TEMP；2，PRES；3，dIs；4，dIr;
 **/
typedef struct {
    QString Date;           ///< 时间
    double dPM2_5;          ///< PM2.5
    double dData;            ///< double类型数据,
}ST_PM_DOUDATA;

/**
 * PM2.5与DEWP
 **/
typedef struct {
    QString Date;           ///< 时间
    double dPM2_5;          ///< PM2.5
    double DEWP;            ///< Dew Point
}ST_PM_DEWP;


/**
 * PM2.5 与 TEMP
 **/
typedef struct {
    QString Date;           ///< 时间
    double dPM2_5;          ///< PM2.5
    double TEMP;            ///< Dew Point
}ST_PM_TEMP;


/**
 * PM2.5 与 PRES
 **/
typedef struct {
    QString Date;           ///< 时间
    double dPM2_5;          ///< PM2.5
    double PRES;            ///< Dew Point
}ST_PM_PRES;


/**
 * PM2.5 与 dIr
 **/
typedef struct {
    QString Date;           ///< 时间
    double dPM2_5;          ///< PM2.5
    double dIr;            ///< Dew Point
}ST_PM_dIr;

/**
 * PM2.5 与 dIs
 **/
typedef struct {
    QString Date;           ///< 时间
    double dPM2_5;          ///< PM2.5
    double dIs;            ///< Dew Point
}ST_PM_dIs;

/**
 * 事务名称和数量的集合
 **/
typedef struct {
    QString MatterName;         ///< 事务名称
    int i32Count;               ///< 事务数量
    int i32Increase;            ///< 数据增加
}ST_MATTER_COUNT;



class CMyApriori
{
public:
    CMyApriori(QString TempdataFileName){
        dataFileName = TempdataFileName;

        memset(DataCode, 0x0, sizeof(unsigned char) * 10);
        memset(ItemCode, -1, sizeof(char) * 5);
        i32ItemPoint = 0;
    }
    ~CMyApriori();

    int ReadFile();
    int buildDataBase();
    int DealData();
    int GetItem1();
    int DealData2();
//    int GetItem2();
    int DealItem2();

private:
    unsigned char DataCode[10];                 ///< 数据编码，表示5种数据对中的PM2.5和其他数据的关系
    char ItemCode[5];                           ///< 项目编码
    QString dataFileName;
    QMap<int, ST_MATTER_COUNT> TextDataBase;        ///< 事务数据库
    /// ，0，DEWP；1，TEMP；2，PRES；3，dIs；4，dIr;
    QVector<ST_PM_DOUDATA> vecDouData_DEWP;             ///< 存储对应项的数据
    QVector<ST_PM_DOUDATA> vecDouData_TEMP;             ///< 存储对应项的数据
    QVector<ST_PM_DOUDATA> vecDouData_PRES;             ///< 存储对应项的数据
    QVector<ST_PM_DOUDATA> vecDouData_dIs;             ///< 存储对应项的数据
    QVector<ST_PM_DOUDATA> vecDouData_dIr;             ///< 存储对应项的数据

    QMap<int, QVector<ST_PM_DOUDATA>> MapDouData;           ///< 数据项和数据匹配
    int i32ItemStartPoint;                  ///< 事务集指针，指向当前需要处理的数据集的首个元素
    int i32ItemPoint;                       ///< 事务集指针，指向map的最后一个元素

    vector<ST_PM_DATA> vecSTPM;             ///< 北京PM2.5数据

};

#endif // CMYAPRIORI_H
