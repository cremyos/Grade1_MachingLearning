#include <QCoreApplication>
#include <string.h>
#include <stdio.h>
#include "cbp_algorithm.h"

using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    unsigned int Id, Od;    //样本数据的输入维数/输出维数
    int select = 0;
    CBP_Algorithm *bp = new CBP_Algorithm();
    const char * inputDataName = "exercisedata.txt";//训练数据文件名称
    const char * testDataName = "testdata.txt";   //测试数据文件名称
    const char * outputDataName = "result.txt";     //输出文件名称

    printf("please input sample input dimension and output dimension:\n");
    scanf("%d%d", &Id, &Od);
    bp->ReadFile(inputDataName,Id,Od);

    //exercise
    bp->Train();
    //Test
    printf("\n******************************************************\n");
    printf("*1.使用测试文件中国的数据测试 2.从控制台输入数据测试  \n");
    printf("******************************************************\n");
    scanf("%d", &select);
    switch (select)
    {
    case 1:
        bp->ReadTestFile(testDataName,Id,Od);
        bp->ForCastFromFile(bp);
        bp->WriteToFile(outputDataName);
        printf("the result have been save in the file :result.txt.\n");
        break;
    case 2:
        printf("\n\nplease input the Test Data(3 dimension )：\n");
        while (1)
        {
            vector<double> in;
            for (int i = 0; i < Id; i++)
            {
                double v;
                scanf("%lf", &v);
                in.push_back(v);
            }
            vector<double> ou;
            ou = bp->ForeCast(in);
            printf("%lf\n", ou[0]);
        }
        break;
    default:
        printf("Input error!");
        exit(0);
    }


    return a.exec();
}
