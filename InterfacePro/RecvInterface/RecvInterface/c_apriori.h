#ifndef C_APRIORI_H
#define C_APRIORI_H


#include <iostream>
#include <map>
#include <string>
#include <QString>
#include <sstream>
#include <set>
#include <QDebug>
#include <fstream>

using namespace std;


typedef unsigned char BYTE;


class C_Apriori
{
public:
    map<long, set<string>> textDatabase;   ///< 事务数据库
    long textDatabaseCount;                 ///< 事务数据库中的事务数
    map<int, string> mType_Name;            ///< 事务数据map
    int minSup;

public:
    C_Apriori();
    ~C_Apriori(){};

    void createMap();                   ///< 创建事务
    map< set<string>, int > findFreq1ItemSet();     ///< 获取频繁1项集
    map<string, int> getCand1ItemSet();         ///< 获取候选1项集
    map< set<string>, int > getFreqKItemSet(int k, set< set<string>> freqMItemSet); ///< 根据频繁k-1项集键集获取频繁k项集(K > 1)
    set<set<string>> keySet(map<set<string>, int > &mapSet);            ///< 获取mapSet的键值，存放于set中
    void printMapSet(map<set<string> ,int> &mapSet);
    set< set<string>> aprioriK_1Gen(int m, set< set<string>> &freqMItemSet);
    void removeAll(set<string> &set1, set<string> &set2);
    set<string> addAll(set<string> &set1, set<string> &set2);
    bool has_infrequent_subset(set<string> &candKItemSet, set<set<string>> &freqMItemSet);


};

#endif // C_APRIORI_H
