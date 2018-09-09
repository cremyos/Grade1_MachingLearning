#include "apriori.h"

void Apriori::printMapSet(map< set<string> ,int> &mapSet)
{
    map< set<string>, int >::iterator it = mapSet.begin();
    while(it != mapSet.end()){
        set<string>::iterator itSet = it->first.begin();
        cout << "#" << it->second << "\t";
        cout << "[" ;
        while(itSet != it->first.end()){
            cout << *itSet << "," ;
            ++itSet;
        }
        cout << "]" << endl;
        ++it;
    }
}
void Apriori::printsetSet(set< set<string> > &setSet)
{
    set< set<string> >::iterator c2It = setSet.begin();
    while(c2It != setSet.end()){
       set<string>::iterator ckSetIt = (*c2It).begin();
       cout << "[";
        while(ckSetIt != (*c2It).end()){
            cout << *ckSetIt << "," ;
            ++ckSetIt;
        }
        cout << "]"<< endl;
        ++c2It;
    }
}
void Apriori::printSet(set<string> &setS)
{
    set<string>::iterator setIt = setS.begin();
    cout << "[";
    while(setIt != setS.end()){
        cout <<*setIt << "," ;
        ++setIt;
    }
    cout << "]" << endl;
}

//---------------------------------------------------------
//将文本数据存入到Map中，产生事务数据库D，即textDataBase
//---------------------------------------------------------
int Apriori::buildData()
{
    /*打开文本文件*/
    ifstream inFile;
    inFile.open(dataFileName.c_str());
    if(!inFile){
        cerr << "open " <<dataFileName << "is failed!" << endl;
        return EXIT_FAILURE;
    }
    /*读取文本行*/
    string textline;
    vector<string> lines_of_text;

    while(getline(inFile,textline))
        lines_of_text.push_back(textline);
    /*产生事务数据库*/
    int line_num ;
    for(line_num = 0; line_num != lines_of_text.size(); ++line_num){
        istringstream line(lines_of_text[line_num]);
        string word;
        while(line >> word){
            textDatabase[line_num].insert(word);
        }
    }
    textDatabaseCount = textDatabase.size();
    cout << "textDatabaseCount: " << textDatabaseCount << " " << line_num<< endl;
    return EXIT_SUCCESS;
}

//得到事物项集D中每项的重复度，即每项在事物项集中出现的频率
map< set<string>, int> Apriori::getTextDatabaseFre()
{
    map< set<string>, int> textDatabaseFre;
    map<long ,set<string> >::iterator textDataIt = textDatabase.begin();
    while(textDataIt != textDatabase.end()){
        pair<map< set<string>, int>::iterator, bool> ret =
            textDatabaseFre.insert(make_pair(textDataIt->second,1));
        if(!ret.second)
            ++ret.first->second;
        ++textDataIt;
    }
    return textDatabaseFre;
}

//得到事物项集D中每项的支持度,即统计数据集每一项作为子集出现的频率
//注:set<string>中的string是按从小到大的顺序排列的
//map< set<string>, int >
map< set<string>, int> Apriori::getTextDatabaseSurpport()
{
    map< set<string>, int> textDatabaseSurpport;
    map< set<string>, int> textDatabaseFre = getTextDatabaseFre();
    //cout << "原始数据集D各项的频度" << endl;
    //apriori.printMapSet(textDatabaseFre);
    map<long ,set<string> >::iterator textDataIt1 = textDatabase.begin();
    while(textDataIt1 != textDatabase.end()){
        textDatabaseSurpport.insert(make_pair(textDataIt1->second,0));
        map<long ,set<string> >::iterator textDataIt2 = textDatabase.begin();
        while(textDataIt2 != textDatabase.end()){
            if(retainAll(textDataIt1->second, textDataIt2->second) == textDataIt1->second)
                ++textDatabaseSurpport[textDataIt1->second];
            ++textDataIt2;
        }
        ++textDataIt1;
    }
    map< set<string>, int>::iterator textDatabaseFreIt = textDatabaseFre.begin();

    while(textDatabaseFreIt != textDatabaseFre.end()){
        textDatabaseSurpport[textDatabaseFreIt->first] =
            textDatabaseSurpport[textDatabaseFreIt->first]/textDatabaseFreIt->second;
        ++textDatabaseFreIt;
    }

    return textDatabaseSurpport;
}


//-------------------------------------------------------------------------
//获取候选1项集
//-------------------------------------------------------------------------
map<string, int> Apriori::getCandidate1ItemSet()
{
    map<string, int> candidate1ItemSetTemp;
    map<long, set<string> >::iterator mapIter = textDatabase.begin();
    set<string>::iterator setIter = mapIter->second.begin();

    while(mapIter != textDatabase.end()){
        while(setIter != mapIter->second.end()){
            pair<map<string, int>::iterator, bool> ret =
                candidate1ItemSetTemp.insert(make_pair(*setIter,1));
            if(!ret.second)
                ++ret.first->second;
            ++setIter;
        }
        ++mapIter;
        setIter = mapIter->second.begin();
    }
    return candidate1ItemSetTemp;
}

//-------------------------------------------------------------------------
//获取频繁1项集
//-------------------------------------------------------------------------
map< set<string>, int > Apriori::findFrequent1Itemsets()
{
    set<string> freq1Key;
    map< set<string>, int > freq1ItemSetMap;
    map<string, int> candidate1ItemSet = getCandidate1ItemSet();
    map<string, int>::iterator candIt = candidate1ItemSet.begin();
    while(candIt != candidate1ItemSet.end()){
        if(candIt->second >= minSup){
            freq1Key.erase(freq1Key.begin(),freq1Key.end());
            freq1Key.insert(candIt->first);
            freq1ItemSetMap[freq1Key] = candIt->second;
        }
        ++candIt;
    }
    return freq1ItemSetMap;
}

//-------------------------------------------------------------------------
//根据频繁k-1项集键集获取频繁k项集
//k>1
//-------------------------------------------------------------------------
map< set<string>, int > Apriori::getFreqKItemSet(int k, set< set<string> > freqMItemSet)
{
    map< set<string>, int > freqKItemSetMap;
    map< set<string>, int> candFreqKItemSetMap;
    set< set<string> > candFreqKItemSet = aprioriGen(k-1, freqMItemSet);

    //效率是根据min_sup的值的大小决定的，大，效率高，小效率高
    map<long, set<string> >::iterator mapIter = textDatabase.begin();
    //下面的while循环效率很低
    while(mapIter != textDatabase.end()){
        set<string> itValue = mapIter->second;
        set< set<string> >::iterator kit = candFreqKItemSet.begin();
        while(kit != candFreqKItemSet.end()){
            set<string> kSet = *kit;
            set<string> setTemp(kSet.begin(),kSet.end());
            removeAll(setTemp,itValue);
            if(setTemp.size() == 0){
                pair< map< set<string>, int >::iterator ,bool > ret =
                            candFreqKItemSetMap.insert(make_pair(kSet,1));
                if(!ret.second)
                    ++ret.first->second;
            }
            ++kit;
        }
        ++mapIter;
    }

    map< set<string>, int>::iterator candIt = candFreqKItemSetMap.begin();

    while(candIt != candFreqKItemSetMap.end()){
        if(candIt->second >= minSup){
            freqKItemSetMap[candIt->first] = candIt->second;
        }
        ++candIt;
    }

    return freqKItemSetMap;
}

//-------------------------------------------------------------------------
//取交集
//-------------------------------------------------------------------------
set<string> Apriori::retainAll(set<string> set1, set<string> set2)
{

    set<string>::iterator set1It = set1.begin();
    set<string> retSet;

    while(set1It != set1.end()){
        set<string>::iterator set2It = set2.begin();
        while(set2It != set2.end()){
            if((*set1It) == (*set2It)){
                retSet.insert(*set1It);
                break;
            }
            ++set2It;
        }
        ++set1It;
    }

    return retSet;
}

//-------------------------------------------------------------------------
//返回set1中去除了set2的数据集
//-------------------------------------------------------------------------
void Apriori::removeAll(set<string> &set1, set<string> &set2)
{
    set<string>::iterator set2It = set2.begin();
    while(set2It != set2.end()){
        set1.erase(*set2It);
        ++set2It;
        if(set1.size() == 0)break;
    }
}

//-------------------------------------------------------------------------
//取并集
//-------------------------------------------------------------------------
set<string> Apriori::addAll(set<string> &set1, set<string> &set2)
{
    set<string>::iterator set1It = set1.begin();
    set<string>::iterator set2It = set2.begin();
    set<string> retSet(set1.begin(),set1.end());

    while(set2It != set2.end()){
        retSet.insert(*set2It);
        ++set2It;
    }
    return retSet;
}

//-------------------------------------------------------------------------
//根据频繁(k-1)项集获取候选k项集
//m = k-1
//freqMItemSet:频繁k-1项集
//-------------------------------------------------------------------------
set< set<string> > Apriori::aprioriGen(int m, set< set<string> > &freqMItemSet)
{
    set< set<string> > candFreqKItemSet;
    set< set<string> >::iterator it = freqMItemSet.begin();
    set<string> originalItemSet;

    set<string> identicalSetRetain;

    //cout << "aprioriGen start" <<endl;
    while(it != freqMItemSet.end()){
        originalItemSet = *it;

        /*itr其实就是当前it自加一次所指*/
        set< set<string> >::iterator itr = ++it;
        while(itr != freqMItemSet.end()){
            set<string> identicalSet(originalItemSet.begin(),originalItemSet.end());
            set<string> setS(*itr);
            identicalSetRetain.erase(identicalSetRetain.begin(),identicalSetRetain.end());
            identicalSetRetain = addAll(identicalSet,setS);//是取originalItemSet和setS的交集

            if(identicalSetRetain.size() == m+1){
                if(!has_infrequent_subset(identicalSetRetain, freqMItemSet))
                    candFreqKItemSet.insert(identicalSetRetain);
            }
            ++itr;
        }
    }
    //cout << "aprioriGen end" <<endl;
    return candFreqKItemSet;
}

//-------------------------------------------------------------------------
//使用先验知识，剪枝。删除候选k项集中存在k-1项的子集
//-------------------------------------------------------------------------
bool Apriori::has_infrequent_subset(set<string> &candKItemSet, set< set<string> > &freqMItemSet)
{
    int occurs = 0;

    if(freqMItemSet.count(candKItemSet))
        return true;

    return false;
}
//-------------------------------------------------------------------------
//获取mapSet的键值，存放于set中
//-------------------------------------------------------------------------
set< set<string> > Apriori::keySet(map< set<string>, int > &mapSet)
{
    map< set<string>, int >::iterator it = mapSet.begin();
    set< set<string> > retSet;

    while(it != mapSet.end()){
        retSet.insert(it->first);
        ++it;
    }

    return retSet;
}
