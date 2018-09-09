#ifndef CK_MEANS_H
#define CK_MEANS_H


#include <string.h>
#include <QDebug>
#include <string>
#include <iostream>
#include <map>

using namespace std;


#define MAXCLUSTER      10          /// 最大多少类
#define MAXDATACOUNT    50          /// 最大数据量，50个，用于循环队列


typedef struct {
    int i32Type;                ///< 数据类型码
    double dData;               ///< 数据值
}ST_TYPE_DATA;

typedef struct {
    int i32Class;               ///< 类
    ST_TYPE_DATA *stTypedata;   ///< ST_TYPE_DATA结构体数组
}ST_CLASS_TYPE_DATA;

/**
 * K-means聚类
 */
class CK_means
{
public:
    double *assignSampleCenter;  ///< 样本中心
    double Samples[MAXDATACOUNT];           ///< 存放样本数据的数组
    double centerX[MAXCLUSTER];             ///< 中心点
    double oldcenterX[MAXCLUSTER];          ///< 旧的中心点
    int k;                                  ///< 初始的中心点的个数
    int centroidcount[MAXCLUSTER];          ///< 每一个簇的个数
    double classK[MAXCLUSTER][MAXDATACOUNT];    ///< 多少个簇，每个簇多少个元素，每个簇对应一个循环队列
    int i32SampleIndex;                         ///< 样本计数
    ST_TYPE_DATA *stTypeData;                ///< 数据类型和数据结构体

//    ST_CLASS_TYPE_DATA stClassTypedata[MAXCLUSTER];        ///< 类、样本、类型结构体


public:
    CK_means();
    ~CK_means();

    void AddSamples(int i32Type, double x);
    double calculateDistance(double x1, double x2);
    void assignCentroid(double x1, int point);
    void calculateNewCentroid();
    void ShowData();
};

#endif // CK_MEANS_H
