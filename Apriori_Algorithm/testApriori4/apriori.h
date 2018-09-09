#ifndef APRIORI_H
#define APRIORI_H

#include <iostream>
#include <cstdlib>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <QDebug>

using namespace std;

class Apriori
{

public:
    Apriori(string dataFileName,float minSup){
        this->dataFileName = dataFileName;
        this->minSup = minSup;
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
    string dataFileName;
    map<long, set<string>> textDatabase;   //事务数据库
    float minSup;                           //最小支持度,(使用绝对支持度)
    long textDatabaseCount;                 //事务数据库中的事务数
    map<set<set<string>>, int > freqItemSet;             //候选项集集合
    map<set<set<string>>, int > candidateItemSet;        //频繁项集集合
};

#endif // APRIORI_H
