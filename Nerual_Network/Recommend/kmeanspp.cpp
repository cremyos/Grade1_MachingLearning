#include "kmeanspp.h"

KmeansPP::KmeansPP(const std::vector<datapoint> &input_data)
    : input_data(input_data)
{}

#if 0
KmeansPP::KmeansPP(const std::vector<datapoint> &input_data)
    : input_data(input_data), random_index_gen(0, input_data.size()-1), random_real_gen(0, 1), random_engine(rd())
{}

KmeansPP::~KmeansPP(void)
{}



/**
 * @brief KmeansPP::RunKMeansPP
 * @param K
 * @param centroids
 * @return
 * 运行k-means++算法，
 * 在数据集中随机选择一个样本点作为第一个初始化的聚类中心
 * 选择出其余的聚类中心：
 *         计算样本中的每一个样本点与已经初始化的聚类中心之间的距离，并选择其中最短的距离，记为d_i
 *         以概率选择距离最大的样本作为新的聚类中心，重复上述过程，直到k个聚类中心都被确定
 * 对k个初始化的聚类中心，利用K-Means算法计算最终的聚类中心。
 **/
vector<vector<int> > KmeansPP::RunKMeansPP(int K, vector<datapoint> &centroids)
{

//    qDebug()<<"k = " + QString::number(K) + "input_data.size() = " + QString::number(input_data.size());
    assert(K<=input_data.size());
    vector<vector<int> > clusters_vec(K);

    /**
     * 选择初始的聚类中心初始化
     */
    init();
    int first_centroid = random_index_gen(random_engine);
    for(int i32TempIndex0 = 0; i32TempIndex0 < MAXCLASS; i32TempIndex0++) {
//        initial_centroids_.push_back(input_data[i32TempIndex0].at(first_centroid));
        datapoint TempData;
        TempData.push_back(input_data[i32TempIndex0].at(first_centroid));
        initial_centroids_.push_back(TempData);
    }

#if 1
    qDebug()<<"Print$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
//        vector<datapoint> initial_centroids_;
    for(int i32TempIndex1 = 0; i32TempIndex1 < initial_centroids_.size(); i32TempIndex1++) {
        qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1);
        datapoint TempVecDataPoint = initial_centroids_.at(i32TempIndex1);
        for(int i32TempIndex2 = 0; i32TempIndex2 < TempVecDataPoint.size(); i32TempIndex2++) {
            qDebug()<<"i32TempIndex2 = " + QString::number(i32TempIndex2) + " || " + QString::number(TempVecDataPoint.at(i32TempIndex2));
        }
    }
    qDebug()<<"+++++++++++++++++++++++++++++++++++";
#endif

    for(int i=1 ; i<K ; i++)
    {
#if 0
        qDebug()<<"Print";
//        vector<datapoint> initial_centroids_;
        for(int i32TempIndex1 = 0; i32TempIndex1 < initial_centroids_.size(); i32TempIndex1++) {
            qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1);
            datapoint TempVecDataPoint = initial_centroids_.at(i32TempIndex1);
            for(int i32TempIndex2 = 0; i32TempIndex2 < TempVecDataPoint.size(); i32TempIndex2++) {
                qDebug()<<"i32TempIndex2 = " + QString::number(i32TempIndex2) + " || " + QString::number(TempVecDataPoint.at(i32TempIndex2));
            }
        }
        qDebug()<<"++++++++++++++++++++++++++++++++++++";
#endif
        updateNearestClusters(initial_centroids_);
        initial_centroids_.push_back(input_data[getNextInitialCentroidIndex()]);
        qDebug()<<"First updateNearestClusters END";
    }

#if 0
    /**
     * 打印聚类结果
     **/
    qDebug()<<"Print57:--------------------------------";
    for(int i32TempIndex1 = 0; i32TempIndex1 < 1; i32TempIndex1++) {
        qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1);
//        datapoint TempVecDataPoint = initial_centroids_.at(i32TempIndex1);
//        qDebug()<<"TempVecDataPoint.size = " + QString::number(TempVecDataPoint.size());
#if 1
        datapoint TempVecDataPoint = initial_centroids_.at(i32TempIndex1);
        for(int i32TempIndex2 = 0; i32TempIndex2 < TempVecDataPoint.size(); i32TempIndex2++) {
            qDebug()<<"i32TempIndex2 = " + QString::number(i32TempIndex2) + " || " + QString::number(TempVecDataPoint.at(i32TempIndex2));
        }
#endif
    }
    qDebug()<<"-------------------------------------";
#endif


    /**
     * 第二次：继续K-means++聚类算法
     **/
    cur_centroids_ = initial_centroids_;
    do {

        prev_centroids_ = cur_centroids_;
#if 0
    /**
     * 打印聚类结果
     **/
    qDebug()<<"Print84:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    for(int i32TempIndex1 = 0; i32TempIndex1 < 1; i32TempIndex1++) {
        qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1);
//        datapoint TempVecDataPoint = initial_centroids_.at(i32TempIndex1);
//        qDebug()<<"TempVecDataPoint.size = " + QString::number(TempVecDataPoint.size());
#if 1
        datapoint TempVecDataPoint = cur_centroids_.at(i32TempIndex1);
        for(int i32TempIndex2 = 0; i32TempIndex2 < 30; i32TempIndex2++) {
            qDebug()<<"i32TempIndex2 = " + QString::number(i32TempIndex2) + " || " + QString::number(TempVecDataPoint.at(i32TempIndex2));
        }
#endif
    }
    qDebug()<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
#endif


        updateNearestClusters(cur_centroids_);
        updateCentroids(cur_centroids_);

    }while(!equalCentroids(cur_centroids_,prev_centroids_));

    qDebug()<<"After While";

#if 0
    /**
     * 打印聚类结果
     **/
    qDebug()<<"Print91:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    for(int i32TempIndex1 = 0; i32TempIndex1 < 1; i32TempIndex1++) {
        qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1);
//        datapoint TempVecDataPoint = initial_centroids_.at(i32TempIndex1);
//        qDebug()<<"TempVecDataPoint.size = " + QString::number(TempVecDataPoint.size());
#if 1
        datapoint TempVecDataPoint = cur_centroids_.at(i32TempIndex1);
        for(int i32TempIndex2 = 0; i32TempIndex2 < 300; i32TempIndex2++) {
            qDebug()<<"i32TempIndex2 = " + QString::number(i32TempIndex2) + " || " + QString::number(TempVecDataPoint.at(i32TempIndex2));
        }
#endif
    }
    qDebug()<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
#endif


    /**
     * 将结果存入聚类向量中
     **/
    for(int i = 0 ; i<input_data.size() ; i++) {
        qDebug()<<" Center = " + QString::number(nearest_cluster_idx[i]);
        clusters_vec[nearest_cluster_idx[i]].push_back(i);
    }
    centroids = cur_centroids_;




#if 0
    /**
     * 打印聚类结果
     **/
    qDebug()<<"Print:++++++++++++++++++++++++++++++++++++";
    for(int i32TempIndex1 = 0; i32TempIndex1 < 1; i32TempIndex1++) {
        qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1);
        datapoint TempVecDataPoint = centroids.at(i32TempIndex1);
        qDebug()<<"TempVecDataPoint.size = " + QString::number(TempVecDataPoint.size());
#if 1
//        datapoint TempVecDataPoint = centroids.at(i32TempIndex1);
        for(int i32TempIndex2 = 0; i32TempIndex2 < 3000; i32TempIndex2++) {
            qDebug()<<"i32TempIndex2 = " + QString::number(i32TempIndex2) + " || " + QString::number(TempVecDataPoint.at(i32TempIndex2));
        }
#endif
    }
    qDebug()<<"+++++++++++++++++++++++++++++++++++++++";
#endif

    return clusters_vec;

}

/**
 * @brief KmeansPP::getNextInitialCentroidIndex
 * @return
 * 根据概率D(x)/SIGMA(D(x))增加剩下的点，D(x)表示x距离他最近的聚类中心的距离
 */
int KmeansPP::getNextInitialCentroidIndex()
{

    /**
     * 计算错误和，SIGMA(D(x)）
     */
    double total_distance=0;
    for (int i = 0; i < input_data.size() ; ++i)
        total_distance += nearest_cluster_dist[i];

    /**
     * 计算概率D(x)/SIGMA(D(x))
     */
    vector<double> cumm_prob(input_data.size(),0);
    for (int i = 0; i < input_data.size() ; ++i)
        cumm_prob[i] = (nearest_cluster_dist[i] / total_distance);

    /**
     * 累计求和
     **/
    for (int i = 1; i < input_data.size() ; ++i)
        cumm_prob[i] += cumm_prob[i-1];

    /**
     * 选择下一个点可能性为 D(x)/SIGMA(D(x))
     */
    int rand_num = random_real_gen(random_engine);
    for (int i = 0; i < cumm_prob.size() ; ++i) {
        if (rand_num < cumm_prob[i]) {
            return i;
        }
    }

    return cumm_prob.size()- 1;
}

/**
 * @brief KmeansPP::updateNearestClusters
 * @param centroids_
 * 更新最近的聚类中心
 */
void KmeansPP::updateNearestClusters(const vector<datapoint> &centroids_)
{
    for (int i = 0; i < input_data.size() ; ++i)
    {
        int idx = getClosestCentroidIndex(i, centroids_);
        nearest_cluster_idx[i] = idx;
        nearest_cluster_dist[i] = distance(centroids_[idx],input_data[i]);
    }
}

/**
 * @brief KmeansPP::updateCentroids
 * @param centroids_
 * 更新聚类中心
 */
void KmeansPP::updateCentroids(const vector<datapoint> &centroids_)
{
    vector<int>freq(centroids_.size(),0);

    int i32MaxSize = 0;
#if 1
    for(int i32TempIndex = 0; i32TempIndex < input_data.size(); i32TempIndex++) {
//        qDebug()<<"Line 101: centroids_[ " + QString::number(i32TempIndex) + " ].size() = " + QString::number(centroids_[i32TempIndex].size());
        if(i32MaxSize <= input_data[i32TempIndex].size()) {
            i32MaxSize = input_data[i32TempIndex].size();
        }
    }
//    vector<datapoint> new_centroids(centroids_.size(), vector<double>(centroids_[0].size(),0));

    vector<datapoint> new_centroids(centroids_.size(), vector<double>(i32MaxSize, 0));
#endif
//    vector<datapoint> new_centroids[MAXCLASS];


    for (int i = 0; i < input_data.size() ; ++i) // training set count
    {
//        new_centroids = new vector<datapoint>[centroids_.size()][vector<double>(centroids_[i].size(),0)];
//        qDebug()<<"Line19: nearest_cluster_idx[ " + QString::number(i) + " ]  = " + QString::number(nearest_cluster_idx[i]);
        int Index = nearest_cluster_idx[i];
//        qDebug()<<"Line20: centerData = " + QString::number(input_data[i][Index]);
        ++freq[nearest_cluster_idx[i]];
//        qDebug()<<"Line 104";
        for(int j=0 ; j< input_data[i].size() ; ++j) { // feature count
//            qDebug()<<"nearest_cluster_idx[ " + QString::number(i) + " ] = " + QString::number(nearest_cluster_idx[i]);
//            qDebug()<<"Line 116: input_data[ " + QString::number(i) + " ].size() = " + QString::number(input_data[i].size());
            new_centroids[nearest_cluster_idx[i]][j] += input_data[i][j];
        }
//        qDebug()<<"Line 107";
    }

#if 1
        qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
        for(int i32TempIndex0 = 0; i32TempIndex0 < new_centroids.size(); i32TempIndex0++) {
            datapoint TempData = new_centroids.at(i32TempIndex0);
            for(int i32TempIndex1 = 0; i32TempIndex1 < TempData.size(); i32TempIndex1++){
                qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1) + QString::number(TempData.at(i32TempIndex1));
            }
        }
        qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
#endif

//    qDebug()<<"Line 110";

    for (int i = 0; i < centroids_.size() ; ++i) {
        if(freq[i]) {
            for (int j = 0; j < centroids_[i].size() ; ++j) {
//                qDebug()<<"centroids_[i].size() = " + QString::number(centroids_[i].size()) + " || freq[ " + QString::number(i) + " ] = " + QString::number(freq[i]);
                new_centroids[i][j] *= (1.0/(double)freq[i]);
            }
        }
//        qDebug()<<"Line 120";
    }
    cur_centroids_ = new_centroids;
}


/**
 * @brief KmeansPP::getClosestCentroidIndex
 * @param data_point_idx
 * @param centroids_
 * @return
 * 获得最近的聚类中心
 */
int KmeansPP::getClosestCentroidIndex(int data_point_idx, const std::vector<datapoint> &centroids_)
{
    int closest_cluster=-1;
    double minDistance=1e18;
    for(int i = 0 ; i<centroids_.size() ; ++i)
    {
//        qDebug()<<"centroids_[" + QString::number(i) + "]";
        qDebug()<<"_+_+_++_+_+Line284: data_point_idx = " + QString::number(data_point_idx);
        double dist_dp_centroid = distance(centroids_[i],input_data[data_point_idx]);
//        qDebug()<<"centroids_[ " + QString::number(i) + " ] = " + QString::number(centroids_.at(i)) + "dist_dp_centroid = " + QString::number(dist_dp_centroid);
        qDebug()<<"!!!!!!!287: dist_dp_centroid = " + QString::number(dist_dp_centroid);
        if(minDistance > dist_dp_centroid)
        {
            minDistance = dist_dp_centroid;
            closest_cluster = i;
        }
    }
//    qDebug()<<"closet_cluster = " + QString::number(closest_cluster);
    assert(closest_cluster!=-1);
    return closest_cluster;
}

/**
 * @brief KmeansPP::distance
 * @param a
 * @param b
 * @return
 * 计算距离
 */
double KmeansPP::distance(const datapoint &a , const datapoint &b)
{
    double dist=0;
#if 0
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    datapoint TempDataA = a;
    qDebug()<<"TempDataA.size() = " + QString::number(TempDataA.size());
    for(int i32TempIndex0 = 0; i32TempIndex0 < TempDataA.size(); i32TempIndex0++) {
        qDebug()<<"i32TempIndex0 = " + QString::number(i32TempIndex0) + " || " + QString::number(TempDataA.at(i32TempIndex0));
    }
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";

    qDebug()<<"##############################################################";
    datapoint TempDataB = b;
    qDebug()<<"TempDataB.size() = " + QString::number(TempDataB.size());
    for(int i32TempIndex0 = 0; i32TempIndex0 < TempDataB.size(); i32TempIndex0++) {
        qDebug()<<"i32TempIndex0 = " + QString::number(i32TempIndex0) + " || " + QString::number(TempDataB.at(i32TempIndex0));
    }
    qDebug()<<"##############################################################";

#endif

#if 1
    /**
     * MY
     **/
    for(int i = 0 ; i< a.size() ; i++) {
        qDebug()<<"Line352: a.size = " + QString::number(a.size());
        /**
         * a 是聚类中心点，b 是聚类点，计算两者的距离平方和。
         **/
        for(int j = 0; j < b.size(); j++) {
            dist += (b[j]-a[i])*(b[j]-a[i]);
        }
    }
#endif

    return dist;
}


/**
 * @brief KmeansPP::equalCentroids
 * @param centroid_vec_a
 * @param centroid_vec_b
 * @return
 * 判断距离是否符合条件
 */
bool KmeansPP::equalCentroids(const vector<datapoint> &centroid_vec_a , const vector<datapoint> &centroid_vec_b)
{
    double total_dist=0;
    for(int i = 0 ; i< centroid_vec_a.size() ; i++) {
        total_dist += distance(centroid_vec_a[i],centroid_vec_b[i]);
    }

    qDebug()<<"Line 259: total_dist = " + QString::number(total_dist);
//    return (total_dist<1e-9);
//    return (total_dist<1e+7);
    return (total_dist > 2e+9);
}

/**
 * 初始化聚类中心
 */
void KmeansPP::init()
{
    nearest_cluster_idx.assign(input_data.size(),-1);
    nearest_cluster_dist.assign(input_data.size(),0);
}

#endif


//KmeansPP::KmeansPP(const std::vector<datapoint> &input_data)
//{}

KmeansPP::~KmeansPP(void)
{}


/**
 * @brief KmeansPP::init
 * 初始化聚类中心
 */
void KmeansPP::init()
{
    for(int i32TempIndex = 0; i32TempIndex < input_data.size(); i32TempIndex++) {
        datapoint TempData = input_data.at(i32TempIndex);
        initial_centroids_.push_back(TempData.at(0));
    }

#if 1
    /**
     * 打印初始聚类中心
     **/
    qDebug()<<"Init Cluster center: ";
    for(int i32TempIndex = 0; i32TempIndex < initial_centroids_.size(); i32TempIndex++) {
        qDebug()<<"i32TempIndex = " + QString::number(i32TempIndex) + " || center = " + QString::number(initial_centroids_[i32TempIndex]);
    }

#endif

    cur_centroids_ = initial_centroids_;
    qDebug()<<"InitClusterFinish";
}

void KmeansPP::assignCentroid(vector<datapoint> input_data)
{
    double smallest = 1e+9;
    int chosenCentroid = 999;

    vector<datapoint> TempVecDatapoint;
    for(int i32TempInputDataIndex = 0; i32TempInputDataIndex < input_data.size(); i32TempInputDataIndex++) {
        datapoint TempData = input_data.at(i32TempInputDataIndex);
        datapoint TempClusterData;
        for(int i32TempInputDataIndex2 = 0; i32TempInputDataIndex2 < TempData.size(); i32TempInputDataIndex2++) {
            for(int i32TempIndex = 0; i32TempIndex < i32ClusterCount; i32TempIndex++) {
                double distanceToCentroid = calculateDistance(TempData.at(i32TempInputDataIndex2), cur_centroids_[i32TempIndex]);
                if(smallest > distanceToCentroid) {
                    /**
                     * 选择距离中心最近的点更新最小距离
                     **/
                    smallest = distanceToCentroid;
                    chosenCentroid = i32TempIndex;
                }

            }
//            ClusterData[chosenCentroid][i32TempInputDataIndex2] = TempData.at(i32TempInputDataIndex2);
            TempClusterData.push_back(TempData.at(i32TempInputDataIndex2));
        }
        TempVecDatapoint.push_back(TempClusterData);
    }
    ClusterData = TempVecDatapoint;
}


datapoint KmeansPP::calcNewCentroid()
{
    datapoint TempCurCenter;
    /**
     * 重新计算聚类中心
     **/
    for(int i32TempIndex = 0; i32TempIndex < ClusterData.size(); i32TempIndex++) {
        datapoint TempData = ClusterData.at(i32TempIndex);
        double sum = 0;
        for(int i32TempIndex2 = 0; i32TempIndex2 < TempData.size(); i32TempIndex2++) {
            sum += TempData.at(i32TempIndex2);
        }
        int i32TempCount = TempData.size();
        qDebug()<<"i32TempCount = " + QString::number(i32TempCount);
        TempCurCenter.push_back(sum/i32TempCount);
    }
    return TempCurCenter;
}


double KmeansPP::calculateDistance(double x, double y)
{
    return sqrt((x - y) * (x - y));
}


std::vector<datapoint> KmeansPP::RunKMeansPP(int K)
{
    i32ClusterCount = K;

    datapoint TempDataCenter;
    vector<datapoint> TempCenter;

    init();
    while(1) {
        assignCentroid(input_data);
        qDebug()<<"Finish assignCentroid";
        TempDataCenter = calcNewCentroid();

        qDebug()<<"Judge Cluster";
        /**
         * 判断聚类结果产生的新旧类中心距离差是否满足要求
         **/
        double TempDist[MAXCLASS] = {0.0};
//        TempDist = new double[i32ClusterCount];
//        memset(TempDist, 0.0, sizeof(double) * i32ClusterCount);
        for(int i32TempIndex = 0; i32TempIndex < i32ClusterCount; i32TempIndex++) {
            TempDist[i32TempIndex] = calculateDistance(TempDataCenter.at(i32TempIndex), cur_centroids_.at(i32TempIndex));
        }

        qDebug()<<"Show Dis ++++++++++++++++++++++++++";
        for(int i32TempIndex= 0; i32TempIndex < i32ClusterCount; i32TempIndex++) {
            qDebug()<<"TempDist[ " + QString::number(i32TempIndex) + " ] = " + QString::number(TempDist[i32TempIndex]);
        }
        qDebug()<<"+++++++++++++++++++++++++++++++++++";

        if((TempDist[0] <= 9) && (TempDist[1] <= 11) && (TempDist[2] <= 2) && (TempDist[3] <= 14.0)) {
//            centroids = ClusterData;
            qDebug()<<"Finish Cluster";

            for(int i32TempClusterIndex = 0; i32TempClusterIndex < i32ClusterCount; i32TempClusterIndex++) {
                datapoint TempDataPointTemp;
                for(int i32TempIndexDatapoint = 0; i32TempIndexDatapoint < TempDataCenter.size(); i32TempIndexDatapoint++) {
                    TempDataPointTemp.push_back(TempDataCenter.at(i32TempIndexDatapoint));
                }
                TempCenter.push_back(TempDataPointTemp);
            }


            break;
        }
    }

    return ClusterData;

}
