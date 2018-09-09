#ifndef CBP_ALGORITHM_H
#define CBP_ALGORITHM_H

#include <vector>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

#define INPUTNODE   8       /// 输入节点数
#define HIDENODE    4       /// 隐藏节点数
#define OUTPUTNODE  8       /// 输出节点数
#define TRAINSAMPLE 8       /// 训练样本数

class CBP_Algorithm
{
public:

    double w[INPUTNODE][HIDENODE];     //隐含结点权值
    double w1[HIDENODE][OUTPUTNODE];   //输出结点权值
    double b1[HIDENODE];            //隐含结点阀值
    double b2[OUTPUTNODE];             //输出结点阀值

    double rate_w;      //权值学习率（输入层-隐含层)
    double rate_w1;     //权值学习率 (隐含层-输出层)
    double rate_b1;     //隐含层阀值学习率
    double rate_b2;     //输出层阀值学习率

    double e;           //误差计算
    double error;       //允许的最大误差
    double result[OUTPUTNODE];         // Bp输出


public:
    CBP_Algorithm();

    void init();
    void train(double p[TRAINSAMPLE][INPUTNODE], double t[TRAINSAMPLE][OUTPUTNODE]);//Bp训练
    double p[TRAINSAMPLE][INPUTNODE];     //输入的样本
    double t[TRAINSAMPLE][OUTPUTNODE];    //样本要输出的

    double *recognize(double *p);      //Bp识别


};

#endif // CBP_ALGORITHM_H
