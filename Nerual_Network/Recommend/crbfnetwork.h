#ifndef CRBFNETWORK_H
#define CRBFNETWORK_H


#include<vector>
#include<math.h>
#include "KmeansPP.h"
#include "Util.h"

using namespace std;


typedef std::vector<double> datapoint;

class RBFNetwork
{
public:
    RBFNetwork(const std::vector<datapoint> &training_data, const std::vector<int> &training_labels, int num_of_labels);
    ~RBFNetwork(void);

    double startTraining(int num_rbf_units, double learning_rate, int num_iterations, double &mse, bool print_flag = false);        ///< 训练


    void startTesting(const std::vector<datapoint> &testing_data, const std::vector<int> &testing_labels);          ///< 测试


    int predictLabel(const datapoint &data_point, double &error);

    //TODO
    void saveModel();
    void loadModel();

private:
    int num_of_labels;
    double gamma;
    std::vector<datapoint> training_data;
    std::vector<int> training_labels;
    std::vector< std::vector<double> > rbf_units;
    std::vector< std::vector<double> > layer2_weights;
    std::vector<datapoint> rbf_centroids;

    std::random_device rd;
    std::default_random_engine random_engine;
    std::uniform_real_distribution<double> random_real_gen;

    void buildRBFUnits();
    void calculateGamma();
    double distance(const datapoint &a ,const datapoint &b);
    double basisFunction(const datapoint &data_point, const datapoint &centroid);

};



#endif // CRBFNETWORK_H
