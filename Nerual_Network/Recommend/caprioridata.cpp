#include "caprioridata.h"


CAprioriData::CAprioriData()
{

}

/**
 * 将文本数据存入到Map中，产生事务数据库D，即textDataBase
 **/
int CAprioriData::ReadFile()
{
    FILE *ReadFile;
    FILE *ReadFile2;
    QString dataFileName1 = "go_track_trackspoints.csv";
    QString dataFileName2 = "go_track_tracks.csv";


    int i32LineIndex = 0;

    /*打开文本文件*/
    ReadFile = fopen(dataFileName1.toLatin1().data(), "r");
    if(ReadFile == NULL){
        qDebug()<<"OpenFile ERROR";
        return EXIT_FAILURE;
    }


    ReadFile2 = fopen(dataFileName2.toLatin1().data(), "r");
    if(ReadFile2 == NULL){
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

            /*
             *  typedef struct {
                    double i32Id;         ///< id
                    double dLatitude;   ///< latitude
                    double dLongtitude; ///< longtitude
                    double i32Track_Id;   ///< track_id;
                    QString StrTime;    ///< time
                }ST_GPS_POINT;
              */
            ST_GPS_POINT stGPSPoint;


            char TempLine[1024] = {0x0};

            fscanf(ReadFile, "%s", TempLine);

            QString TempPrint = QString(TempLine);

            QString StrTempData;

            /**
             * "id","latitude","longitude","track_id","time"
             * 1, -10.9393413858164, -37.0627421097422, 1, "2014-09-13 07:24:32"
             */
            QStringList tempStrList = TempPrint.split(",");
            for(int i32Temp = 0; i32Temp < tempStrList.count(); i32Temp++) {
//                qDebug()<<tempStrList.at(i32Temp);

                if(i32Temp == 0) {
                    stGPSPoint.dId = tempStrList.at(i32Temp).toInt();
                }


                if(i32Temp == 1) {
                    stGPSPoint.dLatitude = tempStrList.at(i32Temp).toDouble();
//                    qDebug()<<QString::number(86) + ": stGPSPoint.dLatitude =  " + QString::number(stGPSPoint.dLatitude);
                }

                if(i32Temp == 2) {
                    stGPSPoint.dLongtitude = tempStrList.at(i32Temp).toDouble();
//                    qDebug()<<QString::number(91) + ": stGPSPoint.dLongtitude =  " + QString::number(stGPSPoint.dLongtitude);
                }

                if(i32Temp == 3) {
                    stGPSPoint.i32Track_Id = tempStrList.at(i32Temp).toInt();
//                    qDebug()<<QString::number(96) + ": stGPSPoint.i32Track_Id =  " + QString::number(stGPSPoint.i32Track_Id);
                }

                if(i32Temp == 4) {
                    stGPSPoint.StrTime = tempStrList.at(i32Temp);
                }

            }

            VecStGPSPoint.append(stGPSPoint);

            QMap<int, QVector<ST_GPS_POINT>>::iterator MapVecGPSPointIter = MapVecGPSPoint.find(stGPSPoint.i32Track_Id);

            if(MapVecGPSPointIter == MapVecGPSPoint.end()) {
//                qDebug()<<"Canot Find it";
                MapVecGPSPoint.insert(stGPSPoint.i32Track_Id, VecStGPSPoint);
            } else {
//                qDebug()<<"Find it ===============  stGPSPoint.i32Track_Id =  " + QString::number(stGPSPoint.i32Track_Id) + " || MapVecGPSPointIter.key = " + QString(MapVecGPSPointIter.key());
                MapVecGPSPoint.erase(MapVecGPSPointIter);
                MapVecGPSPoint.insert(stGPSPoint.i32Track_Id, VecStGPSPoint);
            }

//            vecSTPM.push_back(stPMDataTemp);

        }

        if(i32LineIndex > MAXLINE) {
            break;
        }

        i32LineIndex++;

    }

    i32LineIndex = 0;

    while(!feof(ReadFile2)){
        if(i32LineIndex == 0) {
            char Data[1024] = {0x0};

            fscanf(ReadFile2, "%s", Data);


            i32LineIndex++;

            continue;
        } else if(i32LineIndex != 0) {

            /**
             * typedef struct {
                    int i32Index;           ///< 序号
                    double dId;            ///< id
                    int i32Id_Android;     ///< id_android
                    double dSpeed;            ///< speed
                    double dTime;            ///< time
                    double dDistance;            ///< distance
                    int i32Rating;             ///< rating
                    int i32Rating_bus;             ///< rating_bus
                    int i32Rating_weather;             ///< rating_weather
                    int dCarORBus;                  /// car or bus
                    QString StrLinha;           /// linha
                }ST_GPS_DATA;
              **/
            ST_GPS_DATA stGPSData;


            char TempLine[1024] = {0x0};

            fscanf(ReadFile2, "%s", TempLine);

            QString TempPrint = QString(TempLine);

            QString StrTempData;

            /**
             * "id","id_android","speed","time","distance","rating","rating_bus","rating_weather","car_or_bus","linha"
             * 1,0,19.2105856218409,0.138048888888889,2.652,3,0,0,1,""
             * 类型：
             */
            QStringList tempStrList = TempPrint.split(",");
            for(int i32Temp = 0; i32Temp < tempStrList.count(); i32Temp++) {
                if(i32Temp == 0) {
                    stGPSData.i32Id = tempStrList.at(i32Temp).toDouble();
                }

                if(i32Temp == 1) {
                    stGPSData.i32Id_Android = tempStrList.at(i32Temp).toInt();
                }

                if(i32Temp == 2) {
                    stGPSData.dSpeed = tempStrList.at(i32Temp).toDouble();
//                    qDebug()<<"line 187 stGPSData.dSpeed = " + QString::number(stGPSData.dSpeed);
                }

                if(i32Temp == 3) {
                    stGPSData.dTime = tempStrList.at(i32Temp).toDouble();
//                    qDebug()<<"line 192 stGPSData.dTime = " + QString::number(stGPSData.dTime);
                }

                if(i32Temp == 4) {
                    stGPSData.dDistance = tempStrList.at(i32Temp).toDouble();
                }

                if(i32Temp == 5) {
                    stGPSData.i32Rating = tempStrList.at(i32Temp).toInt();
                }

                if(i32Temp == 6) {
                    stGPSData.i32Rating_bus = tempStrList.at(i32Temp).toInt();
                }

                if(i32Temp == 7) {
                    stGPSData.i32Rating_weather = tempStrList.at(i32Temp).toInt();
                }

                if(i32Temp == 8) {
                    stGPSData.dCarORBus = tempStrList.at(i32Temp).toDouble();
                }

                if(i32Temp == 9) {
                    stGPSData.StrLinha = tempStrList.at(i32Temp);
                }

//                VecStDevData.append(stGPSData);

            }
            QMap<int, QVector<ST_GPS_POINT>>::iterator MapVecGPSPointIter = MapVecGPSPoint.find(stGPSData.i32Id);

            if(MapVecGPSPointIter == MapVecGPSPoint.end()) {

            } else {
                /**
                 * 找到对应的设备
                 **/
                ST_DEVICE_DATA stDevData;
                stDevData.VecStGPSPoint = MapVecGPSPointIter.value();
                stDevData.stGPSData = stGPSData;
                MapIntDeviceData.insert(stGPSData.i32Id, stDevData);
            }

//            qDebug()<<"Show Other Data";

            if(i32LineIndex > MAXLINE) {
                break;
            }

            i32LineIndex++;
        }
    }
#if 0
    qDebug()<<"ShowReadFile";
    QMap<int, ST_DEVICE_DATA>::iterator MapIntDeviceDataIter = MapIntDeviceData.begin();
    for(MapIntDeviceDataIter; MapIntDeviceDataIter != MapIntDeviceData.end(); MapIntDeviceDataIter++) {
        qDebug()<<"Key is " + QString::number(MapIntDeviceDataIter.key());
        ST_DEVICE_DATA stDevDataTemp = MapIntDeviceDataIter.value();
        QVector<ST_GPS_POINT> VecStGPSPointTemp = stDevDataTemp.VecStGPSPoint;

        for(int i32TempIndex3 = 0; i32TempIndex3 < VecStGPSPointTemp.size(); i32TempIndex3++) {
            qDebug()<<"dLatitude = " + QString::number(VecStGPSPointTemp.at(i32TempIndex3).dLatitude) + " || dLongtitude = " + QString::number(VecStGPSPointTemp.at(i32TempIndex3).dLongtitude);
        }
    }
#endif

#if 0
    qDebug()<<"ShowReadFile";
    QMap<int, ST_DEVICE_DATA>::iterator MapIntDeviceDataIter = MapIntDeviceData.begin();
    for(MapIntDeviceDataIter; MapIntDeviceDataIter != MapIntDeviceData.end(); MapIntDeviceDataIter++) {
        qDebug()<<"Key is " + QString::number(MapIntDeviceDataIter.key());
        ST_DEVICE_DATA stDevDataTemp = MapIntDeviceDataIter.value();
        ST_GPS_DATA stGPSDataTemp = stDevDataTemp.stGPSData;

        qDebug()<<"dSpeed = " + QString::number(stGPSDataTemp.dSpeed) + " || dTime = " + QString::number(stGPSDataTemp.dTime);
    }
#endif


    qDebug()<<"Read file finish";
    fclose(ReadFile);
    fclose(ReadFile2);


    return EXIT_SUCCESS;
}

/**
 * @brief CAprioriData::loadTrainingData
 * @return
 * 给训练数据加标签
 */
int CAprioriData::loadTrainingData()
{
    QMap<int, ST_DEVICE_DATA>::iterator MapIntDeviceDataIter = MapIntDeviceData.begin();
    for(MapIntDeviceDataIter; MapIntDeviceDataIter != MapIntDeviceData.end(); MapIntDeviceDataIter++) {
        /**
         *   id, id_android,       speed,        time, distance, rating, rating_bus, rating_weather, car_or_bus, linha,     latitude,    longitude, track_id,           time
         *    1,          0, 19.21058562, 0.138048889,    2.652,      3,          0,              0,          1,    NA, -10.93934139, -37.06274211,        1, 2014/9/13 7:24
         * Type,          1,           2,           2,        2,      1,          1,              3,          3,      ,            4,            4,
         **/
        ST_DEVICE_DATA stDevDataTemp = MapIntDeviceDataIter.value();
        training_point[0].push_back(stDevDataTemp.stGPSData.i32Id_Android);
//        qDebug()<<"training_point[0] = " + QString::number(stDevDataTemp.stGPSData.i32Id_Android);
//        training_labels.push_back(1);

        training_point[1].push_back(stDevDataTemp.stGPSData.dSpeed);
//        qDebug()<<"training_point[1] = " + QString::number(stDevDataTemp.stGPSData.dSpeed);
//        training_labels.push_back(2);

        training_point[1].push_back(stDevDataTemp.stGPSData.dTime);
//        qDebug()<<"training_point[1] = " + QString::number(stDevDataTemp.stGPSData.dTime);
//        training_labels.push_back(2);

        training_point[1].push_back(stDevDataTemp.stGPSData.dDistance);
//        qDebug()<<"training_point[1] = " + QString::number(stDevDataTemp.stGPSData.dDistance);
//        training_labels.push_back(2);

        training_point[0].push_back(stDevDataTemp.stGPSData.i32Rating);
//        qDebug()<<"training_point[2] = " + QString::number(stDevDataTemp.stGPSData.i32Rating);
//        training_labels.push_back(3);

        training_point[0].push_back(stDevDataTemp.stGPSData.i32Rating_bus);
//        qDebug()<<"training_point[2] = " + QString::number(stDevDataTemp.stGPSData.i32Rating_bus);
//        training_labels.push_back(3);

        training_point[2].push_back(stDevDataTemp.stGPSData.i32Rating_weather);
//        qDebug()<<"training_point[3] = " + QString::number(stDevDataTemp.stGPSData.i32Rating_weather);
//        training_labels.push_back(4);

        training_point[2].push_back(stDevDataTemp.stGPSData.dCarORBus);
//        qDebug()<<"training_point[3] = " + QString::number(stDevDataTemp.stGPSData.dCarORBus);
//        training_labels.push_back(4);

        QVector<ST_GPS_POINT> VecStGPSPointTemp = stDevDataTemp.VecStGPSPoint;

        for(int i32TempIndex1 = 0; i32TempIndex1 < VecStGPSPointTemp.size(); i32TempIndex1++) {
            training_point[3].push_back(VecStGPSPointTemp.at(i32TempIndex1).dLatitude);
//            qDebug()<<"training_point[4] = " + QString::number(VecStGPSPointTemp.at(i32TempIndex1).dLatitude);
//            training_labels.push_back(5);

            training_point[3].push_back(VecStGPSPointTemp.at(i32TempIndex1).dLongtitude);
//            qDebug()<<"training_point[4] = " + QString::number(VecStGPSPointTemp.at(i32TempIndex1).dLongtitude);
//            training_labels.push_back(5);
        }

    }

    for(int i32TempIndex0 = 0; i32TempIndex0 < MAXCLASS; i32TempIndex0++) {
        training_data.push_back(training_point[i32TempIndex0]);
        training_labels.push_back(i32TempIndex0 + 1);
    }


#if 0
    qDebug()<<"SHow the train Data";
    for(int i32TempIndex0 = 0; i32TempIndex0 < 1; i32TempIndex0++) {
        qDebug()<<"i32TempIndex = " + QString::number(i32TempIndex0) + " || size = " + QString::number(training_data.at(i32TempIndex0).size());
        datapoint TempData = training_data.at(i32TempIndex0);
        for(int i32TempIndex1 = 0; i32TempIndex1 < TempData.size(); i32TempIndex1++) {
            qDebug()<<"i32TempIndex1 = " + QString::number(i32TempIndex1) + " || data = " + QString::number(TempData.at(i32TempIndex1));
        }
    }
#endif

}


int CAprioriData::WriteFile()
{
    FILE *WriteFileIdle;

    WriteFileIdle = fopen("TrainFile.txt", "wb+");
    for(int i32TempIndex = 0; i32TempIndex < training_data.size(); i32TempIndex++) {
        datapoint TempData = training_data.at(i32TempIndex);
        for(int i32TempIndex2 = 0; i32TempIndex2 < TempData.size(); i32TempIndex2++) {
//            QString TempWriteData = QString::number(TempData.at(i32TempIndex2)) + " " + QString::number(i32TempIndex);
            char WriteData[1024] = {0x0};
            sprintf(WriteData, "%lf %d\n", TempData.at(i32TempIndex2), i32TempIndex);
//            fwrite(WriteData, sizeof(WriteData), 1, WriteFileIdle);
            fprintf(WriteFileIdle, "%s", WriteData);
        }

    }

    fclose(WriteFileIdle);
    qDebug()<<"Write File Finish";
}



CAprioriData::~CAprioriData()
{

}
