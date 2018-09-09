#include "ck_means.h"

CK_means::CK_means()
{
    /**
     * 初始化参数
     */
    assignSampleCenter = new double[MAXDATACOUNT];
    memset(Samples, 0, sizeof(double) * MAXDATACOUNT);
    memset(centerX, 0x0, sizeof(double) * MAXCLUSTER);
    k = 2;
    memset(classK, 0, sizeof(classK));
    i32SampleIndex = 0;

    stTypeData = new ST_TYPE_DATA[MAXDATACOUNT];

    centerX[0] = 20337.50;
    centerX[1] = 52224;
}

/**
 * @brief CK_means::AddSamples
 * @param x
 * 增加样本数据
 */
void CK_means::AddSamples(int i32Type, double x)
{
    stTypeData[i32SampleIndex].dData = x;
    stTypeData[i32SampleIndex].i32Type = i32Type;

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

        double xsum = 0;
        double count = 0;

        int KIndex = 0;

        for( int j=0; j < i32SampleIndex; j++)
        {
            qDebug()<<"check Before if assignSampleCenter[ " + QString::number(j) + " ] = " + QString::number(assignSampleCenter[j]);
            if(assignSampleCenter[j] == i)
            {
                MapInt.insert(pair<int, int>(j, i));
            }

        }
        if(count != 0) {
            centerX[i] = xsum / count;
        }


    }
}

void CK_means::ShowData()
{


}


CK_means::~CK_means()
{
#if 1
    map<int, int>::iterator MapIntIter = MapInt.begin();
    cout<<"Class                Type            Value"<<endl;
    for(MapIntIter; MapIntIter != MapInt.end(); MapIntIter++) {
        cout<<MapIntIter->second<<"           ";
        cout<<stTypeData[MapIntIter->first].i32Type<<"          "<<stTypeData[MapIntIter->first].dData<<endl;
    }
#endif

    delete []stTypeData;
    delete []assignSampleCenter;
}
