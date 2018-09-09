#include <QCoreApplication>
#include <fstream>
#include <string>
#include <iostream>
#include "apriori.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc != 5){
        cout << "usage: apriori.exe [min_sup] [topic-i.txt] [max-i.txt] [closed-i.txt]" << endl;
        return 0;
    }
    int min_sup = atoi(argv[1]);


    Apriori apriori(argv[2],min_sup);
    /*获取文本文件中原始数据*/
    apriori.buildData();
#if (1)
    map<int, map< set<string>, int > > L;
    map< set<string>, int > freq1ItemSetMap = apriori.findFrequent1Itemsets();
    set< set<string> > freqKItemSet = apriori.keySet(freq1ItemSetMap);
    L.insert(make_pair(1,freq1ItemSetMap));
    //for循环退出条件为:得到频繁k项集为空集时

    for(int k = 2; ;++k){
        //cout << "k= " << k <<endl;
        map< set<string>, int> freqKItemSetMap = apriori.getFreqKItemSet(k, freqKItemSet);
        L.insert(make_pair(k,freqKItemSetMap));
        if(freqKItemSetMap.size() != 0) {
            set< set<string> > freqKItemSetTemp = apriori.keySet(freqKItemSetMap);
            freqKItemSet = apriori.keySet(freqKItemSetMap);
        }
        else {
            //cout << "k= " << k <<endl;
            break;
        }
    }

    //打印所有满足min_sup的频繁集
    map<int, map< set<string>, int > >::iterator allLIt = L.begin();
    while(allLIt != L.end()){
        if(allLIt->second.size() != 0)cout << "频繁k" << allLIt->first << "项集: " << endl;
        apriori.printMapSet(allLIt->second);
        ++allLIt;
    }
    //获取每个topic-i.txt的最大频繁项集
    ofstream maxFstream;

    maxFstream.open(argv[3],fstream::out);
    map<int, map< set<string>, int > >::iterator maxLIt = --(--L.end());
    map< set<string>, int >::iterator maxit = maxLIt->second.begin();
    while(maxit != maxLIt->second.end()){
        set<string>::iterator maxitSet = maxit->first.begin();
        maxFstream << "#" << maxit->second << "\t";
        maxFstream << "[" ;
        while(maxitSet != maxit->first.end()){
            maxFstream << *maxitSet << "," ;
            ++maxitSet;
        }
        maxFstream << "]" << endl;
        ++maxit;
    }
    maxFstream.close();
    //获取每个topic-i.txt的所有闭项集
    //第一步:求原始数据集D中的每项的支持度
    //第二步:比较第k项频繁项集的每一项的直接超集的支持度是否与该项支持度相同，直至比较完该k项频繁项集
    //第三步:循环所有的频繁项集，并保存闭项集
    //第四步:将所有的闭项集输入到closed-i.txt中
    ofstream closedFstream;
    closedFstream.open(argv[4],fstream::out);

    map< set<string>, int> textDatabaseSur = apriori.getTextDatabaseSurpport();
    cout << "原始数据集D各项支持度" <<endl;
    apriori.printMapSet(textDatabaseSur);

    map<int, map< set<string>, int > >::iterator closedLIt = L.begin();
    while(closedLIt != L.end()){
        map< set<string> , int>::iterator closedMapSetIt = closedLIt->second.begin();
        while(closedMapSetIt != closedLIt->second.end()){
            map< set<string>, int>::iterator textDatabaseSurIt = textDatabaseSur.begin();
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
            cout << "存在第"<< closedLIt->first << "项频繁闭项集:" <<endl;
            //apriori.printMapSet(closedLIt->second);
            map< set<string>, int >::iterator closedit = closedLIt->second.begin();
            while(closedit != closedLIt->second.end()){
                set<string>::iterator itSet = closedit->first.begin();
                closedFstream << "#" << closedit->second << "\t";
                closedFstream << "[" ;
                while(itSet != closedit->first.end()){
                    closedFstream << *itSet << "," ;
                    ++itSet;
                }
                closedFstream << "]" << endl;
                ++closedit;
            }
        }
        ++closedLIt;
    }

#endif

#if (0)
    /*获取文本文件中原始数据*/
    apriori.buildData();
    cout << "----------------" << endl;
    /*获取候选集1*/
    map<string, int> candidate1ItemSet = apriori.getCandidate1ItemSet();
    cout << "候选1项集大小: " << candidate1ItemSet.size() << endl;
    /*获取频繁项集1*/
    map< set<string>, int > freq1ItemSetMap = apriori.findFrequent1Itemsets();
    cout << "频繁1项集大小: " << freq1ItemSetMap.size() << endl;
    /*打印频繁项集1*/
    cout << "-频繁1项集-" << endl;
    apriori.printMapSet(freq1ItemSetMap);
    /*获取候选集2*/
    set< set<string> > C2 = apriori.aprioriGen(1, apriori.keySet(freq1ItemSetMap));
    cout << "-候选2项集-" << endl;
    apriori.printsetSet(C2);
    /*获取频繁2项集*/
    set< set<string> > C1 = apriori.keySet(freq1ItemSetMap);
    cout << "-频繁1项集键集--" << endl;
    apriori.printsetSet(C1);
    map< set<string>, int> L2 = apriori.getFreqKItemSet(2,C1);
    cout << "---频繁2项集----" << endl;
    apriori.printMapSet(L2);
    /*获取频繁3项集*/
    map< set<string>, int> L3 = apriori.getFreqKItemSet(3,C2);
    cout << "---频繁3项集----" << endl;
    apriori.printMapSet(L3);
#endif

    return a.exec();
}
