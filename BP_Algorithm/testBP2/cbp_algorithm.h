#ifndef CBP_ALGORITHM_H
#define CBP_ALGORITHM_H

#include <vector>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace std;

#define LAYER   3       ///< 神经网络层数
#define LAYER_COUNT 10  ///< 每层最多节点数

#define A   1
#define B   1        ///< 激活函数的参数
#define ITERS   1000    ///< 最大训练次数
#define ETA_W   0.0035  ///< 权值调整率
#define ETA_B   0.001   ///< 阈值调整率
#define ERROR   0.002   ///< 单个样本允许误差
#define ACCU    0.005   ///< 每次迭代允许的误差


typedef struct {
    vector<double>  x;      ///< 输入属性
    vector<double>  y;      ///< 输出属性
}ST_DATA;


class CBP_Algorithm
{
private:
    int in_num;                 //输入层节点数
    int ou_num;                 //输出层节点数
    int hd_num;                 //隐含层节点数

    vector<ST_DATA> data;            //样本数据
    vector<vector<double>> testdata;//测试数据
    vector<vector<double>> result;  //测试结果
    int        rowLen;            //样本数量
    int        restrowLen;        //测试样本数量

    double w[LAYER][LAYER_COUNT][LAYER_COUNT];    //BP网络的权值
    double b[LAYER][LAYER_COUNT];         //BP网络节点的阀值

    double x[LAYER][LAYER_COUNT];         //每个神经元的值经S型函数转化后的输出值，输入层就为原值
    double d[LAYER][LAYER_COUNT];         //记录delta学习规则中delta的值,使用delta规则来调整联接权重 Wij(t+1)=Wij(t)+α(Yj-Aj(t))Oi(t)

private:
    void InitNetWork();         //初始化网络
    void GetNums();             //获取输入、输出和隐含层节点数
    void ForwardTransfer();     //正向传播子过程
    void ReverseTransfer(int);  //逆向传播子过程
    void CalcDelta(int);        //计算w和b的调整量
    void UpdateNetWork();       //更新权值和阀值
    double GetError(int);         //计算单个样本的误差
    double GetAccu();             //计算所有样本的精度
    double Sigmoid(const double);   //计算Sigmoid的值
    void split(char *buffer, vector<double> &vec);

public:
    CBP_Algorithm();
    ~CBP_Algorithm(){};


    void GetData(const vector<ST_DATA>);
    void Train();
    vector<double> ForeCast(const vector<double>);
    void ForCastFromFile(CBP_Algorithm * &);
    void ReadFile(const char * InutFileName,int m, int n);

    void ReadTestFile(const char * InputFileName, int m, int n);
    void WriteToFile(const char * OutPutFileName);

};

#endif // CBP_ALGORITHM_H
