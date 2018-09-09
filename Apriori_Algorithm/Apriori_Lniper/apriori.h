#ifndef APRIORI_H
#define APRIORI_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <map>
#include <set>
#include <fstream>
#include <string>
//#include <vector>
//#include <sstream>
//#include <utility>
#include <QDebug>
#include <QVector>


using namespace std;

typedef struct {
    QString Date;            ///< 时间
    double dPM2_5;          ///< PM2.5
    double DEWP;            ///< Dew Point
    double TEMP;            ///< Temperature
    double PRES;            ///< Pressure
    double dIs;             ///< Cumulated hours of snow
    double dIr;             ///< Cumulated hours of rain
}ST_PM_DATA;

class Apriori
{

public:
    Apriori(QString TempdataFileName,float _minSup){
        dataFileName = TempdataFileName;
        minSup = _minSup;
    }

/*Functions*/
public:
    void printMapSet(map<set<string> ,int> &mapSet);
    void printsetSet(set<set<string>> &);
    void printSet(set<string> &);

    int buildData();
    map<set<string>, int> getTextDatabaseFre();
    map<set<string>, int> getTextDatabaseSurpport();
    map<string, int> getCand1ItemSet();
    map<set<string>, int > findFreq1ItemSet();
    set<set<string>> aprioriK_1Gen(int m, set<set<string>> &);
    bool has_infrequent_subset(set<string> &, set<set<string>> &);
    map<set<string>, int > getFreqKItemSet(int k, set<set<string>> freqMItemSet);
    set<set<string>> keySet(map<set<string>, int > &mapSet);
    set<string> retainAll(set<string> set1, set<string> set2);
/*Functions*/
private:
    void removeAll(set<string> &set1, set<string> &set2);
    set<string> addAll(set<string> &set1, set<string> &set2);

/*Variables*/
private:
    QString dataFileName;
    map<long, set<string>> textDatabase;   //事务数据库
    float minSup;                           //最小支持度,(使用绝对支持度)
    long textDatabaseCount;                 //事务数据库中的事务数
    map<set<set<string>>, int > freqItemSet;             //候选项集集合
    map<set<set<string>>, int > candidateItemSet;        //频繁项集集合

    vector<ST_PM_DATA> vecSTPM;             ///< 北京PM2.5数据
};

#endif // APRIORI_H
