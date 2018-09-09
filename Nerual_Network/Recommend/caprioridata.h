#ifndef CAPRIORIDATA_H
#define CAPRIORIDATA_H


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

#define MAXLINE 1000
#define PERCENT1 0.1
#define MAXCLASS   4           ///< 最大分类个数

typedef vector<double> datapoint;

/**
 * 对应go_track_tracks.csv文件
 **/
typedef struct {
    int i32Index;           ///< 序号
    int i32Id;            ///< id
    int i32Id_Android;     ///< id_android
    double dSpeed;            ///< speed
    double dTime;            ///< time
    double dDistance;            ///< distance
    int i32Rating;             ///< rating
    int i32Rating_bus;             ///< rating_bus
    int i32Rating_weather;             ///< rating_weather
    int dCarORBus;                  /// car or bus
    QString StrLinha;           /// linha
}ST_GPS_DATA;

/**
 * 对应go_track_trackspoint文件
 **/
typedef struct {
    double dId;         ///< id
    double dLatitude;   ///< latitude
    double dLongtitude; ///< longtitude
    int i32Track_Id;   ///< track_id;
    QString StrTime;    ///< time
}ST_GPS_POINT;

/**
 * 信息分类
 **/
typedef struct {
    ST_GPS_DATA stGPSData;
    QVector<ST_GPS_POINT> VecStGPSPoint;
}ST_DEVICE_DATA;

class CAprioriData
{
public:
    datapoint training_point[4];                              ///< 训练数据
    vector<datapoint> training_data;                           ///< 训练数据
    vector<int> training_labels;                            ///< 训练标签


public:

    CAprioriData();
    ~CAprioriData();

    int ReadFile();
    int loadTrainingData();
    int WriteFile();
#if 0
    int buildDataBase();
    int DealData();
    int GetItem1();
    int DealData2();
//    int GetItem2();
    int DealItem2();
#endif

private:
    QVector<ST_GPS_POINT> VecStGPSPoint;
    QMap<int, QVector<ST_GPS_POINT>> MapVecGPSPoint;        ///< track_id和ST_GPS_POINT对应
    QVector<ST_DEVICE_DATA> VecStDevData;                           ///< 信息归类
    QMap<int, ST_DEVICE_DATA> MapIntDeviceData;             ///< track_id和ST_DEVICE_DATA对应

};

#endif // CAPRIORIDATA_H
