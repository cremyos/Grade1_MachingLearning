/**
 * Name: Apriori
 * Author: Lniper
 * Date: 2018.02.24
 * Description: Apriori Algorithm. Reference "http://blog.csdn.net/FreeApe/article/details/49562315"
 **/

#include <QCoreApplication>
#include <iostream>
#include "apriori.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int min_sup = 2;

    QString TempdataFileName = "BeiJingPM2_5.csv";

    Apriori apriori(TempdataFileName, min_sup);
    /*获取文本文件中原始数据*/
    apriori.buildData();


    map<int, map<set<string>, int>> FirstItem;
    map<set<string>, int> freq1ItemSetMap = apriori.findFreq1ItemSet();
    set<set<string>> freqKItemSet = apriori.keySet(freq1ItemSetMap);
    FirstItem.insert(make_pair(1,freq1ItemSetMap));

    //for循环退出条件为:得到频繁k项集为空集时

    for(int k = 2; ;++k){
        //cout <<"k= " << k <<endl;
        map<set<string>, int> freqKItemSetMap = apriori.getFreqKItemSet(k, freqKItemSet);
        FirstItem.insert(make_pair(k,freqKItemSetMap));
        if(freqKItemSetMap.size() != 0) {
            set<set<string>> freqKItemSetTemp = apriori.keySet(freqKItemSetMap);
            freqKItemSet = apriori.keySet(freqKItemSetMap);
        }
        else {
            //cout <<"k= " << k <<endl;
            break;
        }
    }

    //打印所有满足min_sup的频繁集
    map<int, map<set<string>, int>>::iterator allLIt = FirstItem.begin();
    while(allLIt != FirstItem.end()){
        if(allLIt->second.size() != 0) {
            qDebug()<<"频繁k " + QString::number(allLIt->first) +  "项集: ";
        }
        apriori.printMapSet(allLIt->second);
        ++allLIt;
    }

#if 1

    //获取每个topic-i.txt的最大频繁项集
    ofstream maxFstream;

    string maxFileName = "max.txt";

    maxFstream.open(maxFileName, fstream::out);
    map<int, map<set<string>, int>>::iterator maxFirItemIter = --(--FirstItem.end());
    map<set<string>, int>::iterator maxit = maxFirItemIter->second.begin();
    while(maxit != maxFirItemIter->second.end()){
        set<string>::iterator maxitSet = maxit->first.begin();
        maxFstream <<"#" << maxit->second <<"\t";
        maxFstream <<"[" ;
        while(maxitSet != maxit->first.end()){
            maxFstream <<*maxitSet <<"," ;
            ++maxitSet;
        }
        maxFstream <<"]" << endl;
        ++maxit;
    }
    maxFstream.close();

#if 0

    /**
     * 获取每个topic-i.txt的所有闭项集
     * 第一步:求原始数据集D中的每项的支持度
     * 第二步:比较第k项频繁项集的每一项的直接超集的支持度是否与该项支持度相同，直至比较完该k项频繁项集
     * 第三步:循环所有的频繁项集，并保存闭项集
     * 第四步:将所有的闭项集输入到closed-i.txt中
     **/
    ofstream closedFstream;

    string closeFile = "closed.txt";
    closedFstream.open(closeFile, fstream::out);

    map<set<string>, int> textDatabaseSur = apriori.getTextDatabaseSurpport();
    qDebug()<<"原始数据集D各项支持度";
    apriori.printMapSet(textDatabaseSur);

    map<int, map<set<string>, int>>::iterator closedLIt = FirstItem.begin();
    while(closedLIt != FirstItem.end()){
        map<set<string> , int>::iterator closedMapSetIt = closedLIt->second.begin();
        while(closedMapSetIt != closedLIt->second.end()){
            map<set<string>, int>::iterator textDatabaseSurIt = textDatabaseSur.begin();
            while(textDatabaseSurIt != textDatabaseSur.end()){
                if(apriori.retainAll(closedMapSetIt->first,textDatabaseSurIt->first) == (closedMapSetIt->first)){
                    if((closedMapSetIt->second) == (textDatabaseSurIt->second))
                        break;
                }

                ++textDatabaseSurIt;
            }
            //该k项频繁项集不是闭项集
            if(textDatabaseSurIt != textDatabaseSur.end()){
                break;
            }
            ++closedMapSetIt;
        }
        //该k项频繁项集是闭项集,写入到文件中保存
        if((closedMapSetIt == closedLIt->second.end()) && (closedLIt->second.size() != 0)){
            qDebug()<<"存在第" + QString::number(closedLIt->first) + "项频繁闭项集:";
            //apriori.printMapSet(closedLIt->second);
            map<set<string>, int>::iterator closedit = closedLIt->second.begin();
            while(closedit != closedLIt->second.end()){
                set<string>::iterator itSet = closedit->first.begin();
                closedFstream <<"#" << closedit->second <<"\t";
                closedFstream <<"[" ;
                while(itSet != closedit->first.end()){
                    closedFstream <<*itSet <<"," ;
                    ++itSet;
                }
                closedFstream <<"]" << endl;
                ++closedit;
            }
        }
        ++closedLIt;
    }
#endif

#endif

    return a.exec();
}
