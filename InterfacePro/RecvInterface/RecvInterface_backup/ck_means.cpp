#include "ck_means.h"

CK_means::CK_means()
{
    /**
     * 初始化参数
     */
    assignSampleCenter = new double[MAXDATACOUNT];
    stTypeData = new ST_TYPE_DATA[MAXDATACOUNT];
    memset(Samples, 0, sizeof(double) * MAXDATACOUNT);
    memset(centerX, 0x0, sizeof(double) * MAXCLUSTER);
    memset(oldcenterX, 0x0, sizeof(double) * MAXCLUSTER);
    k = 2;
    memset(centroidcount, 0, sizeof(int) * MAXCLUSTER);
    memset(classK, 0, sizeof(classK));
    i32SampleIndex = 0;

#if 0
//    stClassTypedata =  (ST_CLASS_TYPE_DATA *) malloc(sizeof(ST_CLASS_TYPE_DATA) * MAXCLUSTER);
    for(int i32TempIndex = 0; i32TempIndex < MAXCLUSTER; i32TempIndex++) {
        stClassTypedata[i32TempIndex].i32Class = 0;
        stClassTypedata[i32TempIndex].stTypedata = new ST_TYPE_DATA [MAXDATACOUNT];
    }
#endif


    centerX[0] = 20337.50;
    centerX[1] = 52224;

    qDebug()<<"Init Success!";
}


/**
 * @brief CK_means::AddSamples
 * @param x
 * 增加样本数据
 */
void CK_means::AddSamples(int i32Type, double x)
{
//    Samples[i32SampleIndex] = x;

    stTypeData[i32SampleIndex].dData = x;
    stTypeData[i32SampleIndex].i32Type = i32Type;

#if 0
    stClassTypedata[0].stTypedata[i32SampleIndex].dData = x;
    stClassTypedata[0].stTypedata[i32SampleIndex].i32Type = i32Type;
#endif


//    ClassIntStruMap.insert(i32SampleIndex, i32Type);

    i32SampleIndex ++;
    i32SampleIndex = i32SampleIndex % MAXDATACOUNT;

}

/**
 * @brief CK_means::calculateDistance
 * @param x1
 * @param x2
 * @return
 * 计算两个数据的距离
 */
double CK_means::calculateDistance(double x1, double x2)
{
    double part1 = (x2 - x1) * (x2 - x1);

    part1 = sqrt(part1);

    return part1;
}

/**
 * @brief CK_means::assignCentroid
 * @param x1
 * @param point
 * 计算中心点。
 */
void CK_means::assignCentroid(double x1, int point)
{
    double smallest = 99999;
    int chosenCentroid = 99999;

    for(int i = 0; i < k; i++)
    {
        double distanceToCentroid = calculateDistance(x1, centerX[i]);

//        qDebug()<<"distanceToCentroid = " + QString::number(distanceToCentroid);

        if( distanceToCentroid < smallest )
        {
            smallest = distanceToCentroid;
            chosenCentroid = i;
        }

    }
    assignSampleCenter[point] = chosenCentroid;

}


void CK_means::calculateNewCentroid()
{
    for( int i=0; i < k; i++)
    {
//        oldcenterX[i] = centerX[i];

        double xsum = 0;
        double count = 0;

        int KIndex = 0;

        for( int j=0; j < i32SampleIndex; j++)
        {
            qDebug()<<"check Before if assignSampleCenter[ " + QString::number(j) + " ] = " + QString::number(assignSampleCenter[j]);
            if(assignSampleCenter[j] == i)
            {
//                xsum += Samples[j];
//                count++;
//                classK[i][KIndex++] = stTypeData[j].dData;
                qDebug()<<"stTypeData[ " + QString::number(j) + " ].dData = " + QString::number(stTypeData[j].dData);
                qDebug()<<" j = " + QString::number(j) + " || i = " + QString::number(i);
#if 0
                stClassTypedata[i].i32Class = i;
                stClassTypedata[i].stTypedata[KIndex++] = stTypeData[j];
#endif


#if 0
                QMap<double, int>::iterator IntDouMapIter = IntDouMap.find(Samples[j]);
                ClassIntDouMap.insertMulti(i, IntDouMapIter);

                QMap<double, int> TempIntDouMap;
                TempIntDouMap.insert(IntDouMapIter.key(), IntDouMapIter.value());
                ClassIntDouMap.insert(i, TempIntDouMap);
#endif
//                KIndex++;
            }

        }
        if(count != 0) {
            centerX[i] = xsum / count;
        }


    }
}

void CK_means::ShowData()
{
    qDebug()<<"ShowData() :";
    for(int iIndex = 0; iIndex < i32SampleIndex; iIndex++) {
        qDebug()<< "stTypeData [ " + QString::number(iIndex) + " ].type = " + QString::number(stTypeData[iIndex].i32Type) + " || stTypeData[ " + QString::number(iIndex) + " ].data = " + QString::number(stTypeData[iIndex].dData);
    }

}


CK_means::~CK_means()
{
#if 0
    for(int iIndex = 0; iIndex < MAXDATACOUNT; iIndex++) {
        qDebug()<< "Samples[ " + QString::number(iIndex) + " ] = " + QString::number(Samples[iIndex]);
    }

    for(int i = 0; i < k; i++) {
        for(int j = 0; j < MAXDATACOUNT; j++) {
            qDebug()<< "classK[ " + QString::number(i) + " ] [ " + QString::number(j) + " ] = " + QString::number(classK[i][j]);
        }

    }

    qDebug()<<"centerX[0] = " + QString::number(centerX[0]) + "centerX[1] = " + QString::number(centerX[1]);
#endif
#if 0
    map<int, int>::iterator MapIntIter = ClassIntStruMap.begin();
    cout<<"class                Type            Data"<<endl;
    for(MapIntIter; MapIntIter != ClassIntStruMap.end(); MapIntIter++) {
        int i32TempIndex = MapIntIter->first;
        cout<<MapIntIter->second<<"             "<< stTypeData[i32TempIndex].i32Type << "           " <<stTypeData[i32TempIndex].dData<<endl;
    }
#endif

    delete []assignSampleCenter;
    delete []stTypeData;
}
