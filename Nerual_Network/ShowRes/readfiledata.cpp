#include "readfiledata.h"

ReadFileData::ReadFileData()
{

}

int ReadFileData::ReadData()
{
    FILE *ReadFileIdle;

    QString dataFileName = "OutPutTest.txt";
    ReadFileIdle = fopen(dataFileName.toLatin1().data(), "r");
    if(ReadFileIdle == NULL){
        qDebug()<<"OpenFile ERROR";
        return EXIT_FAILURE;
    }

    int i32Count = 0;

    int i32TempClass = 0;
    int TempClass = 0;
    double dData = 0.0;
    int i32TempLabels = 0;

    while(1) {

        char TempLine[1024] = {0x0};

#if 0
        fscanf(ReadFileIdle, "%s", TempLine);

        QString TempPrint = QString(TempLine);

        QString StrTempData;

        /**
         * "id","latitude","longitude","track_id","time"
         * 1, -10.9393413858164, -37.0627421097422, 1, "2014-09-13 07:24:32"
         */
        QStringList tempStrList = TempPrint.split("||");
        for(int i32Temp = 0; i32Temp < tempStrList.count(); i32Temp++) {
            if(i32Temp == 0) {
                QString tempStr = tempStrList.at(i32Temp);
                qDebug()<<"Int = " + QString::number(tempStr.toInt());
            }

            if(i32Temp == 1) {
                QString tempStr = tempStrList.at(i32Temp);
                qDebug()<<"double = " + QString::number(tempStr.toDouble());
            }

            if(i32Temp == 2) {
                QString tempStr = tempStrList.at(i32Temp);
                qDebug()<<"Int = " + QString::number(tempStr.toInt());
            }

        }
        qDebug()<<"+++++++++++++++++++++++++++++++";
#endif

        fscanf(ReadFileIdle, "%s", TempLine);

        if(i32Count%3 == 0) {

            QString TempPrint = QString(TempLine);
            TempClass = TempPrint.toInt();
//            qDebug()<<"i32Count%3 == 0 : " + QString::number(TempClass);
#if 1
            /**
             * 将数据赋值给对应的类
             **/
            if(i32TempClass != TempClass) {
                qDebug()<<"Line74: i32TempClass = " + QString::number(TempClass);
                MapVecSTCDS.insert(i32TempClass, VecSTCDS);

                VecSTCDS.clear();
                VecSTCDS.push_back(stCDS);

                i32TempClass = TempClass;
            } else if(i32TempClass != 0){
                i32TempClass = TempClass;

                VecSTCDS.push_back(stCDS);
            } else if(i32TempClass == 0) {
                i32TempClass = TempClass;
            }
#endif
        } else if(i32Count%2 == 0) {
            QString TempPrint = QString(TempLine);
            stCDS.dData = TempPrint.toDouble();
//            qDebug()<<"i32Count%2 == 0 : " + QString::number(stCDS.dData);
        } else if(i32Count%1 == 0) {
            QString TempPrint = QString(TempLine);
            stCDS.i32ShowType = TempPrint.toInt();
//            qDebug()<<"i32Count%1 == 0 : " + QString::number(stCDS.i32ShowType);
        }

        if(i32Count > MAXLINE) {
            break;
        }

        i32Count++;
    }

    fclose(ReadFileIdle);

#if 0
    qDebug()<<"Print:";
    QMap<int, QVector<ST_CLASS_DATA_SHOWTYPE>>::iterator MapVecSTCDSIter = MapVecSTCDS.begin();
    for(MapVecSTCDSIter; MapVecSTCDSIter != MapVecSTCDS.end(); MapVecSTCDSIter++) {
        qDebug()<<"class = " + MapVecSTCDSIter.key();
        QVector<ST_CLASS_DATA_SHOWTYPE> TempVecSCDS = MapVecSTCDSIter.value();
        for(int i32TempIndex = 0; i32TempIndex < TempVecSCDS.size(); i32TempIndex++) {
            ST_CLASS_DATA_SHOWTYPE stCDS = TempVecSCDS.at(i32TempIndex);
            qDebug()<<"Data: " + QString::number(stCDS.dData) + " || " + QString::number(stCDS.i32ShowType);
        }
    }
#endif
}
