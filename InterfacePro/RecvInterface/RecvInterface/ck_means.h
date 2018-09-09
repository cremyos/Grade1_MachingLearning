#ifndef CK_MEANS_H
#define CK_MEANS_H


#include <string.h>
#include <QDebug>
#include <string>
#include <map>
#include <iostream>
#include <QVector>
#include "c_apriori.h"

using namespace std;

#define MAXCLUSTER      10          /// 最大多少类
#define MAXDATACOUNT    50          /// 最大数据量，50个，用于循环队列

/**
 * 类型和数据
 **/
typedef struct {
    int i32Type;                    ///< 类型
    double dData;                   ///< 数据
}ST_TYPE_DATA;

/**
 * K-means聚类
 */
class CK_means
{
public:
    double *assignSampleCenter;  ///< 样本中心
    double Samples[MAXDATACOUNT];           ///< 存放样本数据的数组
    double centerX[MAXCLUSTER];             ///< 中心点
    int k;                                  ///< 初始的中心点的个数
    double classK[MAXCLUSTER][MAXDATACOUNT];    ///< 多少个簇，每个簇多少个元素，每个簇对应一个循环队列
    int i32SampleIndex;                         ///< 样本计数
//    QVector<ST_TYPE_DATA> vecTpDat;             ///<
    ST_TYPE_DATA *stTypeData;               ///< 结构体数组
    map<int, int> MapInt;                   ///< 第一个key为序号，第二个元素是类
     C_Apriori cMyApri;


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
