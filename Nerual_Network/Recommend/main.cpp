#include <QCoreApplication>
#include "caprioridata.h"
#include "crbfnetwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CAprioriData capriori;
    /*获取文本文件中原始数据*/
    qDebug()<<"ReadFile";
    capriori.ReadFile();
    capriori.loadTrainingData();
    capriori.WriteFile();

    qDebug()<<"Finish LoadTrainingData";

#if 0
    RBFNetwork RBFNN(capriori.training_data, capriori.training_labels, 5);
    double mse=0;
//    RBFNN.startTraining(5, 0.005, 100, mse, true);
//    RBFNN.startTraining(5, 0.005, 1000, mse, true);
    RBFNN.startTraining(4, 0.005, 500, mse, true);

    qDebug()<<"Finish Training";
#endif

#if 0
    for(int i32TempIndex = 0; i32TempIndex < capriori.training_data.size(); i32TempIndex++) {
        double err=0;
        datapoint TempDataPoint = capriori.training_data.at(i32TempIndex);
        int predict = RBFNN.predictLabel(TempDataPoint, err);

        qDebug()<<"predict = " + QString::number(predict);

        /**
         * 输出结果和对应的显示类型：
         * 1:LCD数字显示
         * 2：普通数字显示
         * 3：LED灯
         * 4：使能开关
         * 5：曲线图
         **/
        switch(predict){
            case 1:
                qDebug()<<"LCD数字显示";
            break;

            case 2:
                qDebug()<<"普通数字显示";
            break;

            case 3:
                qDebug()<<"LED灯";
            break;

            case 4:
                qDebug()<<"使能开关";
            break;

            case 5:
                qDebug()<<"曲线图";
            break;
        }

    }
#endif




    return a.exec();
}
