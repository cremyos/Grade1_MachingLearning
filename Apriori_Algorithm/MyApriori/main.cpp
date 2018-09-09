#include <QCoreApplication>
#include <iostream>
#include "cmyapriori.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int min_sup = 2;

    QString TempdataFileName = "BeiJingPM2_5.csv";

    CMyApriori apriori(TempdataFileName);
    /*获取文本文件中原始数据*/
    apriori.ReadFile();

    apriori.buildDataBase();

    apriori.DealData();

    apriori.GetItem1();

    apriori.DealData2();

    apriori.DealItem2();

    for(int i32loopIndex = 1; i32loopIndex; i32loopIndex++) {
        break;
    }

    return a.exec();
}
