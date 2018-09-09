#ifndef KMEANSPP_H
#define KMEANSPP_H

#include <vector>
#include <random>
#include <assert.h>
#include <QDebug>

using namespace std;

#define MAXCLASS 4

typedef std::vector<double> datapoint;

class KmeansPP
{

public:


    KmeansPP(const std::vector<datapoint> &input_data);             ///< 从输入样本中获取每一个数据

    ~KmeansPP(void);

//    std::vector<std::vector<int> > RunKMeansPP(int K, std::vector<datapoint> &centroids);
    std::vector<datapoint> RunKMeansPP(int K);


private:

    int i32ClusterCount;                                ///< 聚类的个数
    std::vector<datapoint> input_data;

//    std::vector<int> nearest_cluster_idx;
//    std::vector<double> nearest_cluster_dist;
//    std::vector<datapoint>initial_centroids_;
    datapoint initial_centroids_;                       ///< 初始聚类中心
//    std::vector<datapoint> cur_centroids_ ;
    datapoint cur_centroids_;
    vector<datapoint> ClusterData;                      ///< 聚类后的结果
    std::vector<datapoint> prev_centroids_;


//    std::random_device rd;
//    std::default_random_engine random_engine;
//    std::uniform_int_distribution<int> random_index_gen;
//    std::uniform_real_distribution<double> random_real_gen;


    void init();
    void assignCentroid(vector<datapoint> input_data);
    datapoint calcNewCentroid();
    double calculateDistance(double x, double y);
//    void updateNearestClusters(const std::vector<datapoint> &centroids_);
//    void updateCentroids(const std::vector<datapoint> &centroids_);
//    int getNextInitialCentroidIndex();
//    int getClosestCentroidIndex(int data_point_idx, const std::vector<datapoint> &centroids_);
//    double distance(const datapoint &a , const datapoint &b);
//    bool equalCentroids(const std::vector<datapoint> &a , const std::vector<datapoint> &b);
//    datapoint getMeanCentroid(const std::vector<datapoint> &centroids_);

};

#endif // KMEANSPP_H
