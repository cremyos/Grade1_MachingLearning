#include "crbfnetwork.h"

RBFNetwork::RBFNetwork(const vector<datapoint> &training_data, const vector<int> &training_labels, int num_of_labels)
    : training_data(training_data), training_labels(training_labels), num_of_labels(num_of_labels)
    , random_real_gen(-1, 1), random_engine(rd())
{}

RBFNetwork::~RBFNetwork(void)
{
}


/**
 * @brief RBFNetwork::startTraining
 * @param num_rbf_units
 * @param learning_rate
 * @param num_iterations
 * @param mse
 * @param print_flag
 * @return
 * 开始训练，径向基网络，获取大量的RBF中心数据点、学习率、迭代次数、是否打印，保存输出模型做出简单的预测，返回ACC和MSE
 */
double RBFNetwork::startTraining(int num_rbf_units, double learning_rate, int num_iterations, double &mse, bool print_flag)
{
    if(print_flag)
    {
        printf("Starting RBF Network Training with %d units and learning rate=%f...\n", num_rbf_units, learning_rate);
        printf("Getting RBF Centroids using K-means++...\n");
    }


    /**
     * 计算聚类中心
     */
    KmeansPP KMPP(training_data);
    rbf_centroids.clear();
    rbf_centroids = KMPP.RunKMeansPP(num_rbf_units);

#if 0
    qDebug()<<"Print RBF_centroids:";
    for(int i32TempIndex = 0; i32TempIndex < rbf_centroids.size(); i32TempIndex++) {
        datapoint TempRBF = rbf_centroids.at(i32TempIndex);
        for(int i32TempIndex1 = 0; i32TempIndex1 < TempRBF.size(); i32TempIndex1++) {
            qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1) + " || " + QString::number(TempRBF.at(i32TempIndex1));
        }
    }
    qDebug()<<"++++++++++++++++++++++++++++++++++";
#endif

    calculateGamma();

    if(print_flag) {
        printf("Building the RBF Units...\n");
    }

    /**
     * 构建第一层网络单元
     **/
    buildRBFUnits();

    /**
     * 初始化第二层单元
     **/
    layer2_weights.assign(num_of_labels, vector<double>(num_rbf_units,0));
    for(auto &label: layer2_weights) {

        for(auto &arc: label) {

            arc = random_real_gen(random_engine);
        }
    }

    /**
     * 训练第二层网络
     **/
    mse = 0;
    double accuracy=0;
    for(int iter = 0 ; iter<num_iterations ; iter++)
    {
        for(int i = 0 ; i<training_data.size() ; i++)
        {
            for(int label = 0 ; label<num_of_labels ; label++)
            {
                double hypothesis = Utility::multiplyVectors(rbf_units[i], layer2_weights[label]);
                hypothesis = max(min(hypothesis,+1.0),-1.0);
                double truth_base = (training_labels[i]==label) ? +1.0 : -1.0;
                double error_direction = (truth_base-hypothesis);
                vector<double> delta = Utility::multiplyVecConst(rbf_units[i], error_direction * learning_rate);
                Utility::AddVectors(layer2_weights[label], delta);
            }
        }

        if(print_flag)
        {
            /**
             * 收集数据
             **/
            mse = 0.0;
            accuracy=0;
            vector<int> labelFreq(num_of_labels);
            for(int i = 0 ; i<training_data.size() ; i++)
            {
                double error_dir = 0;
                int prediction = predictLabel(training_data[i], error_dir);
                ++labelFreq[prediction];
                qDebug()<<"Compare Predition = " + QString::number(prediction) + " || training_labels = " + QString::number(training_labels[i]);
                if(prediction == training_labels[i])accuracy++;
                mse += error_dir * error_dir;
            }
            mse *= (double)(1.0/(double)training_data.size());
            accuracy *= (double)(1.0/(double)training_data.size());
            printf("Training (%*d/%d), MSE=[%.3f], Acc=[%.2f], Progress [%.2f]\r",
                2, (iter+1), num_iterations, mse, accuracy*100.0 , ((double)((double)(iter+1)/(double)num_iterations) * 100.0));
        }

        if(mse < 1e-9)
            break;
    }
    if(print_flag)
        printf("\n----------------------------\n");
    return accuracy;
}

void RBFNetwork::buildRBFUnits()
{
    //total_centroids_dist.clear();
    //total_centroids_dist.assign(rbf_centroids.size(),0);
    /**
     * 计算每个数据到类中心点的距离和
     **/

    //for(int i = 0 ; i<rbf_centroids.size() ; i++)
    //	for(auto &data_point : training_data)
    //		total_centroids_dist[i] += distance(data_point,rbf_centroids[i]);

    /**
     * 构建RBF单元
     **/
    rbf_units.assign(training_data.size(),vector<double>());
    for(int i = 0 ; i<training_data.size() ; i++)
    {
        for(int j = 0 ; j<rbf_centroids.size() ; j++)
            rbf_units[i].push_back(basisFunction(training_data[i], rbf_centroids[j]));
    }
}

/**
 * @brief RBFNetwork::basisFunction
 * @param data_point
 * @param centroid
 * @return
 * 径向基函数
 */
double RBFNetwork::basisFunction(const datapoint &data_point, const datapoint &centroid)
{
    return exp(gamma * distance(data_point,centroid));
}

/**
 * @brief RBFNetwork::distance
 * @param a
 * @param b
 * @return
 * 计算距离
 */
double RBFNetwork::distance(const datapoint &a, const datapoint &b)
{
    double dist=0;
    for(int i = 0 ; i< a.size() ; i++)
        dist += (a[i]-b[i])*(a[i]-b[i]);
    return dist;
}

/**
 * @brief RBFNetwork::predictLabel
 * @param data_point
 * @param error
 * @return
 * 预测标签，预测数据标签支持多分类
 */
int RBFNetwork::predictLabel(const datapoint &data_point, double &error)
{
    double maxi=-1e18;
    int bestLabel=-1;
    vector<double> cur_rbf_unit;
    for(int j = 0 ; j<rbf_centroids.size() ; j++) {
        cur_rbf_unit.push_back(basisFunction(data_point, rbf_centroids[j]));
    }
    /**
     * 获取标签最大的预测
     **/
    for(int label = 0 ; label<num_of_labels ; label++)
    {
        double hypothesis = Utility::multiplyVectors(cur_rbf_unit, layer2_weights[label]);
        if(maxi<hypothesis)
        {
            maxi = hypothesis;
            bestLabel = label;
        }
    }
    qDebug()<<"bestLabel = " + QString::number(bestLabel);
    assert(bestLabel!=-1);
    error = (double)bestLabel-maxi;
    return bestLabel;
}

/**
 * @brief RBFNetwork::calculateGamma
 * 计算gamma的值
 */
void RBFNetwork::calculateGamma()
{
    double variance = -1;
    for (int i = 0; i < rbf_centroids.size() ; ++i) {

        for (int j = i + 1; j < rbf_centroids.size() ; ++j) {
            variance = max(variance, distance(rbf_centroids[i], rbf_centroids[j]));
        }
    }
    variance *= (1.0 / ((double)rbf_centroids.size()));
    gamma = (-1.0 / (2.0 * variance));
}

/**
 * @brief RBFNetwork::startTesting
 * @param testing_data
 * @param testing_labels
 * 开始测试，确保网络不会过拟合
 */
void RBFNetwork::startTesting(const std::vector<datapoint> &testing_data, const std::vector<int> &testing_labels)
{
    printf("Testing...\n");
    double acc=0,mse=0,err=0;
    for(int i = 0 ; i<testing_data.size() ; i++) {
        if(predictLabel(testing_data[i],err)==testing_labels[i]) {
            acc++;
        }
        mse += err*err;
    }
    acc *= (1.0/(double)testing_data.size());
    mse *= (1.0/(double)testing_data.size());
    printf("Testing Results MSE = %.6f, Accuracy = %.2f\n",mse ,acc*100.0);
    printf("------------------------------\n");
}
