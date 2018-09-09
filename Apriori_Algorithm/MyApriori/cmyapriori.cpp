#include "cmyapriori.h"

/**
 * 将文本数据存入到Map中，产生事务数据库D，即textDataBase
 **/
int CMyApriori::ReadFile()
{
    FILE *ReadFile;

    int i32LineIndex = 0;

    /*打开文本文件*/
    ReadFile = fopen(dataFileName.toLatin1().data(), "r");
    if(ReadFile == NULL){
        qDebug()<<"OpenFile ERROR";
        return EXIT_FAILURE;
    }


    while(!feof(ReadFile)){

        if(i32LineIndex == 0) {
            char Data[1024] = {0x0};

            fscanf(ReadFile, "%s", Data);


            i32LineIndex++;

            continue;
        } else if(i32LineIndex != 0) {

            /**
             * typedef struct {
             *  QString Date;            ///< 时间
             *  double dPM2_5;          ///< PM2.5
             *  double DEWP;            ///< Dew Point
             *  double TEMP;            ///< Temperature
             *  double PRES;            ///< Pressure
             *  double dIs;             ///< Cumulated hours of snow
             *  double dIr;             ///< Cumulated hours of rain
             * }ST_PM_DATA;
             */
            ST_PM_DATA stPMDataTemp;

            /**
             * No,year,month,day,hour,pm2.5,DEWP,TEMP,PRES,cbwd,Iws,Is,Ir
             * 25,2010,1,2,0,129,-16,-4,1020,SE,1.79,0,0             
             * 数据如上图
             */

            char TempLine[1024] = {0x0};

            fscanf(ReadFile, "%s", TempLine);

            QString TempPrint = QString(TempLine);

            QString StrTempData;
            /**
             * No,year,month,day,hour,pm2.5,DEWP,TEMP,PRES,cbwd,Iws,Is,Ir
             * 0 ,  1 ,  2  , 3 , 4  , 5   , 6  , 7  , 8  , 9  , 10,11,12
             */
            QStringList tempStrList = TempPrint.split(",");
            for(int i32Temp = 0; i32Temp < tempStrList.count(); i32Temp++) {
                ST_PM_DOUDATA stPMData;

                if(i32Temp == 0) {
                    stPMDataTemp.i32Index = tempStrList.at(i32Temp).toInt();
                }

                if((i32Temp > 0) && (i32Temp < 5)) {
                    StrTempData += tempStrList.at(i32Temp);
                    if(i32Temp != 4) {
                        StrTempData += "_";
                    }
                }

                if(i32Temp == 5) {
                    stPMDataTemp.Date = StrTempData;

                    stPMData.Date = stPMDataTemp.Date;

                    if(tempStrList.at(i32Temp) == "NA") {
                        stPMDataTemp.dPM2_5 = 0;

                        stPMData.dPM2_5 = stPMDataTemp.dPM2_5;
                    } else {
                        stPMDataTemp.dPM2_5 = tempStrList.at(i32Temp).toDouble();

                        stPMData.dPM2_5 = stPMDataTemp.dPM2_5;
                    }

//                    qDebug()<<"stPMDataTemp.Date = " + StrTempData + " || stPMDataTemp.dPM2_5 = " + QString::number(stPMDataTemp.dPM2_5);
                }

                if(i32Temp == 6) {
                    stPMDataTemp.DEWP = tempStrList.at(i32Temp).toDouble();
                    stPMData.dData = stPMDataTemp.DEWP;
                    vecDouData_DEWP.push_back(stPMData);

                    MapDouData.insert(0, vecDouData_DEWP);

//                    qDebug()<<"stPMDataTemp.DEWP = " + QString::number(stPMDataTemp.DEWP);
                }

                if(i32Temp == 7) {
                    stPMDataTemp.TEMP = tempStrList.at(i32Temp).toDouble();

                    stPMData.dData = stPMDataTemp.TEMP;
                    vecDouData_TEMP.push_back(stPMData);

                    MapDouData.insert(1, vecDouData_TEMP);

//                    qDebug()<<"stPMDataTemp.TEMP = " + QString::number(stPMDataTemp.TEMP);
                }

                if(i32Temp == 8) {
                     stPMDataTemp.PRES = tempStrList.at(i32Temp).toDouble();

                     stPMData.dData = stPMDataTemp.PRES;
                     vecDouData_PRES.push_back(stPMData);

                     MapDouData.insert(2, vecDouData_PRES);

//                     qDebug()<<"stPMDataTemp.PRES = " + QString::number(stPMDataTemp.PRES);
                }

                if(i32Temp == 11) {
                    stPMDataTemp.dIs = tempStrList.at(i32Temp).toDouble();

                    stPMData.dData = stPMDataTemp.dIs;
                    vecDouData_dIs.push_back(stPMData);

                    MapDouData.insert(3, vecDouData_dIs);
//                    qDebug()<<"stPMDataTemp.dIs = " + QString::number(stPMDataTemp.dIs);
                }

                if(i32Temp == 12) {
                    stPMDataTemp.dIr = tempStrList.at(i32Temp).toDouble();

                    stPMData.dData = stPMDataTemp.dIr;
                    vecDouData_dIr.push_back(stPMData);

                    MapDouData.insert(4, vecDouData_dIr);
//                    qDebug()<<"stPMDataTemp.dIr = " + QString::number(stPMDataTemp.dIr);
                }
            }

            vecSTPM.push_back(stPMDataTemp);

        }

        if(i32LineIndex > MAXLINE) {
            break;
        }

        i32LineIndex++;

    }

#if 0

    /*产生事务数据库*/
    int line_num ;
    for(line_num = 0; line_num != lines_of_text.size(); ++line_num){
        istringstream line(lines_of_text[line_num]);
        string word;
//        cout<<"word is "<<endl;
        while(line >> word){
            textDatabase[line_num].insert(word);
//            cout<<word<<endl;
        }
    }
#endif
//    textDatabaseCount = textDatabase.size();
//    cout <<"textDatabaseCount: " << textDatabaseCount <<" " << line_num<< endl;

    delete ReadFile;

    return EXIT_SUCCESS;
}

/**
 * 创建事务集
 **/
int CMyApriori::buildDataBase()
{
    /**
     * PM2.5与DEWP || PM2.5 与 TEMP || PM2.5 与 PRES || PM2.5 与 dIr || PM2.5 与 dIs
     * 对应1               1                1                  1               1
     * 例如PM2.5与DEWP 字符串表示为10000， 同理 PM2.5 与 TEMP 表示为 01000
     **/
    QString MatterCode[5] = {"10000", "01000", "00100", "00010", "00001"};



    for(int i32TempIndex = 0; i32TempIndex < 5; i32TempIndex++) {
        ST_MATTER_COUNT stMatterCount;
        stMatterCount.MatterName = MatterCode[i32TempIndex];
        stMatterCount.i32Count = 0;
        i32ItemStartPoint = 0;
        TextDataBase.insert(i32TempIndex, stMatterCount);
        i32ItemPoint = i32TempIndex;
    }

#if 0
    qDebug()<<"TextDataBase is ";
    QMap<int, QString>::iterator TextDataBaseIter = TextDataBase.begin();
    for(; TextDataBaseIter != TextDataBase.end(); TextDataBaseIter++) {
        qDebug()<<QString::number(TextDataBaseIter.key()) + " || " + TextDataBaseIter.value();
    }
#endif

}

/**
 * 数据处理功能
 */
int CMyApriori::DealData()
{
    /**
     * 对应位置的数据集，用于记录升高的数量和降低的数量
     * PM2.5升高时的数据
     **/
    int PM2_5AriseArrayCountIncrease[5] = {0};
    int PM2_5AriseArrayCountDecrease[5] = {0};

    /**
     * PM2.5降低的时候的数据
     **/
    int PM2_5DecArrayCountIncrease[5] = {0};
    int PM2_5DecArrayCountDecrease[5] = {0};

    for(int i32TempIndex = 1; i32TempIndex < vecSTPM.size(); i32TempIndex++) {
        if((vecSTPM.at(i32TempIndex).dPM2_5 - vecSTPM.at(i32TempIndex - 1).dPM2_5) > 0) {
            /**
             * PM2.5升
             **/
            if((vecSTPM.at(i32TempIndex).DEWP - vecSTPM.at(i32TempIndex - 1).DEWP) > 0) {
//                qDebug()<<"DEWP = " + QString::number(vecSTPM.at(i32TempIndex).DEWP - vecSTPM.at(i32TempIndex - 1).DEWP);
                /**
                 * DEWP升
                 **/
                PM2_5AriseArrayCountIncrease[0] ++;
            } else if((vecSTPM.at(i32TempIndex).DEWP - vecSTPM.at(i32TempIndex - 1).DEWP) <= 0){
//                qDebug()<<"DEWP = " + QString::number(vecSTPM.at(i32TempIndex).DEWP - vecSTPM.at(i32TempIndex - 1).DEWP);
                /**
                 * DEWP降
                 **/
                PM2_5AriseArrayCountDecrease[0] ++;
            }

            if((vecSTPM.at(i32TempIndex).TEMP - vecSTPM.at(i32TempIndex - 1).TEMP) > 0) {
//                qDebug()<<"TEMP = " + QString::number(vecSTPM.at(i32TempIndex).TEMP - vecSTPM.at(i32TempIndex - 1).TEMP);
                /**
                 * TEMP 升
                 **/
                PM2_5AriseArrayCountIncrease[1] ++;
            } else if((vecSTPM.at(i32TempIndex).TEMP - vecSTPM.at(i32TempIndex - 1).TEMP) <= 0){
//                qDebug()<<"TEMP = " + QString::number(vecSTPM.at(i32TempIndex).TEMP - vecSTPM.at(i32TempIndex - 1).TEMP);
                /**
                 * TEMP 降
                 **/
                PM2_5AriseArrayCountDecrease[1] ++;
            }

            if((vecSTPM.at(i32TempIndex).PRES - vecSTPM.at(i32TempIndex - 1).PRES) > 0) {
//                qDebug()<<"PRES = " + QString::number(vecSTPM.at(i32TempIndex).PRES - vecSTPM.at(i32TempIndex - 1).PRES);
                /**
                 * PRES 升
                 **/
                PM2_5AriseArrayCountIncrease[2] ++;
            } else if((vecSTPM.at(i32TempIndex).PRES - vecSTPM.at(i32TempIndex - 1).PRES) <= 0){
//                qDebug()<<"PRES = " + QString::number(vecSTPM.at(i32TempIndex).PRES - vecSTPM.at(i32TempIndex - 1).PRES);
                /**
                 * PRES 降
                 **/
                PM2_5AriseArrayCountDecrease[2] ++;
            }

            if((vecSTPM.at(i32TempIndex).dIr - vecSTPM.at(i32TempIndex - 1).dIr) > 0) {
//                qDebug()<<"dIr = " + QString::number(vecSTPM.at(i32TempIndex).dIr - vecSTPM.at(i32TempIndex - 1).dIr);
                /**
                 * dIr 升
                 **/
                PM2_5AriseArrayCountIncrease[3] ++;
            } else if((vecSTPM.at(i32TempIndex).dIr - vecSTPM.at(i32TempIndex - 1).dIr) <= 0){
//                qDebug()<<"dIr = " + QString::number(vecSTPM.at(i32TempIndex).dIr - vecSTPM.at(i32TempIndex - 1).dIr);
                /**
                 * dIr 降
                 **/
                PM2_5AriseArrayCountDecrease[3] ++;
            }

            if((vecSTPM.at(i32TempIndex).dIs - vecSTPM.at(i32TempIndex - 1).dIs) > 0) {
//                qDebug()<<"dIs = " + QString::number(vecSTPM.at(i32TempIndex).dIs - vecSTPM.at(i32TempIndex - 1).dIs);
                /**
                 * dIs 升
                 **/
                PM2_5AriseArrayCountIncrease[4] ++;
            } else if((vecSTPM.at(i32TempIndex).dIs - vecSTPM.at(i32TempIndex - 1).dIs) <= 0){
//                qDebug()<<"dIs = " + QString::number(vecSTPM.at(i32TempIndex).dIs - vecSTPM.at(i32TempIndex - 1).dIs);
                /**
                 * dIs 降
                 **/
                PM2_5AriseArrayCountDecrease[4] ++;
            }

        } else if((vecSTPM.at(i32TempIndex).dPM2_5 - vecSTPM.at(i32TempIndex - 1).dPM2_5) > 0) {
            /**
             * PM2.5降低
             **/
            if((vecSTPM.at(i32TempIndex).DEWP - vecSTPM.at(i32TempIndex - 1).DEWP) > 0) {
//                qDebug()<<"2 DEWP = " + QString::number(vecSTPM.at(i32TempIndex).DEWP - vecSTPM.at(i32TempIndex - 1).DEWP);
                /**
                 * DEWP升
                 **/
                PM2_5DecArrayCountIncrease[0] ++;
            } else if((vecSTPM.at(i32TempIndex).DEWP - vecSTPM.at(i32TempIndex - 1).DEWP) <= 0){
//                qDebug()<<"2 DEWP = " + QString::number(vecSTPM.at(i32TempIndex).DEWP - vecSTPM.at(i32TempIndex - 1).DEWP);
                /**
                 * DEWP降
                 **/
                PM2_5DecArrayCountDecrease[0] ++;
            }

            if((vecSTPM.at(i32TempIndex).TEMP - vecSTPM.at(i32TempIndex - 1).TEMP) > 0) {
//                qDebug()<<"2 TEMP = " + QString::number(vecSTPM.at(i32TempIndex).TEMP - vecSTPM.at(i32TempIndex - 1).TEMP);
                /**
                 * TEMP 升
                 **/
                PM2_5DecArrayCountIncrease[1] ++;
            } else if((vecSTPM.at(i32TempIndex).TEMP - vecSTPM.at(i32TempIndex - 1).TEMP) <= 0){
//                qDebug()<<"2 TEMP = " + QString::number(vecSTPM.at(i32TempIndex).TEMP - vecSTPM.at(i32TempIndex - 1).TEMP);
                /**
                 * TEMP 降
                 **/
                PM2_5DecArrayCountDecrease[1] ++;
            }

            if((vecSTPM.at(i32TempIndex).PRES - vecSTPM.at(i32TempIndex - 1).PRES) > 0) {
//                qDebug()<<"2 PRES = " + QString::number(vecSTPM.at(i32TempIndex).PRES - vecSTPM.at(i32TempIndex - 1).PRES);
                /**
                 * PRES 升
                 **/
                PM2_5DecArrayCountIncrease[2] ++;
            } else if((vecSTPM.at(i32TempIndex).PRES - vecSTPM.at(i32TempIndex - 1).PRES) <= 0){
//                qDebug()<<"2 PRES = " + QString::number(vecSTPM.at(i32TempIndex).PRES - vecSTPM.at(i32TempIndex - 1).PRES);
                /**
                 * PRES 降
                 **/
                PM2_5DecArrayCountDecrease[2] ++;
            }

            if((vecSTPM.at(i32TempIndex).dIr - vecSTPM.at(i32TempIndex - 1).dIr) > 0) {
//                qDebug()<<"2 dIr = " + QString::number(vecSTPM.at(i32TempIndex).dIr - vecSTPM.at(i32TempIndex - 1).dIr);
                /**
                 * dIr 升
                 **/
                PM2_5DecArrayCountIncrease[3] ++;
            } else if((vecSTPM.at(i32TempIndex).dIr - vecSTPM.at(i32TempIndex - 1).dIr) <= 0){
//                qDebug()<<"2 dIr = " + QString::number(vecSTPM.at(i32TempIndex).dIr - vecSTPM.at(i32TempIndex - 1).dIr);
                /**
                 * dIr 降
                 **/
                PM2_5DecArrayCountDecrease[3] ++;
            }

            if((vecSTPM.at(i32TempIndex).dIs - vecSTPM.at(i32TempIndex - 1).dIs) > 0) {
//                qDebug()<<"2 dIs = " + QString::number(vecSTPM.at(i32TempIndex).dIs - vecSTPM.at(i32TempIndex - 1).dIs);
                /**
                 * dIs 升
                 **/
                PM2_5DecArrayCountIncrease[4] ++;
            } else if((vecSTPM.at(i32TempIndex).dIs - vecSTPM.at(i32TempIndex - 1).dIs) <= 0){
//                qDebug()<<"2 dIs = " + QString::number(vecSTPM.at(i32TempIndex).dIs - vecSTPM.at(i32TempIndex - 1).dIs);
                /**
                 * dIs 降
                 **/
                PM2_5DecArrayCountDecrease[4] ++;
            }

        }
    }


    /**
     * 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
     * 0, 2, 4, 6, 8 表示PM2.5增加还是减少，1表示增加，2表示减少
     * 1, 3, 5, 7, 9 表示其他数据增加还是减少，1表示增加，2表示减少
     **/
//    unsigned char DataCode[10] = {0};

    qDebug()<<"result:";
    for(int i32TempIndex = 0; i32TempIndex < 5; i32TempIndex++) {

        double TempDouDataAriseIncrease = PM2_5AriseArrayCountIncrease[i32TempIndex];
        double TempDouDataAriseDecrease = PM2_5AriseArrayCountDecrease[i32TempIndex];

        /**
         * @brief TempDouData
         * 频繁集1的阈值
         */
        double TempDouData = MAXLINE * PERCENT1;

        if(TempDouDataAriseIncrease > TempDouData) {
            /**
             * 判断第i32TempIndex组数据，如果增加则置位1，否则置位2
             **/
            DataCode[i32TempIndex * 2] = 1 + '0';
            DataCode[i32TempIndex * 2 + 1] = 1 + '0';
        } else if(TempDouDataAriseDecrease > TempDouData) {
            DataCode[i32TempIndex * 2] = 1 + '0';
            DataCode[i32TempIndex * 2 + 1] = 2 + '0';
        }

        double TempDouDataDecIncrease = PM2_5DecArrayCountIncrease[i32TempIndex];
        double TempDouDataDecDecrease = PM2_5DecArrayCountDecrease[i32TempIndex];

        if(TempDouDataDecIncrease > TempDouData) {
            /**
             * 判断第i32TempIndex组数据，如果增加则置位1，否则置位2
             **/
            DataCode[i32TempIndex * 2] = 2 + '0';
            DataCode[i32TempIndex * 2 + 1] = 1 + '0';
        } else if(TempDouDataDecDecrease > TempDouData) {
            DataCode[i32TempIndex * 2] = 2 + '0';
            DataCode[i32TempIndex * 2 + 1] = 2 + '0';
        }

    }

    cout<<DataCode<<endl;
}

int CMyApriori::GetItem1()
{
    int i32ItemIndex = 0;
    for(int i32TempIndex = 0; i32TempIndex < 10; )  {
        /**
         * 0 PM2.5增加 对应项目的值降低、1 PM2.5增加 对应项目的值增加
         * 2 PM2.5降低 对应项目的值降低、3 PM2.5降低 对应项目的值增加
         **/
        if(DataCode[i32TempIndex] == 1 + '0') {
            if(DataCode[i32TempIndex + 1] == 1 + '0') {
                ItemCode[i32ItemIndex ++] = 1 + '0';
            } else if(DataCode[i32TempIndex + 1] == 2 + '0') {
                ItemCode[i32ItemIndex ++] = 0 + '0';
            }
        } else if(DataCode[i32TempIndex] == 2 + '0') {
            if(DataCode[i32TempIndex + 1] == 1 + '0') {
                ItemCode[i32ItemIndex ++] = 3 + '0';
            } else if(DataCode[i32TempIndex + 1] == 2 + '0') {
                ItemCode[i32ItemIndex ++] = 2 + '0';
            }
        }

        i32TempIndex += 2;
    }

    QString StrTempItem = QString(ItemCode);
    qDebug()<<"StrTempItem is " + StrTempItem;
}

int CMyApriori::DealData2()
{
    int SignalArray[4][5] = {0x0};
    int SignalCountArray[5] = {0};
    for(int i32Temp = 0; i32Temp < 5; i32Temp++) {
        if(ItemCode[i32Temp] == 0 + '0') {
            /**
             * 找到数据中为 0 的项的下标
             **/
            SignalArray[0][i32Temp] = 1;
            SignalCountArray[0]++;
        } else if(ItemCode[i32Temp] == 1 + '0') {
            /**
             * 找到数据中为 1 的项的下标
             **/
            SignalArray[1][i32Temp] = 1;
            SignalCountArray[1]++;
        } else if(ItemCode[i32Temp] == 2 + '0') {
            /**
             * 找到数据中为 2 的项的下标
             **/
            SignalArray[2][i32Temp] = 1;
            SignalCountArray[2]++;
        } else if(ItemCode[i32Temp] == 3 + '0') {
            /**
             * 找到数据中为 3 的项的下标
             **/
            SignalArray[3][i32Temp] = 1;
            SignalCountArray[3]++;
        }
    }


    QString StrItemCode;

    char ItemData[6] = {0x0};
    memset(ItemData, '\0', sizeof(char) * 6);

    i32ItemStartPoint = i32ItemPoint;

    /**
     * 通过上面对于数据的处理，使得项目集为1的表示存在关系，
     * 下面判断项目集为1的关系中，数据量的大小，是否超过阈值。
     **/
    for(int i32TempIndex1 = 0; i32TempIndex1 < 4; i32TempIndex1++) {
        for(int i32TempIndex2 = 0; i32TempIndex2 < 5; i32TempIndex2++) {
            if(SignalCountArray[i32TempIndex1] >= 2) {
                if((SignalArray[i32TempIndex1][i32TempIndex2] == 1)) {
                    ItemData[i32TempIndex2] = 1 + '0';
                } else {
                    ItemData[i32TempIndex2] = 0 + '0';
                }
            }
        }
//        printf("i32TempIndex1 = %d || %s\n", i32TempIndex1, ItemData);
//        qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1) + " || " + QString(ItemData);
        QString TempData = QString(ItemData);
        if(TempData.contains("1")) {
            ST_MATTER_COUNT stMatterCount;
            stMatterCount.MatterName = TempData;
            stMatterCount.i32Count = 0;
            stMatterCount.i32Increase = 0;
            TextDataBase.insert(i32ItemPoint++, stMatterCount);
        }

        memset(ItemData, 0x0, sizeof(char) * 5);
    }

    qDebug()<<"TextDataBase :";
    QMap<int, ST_MATTER_COUNT>::iterator TextDataBaseIter = TextDataBase.begin();
    for(; TextDataBaseIter != TextDataBase.end(); TextDataBaseIter++) {
        ST_MATTER_COUNT TempMatter = TextDataBaseIter.value();
        qDebug()<<TempMatter.MatterName;
    }
}

int CMyApriori::DealItem2()
{
    char Matter[6] = {0x0};
    int i32TempArray[10];
    QMap<int, QVector<ST_PM_DOUDATA>>::iterator MapDouDataIter;
    QPair<QMap<int, QVector<ST_PM_DOUDATA>>::iterator, bool> PairBoolMap;
    QVector<QPair<QMap<int, QVector<ST_PM_DOUDATA>>::iterator, bool>> VecPair;

//    qDebug()<<"Find Data :";
    QMap<int, int> MapSignalCount;
    QMap<int, ST_MATTER_COUNT>::iterator TextDataBaseIter = TextDataBase.find(i32ItemStartPoint);
    for(; TextDataBaseIter != TextDataBase.end(); TextDataBaseIter++) {
        ST_MATTER_COUNT TempMatter = TextDataBaseIter.value();
        memcpy(Matter, TempMatter.MatterName.toLatin1().data(), sizeof(char) * 6);

        for(int i32TempIndex = 0; i32TempIndex < 6; i32TempIndex++) {
            if(Matter[i32TempIndex] == 1 + '0') {
//                qDebug()<<"Matter[i32TempIndex] == 1 || I32TempIndex = " + QString::number(i32TempIndex);
                MapDouDataIter = MapDouData.find(i32TempIndex);
//                qDebug()<<"I am here MapDouDataIter is " + QString::number(MapDouDataIter.key());
                PairBoolMap = qMakePair(MapDouDataIter, true);
                VecPair.push_back(PairBoolMap);
            }
        }

        /**
         * 对应项存在关系
         **/
//        qDebug()<<"DealItem2 Show:";
        for(int i32TempIndex = 0; i32TempIndex < VecPair.size(); i32TempIndex++){
            int i32ChangeSignal = 0;                /// 对应项的增加或者减少的数量
            if(PairBoolMap.second == true) {
                QMap<int, QVector<ST_PM_DOUDATA>>::iterator TempIter;
                TempIter = VecPair.at(i32TempIndex).first;
                QVector<ST_PM_DOUDATA> vecTemp = TempIter.value();
//                qDebug()<<QString::number(TempIter.key());
                /**
                 * 遍历对应数据项的数据
                 **/
                for(int i32TempIndex2 = 1; i32TempIndex2 < vecTemp.size(); i32TempIndex2++) {
                    if((vecTemp.at(i32TempIndex2) > vecTemp.at(i32TempIndex2 - 1)) || (vecTemp.at(i32TempIndex2) < vecTemp.at(i32TempIndex2 - 1)))  {
                        /**
                         * 存在增减变化
                         **/
                        i32ChangeSignal++;
                    }
                }
            }
        }

        VecPair.clear();
    }





}



CMyApriori::~CMyApriori()
{

}



