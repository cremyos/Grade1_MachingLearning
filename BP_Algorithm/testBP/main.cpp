#include <QCoreApplication>
#include <string.h>
#include <stdio.h>
#include "cbp_algorithm.h"

using namespace std;

//输入样本
double X[TRAINSAMPLE][INPUTNODE] =
{
    {1,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0},
    {0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,1}
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    srand(time(NULL));
    int i, j, k;
    CBP_Algorithm bp;
    bp.init();
    int times = 0;

    while(bp.error > 0.0001 && times < 5000)
    {
        bp.e = 0.0;
        times++;
        bp.train(X, X);
    }
    double m[INPUTNODE] = {0,0,1,0,0,0,0,0};
    bp.recognize(m);
    for(i=0; i<INPUTNODE; ++i)
        cout << m[i];
    cout << " is ";
    for(i=0; i<OUTPUTNODE; i++)
        printf("%d", (int)floor(bp.result[i] + 0.5));
    cout << endl;

    return a.exec();
}
