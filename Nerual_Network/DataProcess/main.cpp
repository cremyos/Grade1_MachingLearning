#include <QCoreApplication>
#include "cprocessdata.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CprocessData pd;
    /*获取文本文件中原始数据*/
    qDebug()<<"ReadFile";
    pd.ReadFile();
    pd.LoadDataLabels();
//    pd.loadTrainingData();
//    pd.WriteFile();

    return a.exec();
}
