#include "cbp_algorithm.h"


CBP_Algorithm::CBP_Algorithm()
{

}


//产生[low, high)之间的随机数
double randval(double low, double high)
{
    double val;
    val = ((double)(rand() % RAND_MAX)/(double)RAND_MAX) * (high - low) + low;
    return(val);
}

void winit(double w[], int n) //权值初始化
{
    for(int i=0; i<n; i++)
        w[i] = randval(-0.01, 0.01);
}

//初始化
void CBP_Algorithm::init()
{
    winit((double*)w, INPUTNODE * HIDENODE);
    winit((double*)w1, HIDENODE * OUTPUTNODE);
    winit(b1, HIDENODE);
    winit(b2, OUTPUTNODE);
}

//训练
void CBP_Algorithm::train(double p[TRAINSAMPLE][INPUTNODE], double t[TRAINSAMPLE][OUTPUTNODE])
{
    double pp[HIDENODE];    //隐含结点的校正误差
    double qq[OUTPUTNODE];     //希望输出值与实际输出值的偏差
    double yd[OUTPUTNODE];     //希望输出值

    double x[INPUTNODE];       //输入向量
    double x1[HIDENODE];    //隐含结点状态值
    double x2[OUTPUTNODE];     //输出结点状态值
    double o1[HIDENODE];    //隐含层激活值
    double o2[HIDENODE];    //输出层激活值

    for(int isamp=0; isamp<TRAINSAMPLE; isamp++)  //循环训练一次样品
    {
        int i, j, k;
        for(i=0; i<INPUTNODE; i++)
            x[i] = p[isamp][i];     //输入的样本
        for(i=0; i<OUTPUTNODE; i++)
            yd[i] = t[isamp][i];    //希望输出的样本

        //正向传播
        //构造每个样品的输入和输出标准
        for(j=0; j<HIDENODE; j++)
        {
            o1[j] = 0.0;
            for(i=0; i< INPUTNODE; i++)
                o1[j] += w[i][j] * x[i];                //隐含层各单元输入激活值
            x1[j] = 1.0 / (1.0 + exp(-o1[j] - b1[j]));  //隐含层各单元的输出
        }

        for(k=0; k<OUTPUTNODE; k++)
        {
            o2[k] = 0.0;
            for(int j=0; j<HIDENODE; j++)
                o2[k] += w1[j][k] * x1[j];              //输出层各单元输入激活值
            x2[k] = 1.0 / (1.0 + exp(-o2[k] - b2[k]));  //输出层各单元输出
        }

        //误差反向传播
        for(k=0; k<OUTPUTNODE; k++)  //对于网络中每个输出单元，计算误差项，并更新权值
        {
            qq[k] = (yd[k] - x2[k]) * x2[k] * (1-x2[k]);    //希望输出与实际输出的偏差
            for(j=0; j<HIDENODE; j++)
                w1[j][k] += rate_w1 * qq[k] * x1[j];        //更新隐含层和输出层之间的连接权
        }

        for(j=0; j<HIDENODE; j++) //对于网络中每个隐藏单元，计算误差项，并更新权值
        {
            pp[j] = 0.0;
            for(k=0; k<OUTPUTNODE; k++)
                pp[j] += qq[k] * w1[j][k];
            pp[j] = pp[j] * x1[j] * (1 - x1[j]);    //隐含层的校正误差

            for(i=0; i<INPUTNODE; i++)
                w[i][j] += rate_w * pp[j] * x[i];   //更新输入层和隐含层之间的连接权
        }

        for(k=0; k<OUTPUTNODE; k++)
        {
            e += pow(yd[k] - x2[k], 2);  //计算均方差
        }
        error = e/2.0;

        for(k=0; k<OUTPUTNODE; k++)
            b2[k] += rate_b2 * qq[k]; //更新隐含层和输出层之间的阈值
        for(j=0; j<HIDENODE; j++)
            b1[j] += rate_b1 * pp[j]; //更新输入层和隐含层之间的阈值
    }
}

//识别
double *CBP_Algorithm::recognize(double *p)
{
    double x[INPUTNODE];       //输入向量
    double x1[HIDENODE];    //隐含结点状态值
    double x2[OUTPUTNODE];     //输出结点状态值
    double o1[HIDENODE];    //隐含层激活值
    double o2[HIDENODE];    //输出层激活值

    int i, j, k;
    for(i=0; i<INPUTNODE; i++)
        x[i] = p[i];

    for(j=0; j<HIDENODE; j++)
    {
        o1[j] = 0.0;
        for(i=0; i<INPUTNODE; i++)
            o1[j] = o1[j] + w[i][j] * x[i];         //隐含层各单元激活值
        x1[j] = 1.0 / (1.0 + exp(-o1[j] - b1[j]));  //隐含层各单元输出
    }

    for(k=0; k<OUTPUTNODE; k++)
    {
        o2[k] = 0.0;
        for(j=0; j<HIDENODE; j++)
            o2[k] = o2[k] + w1[j][k] * x1[j];       //输出层各单元激活值
        x2[k] = 1.0 / (1.0 + exp(-o2[k] - b2[k]));  //输出层各单元输出
    }

    for(k=0; k<OUTPUTNODE; k++)
    {
        result[k] = x2[k];
    }
    return result;
}


