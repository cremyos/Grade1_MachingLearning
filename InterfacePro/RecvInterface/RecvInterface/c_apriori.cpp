#include "c_apriori.h"

void C_Apriori::createMap()
{
    mType_Name.insert(pair<int, string>(1, "A C"));
    mType_Name.insert(pair<int, string>(2, "A B"));
    mType_Name.insert(pair<int, string>(3, "A B C"));
    mType_Name.insert(pair<int, string>(4, "B C"));
    mType_Name.insert(pair<int, string>(5, "A C D"));
    mType_Name.insert(pair<int, string>(6, "A B D"));
    mType_Name.insert(pair<int, string>(7, "A D"));
    mType_Name.insert(pair<int, string>(8, "A C E"));
}



C_Apriori::C_Apriori()
{
    createMap();

    minSup = 2;
    /**
     * 创建样本集
     **/
    vector<string> lines_of_text;

    map<int, string>::iterator MapIntStrIter = mType_Name.begin();

    for(MapIntStrIter; MapIntStrIter != mType_Name.end(); MapIntStrIter++) {
        lines_of_text.push_back(MapIntStrIter->second);
#if 0
        string TempStr = MapIntStrIter->second;
        for(int i32TempIndex = 0; i32TempIndex < TempStr.length(); i32TempIndex++) {
            lines_of_text.push_back(TempStr[i32TempIndex]);
        }
        cout<<endl;
#endif

    }


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
    cout <<"textDatabaseCount: " << textDatabaseCount <<" " << line_num<< endl;

    map<long, set<string>>::iterator MapLonSetIter = textDatabase.begin();
    for(MapLonSetIter; MapLonSetIter != textDatabase.end(); MapLonSetIter++) {
        cout<< MapLonSetIter->first<<" ";
        set<string>::iterator TempSetIter = MapLonSetIter->second.begin();
        for(TempSetIter; TempSetIter !=  MapLonSetIter->second.end(); TempSetIter++) {
            cout<<*TempSetIter<<endl;
        }
        cout<<endl;

    }

    map<int, map<set<string>, int>> FirstItem;
    map<set<string>, int> freq1ItemSetMap = findFreq1ItemSet();
    set<set<string>> freqKItemSet = keySet(freq1ItemSetMap);
    FirstItem.insert(make_pair(1,freq1ItemSetMap));

    //for循环退出条件为:得到频繁k项集为空集时

    for(int k = 2; ;++k){
        //cout <<"k= " << k <<endl;
        map<set<string>, int> freqKItemSetMap = getFreqKItemSet(k, freqKItemSet);
        FirstItem.insert(make_pair(k,freqKItemSetMap));
        if(freqKItemSetMap.size() != 0) {
            set<set<string>> freqKItemSetTemp = keySet(freqKItemSetMap);
            freqKItemSet = keySet(freqKItemSetMap);
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
        printMapSet(allLIt->second);
        ++allLIt;
    }

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

}

/**
 * 获取频繁1项集
 **/
map< set<string>, int > C_Apriori::findFreq1ItemSet()
{
    set<string> freq1Key;
    map< set<string>, int > freq1_ItemSetMap;
    map<string, int> candidate1ItemSet = getCand1ItemSet();
    map<string, int>::iterator candIter = candidate1ItemSet.begin();
    while(candIter != candidate1ItemSet.end()){
        if(candIter->second >= minSup){
            /**
             * 判断当前的元素数量是否大于一定的阈值
             **/
            freq1Key.erase(freq1Key.begin(),freq1Key.end());
            freq1Key.insert(candIter->first);
            freq1_ItemSetMap[freq1Key] = candIter->second;
        }
        ++candIter;
    }

#if 0
    cout<<"Check findFrequen1Itemsets()"<<endl;
    map< set<string>, int >::iterator freq1_ItemSetMapIter = freq1_ItemSetMap.begin();
    for(freq1_ItemSetMapIter; freq1_ItemSetMapIter != freq1_ItemSetMap.end(); freq1_ItemSetMapIter++) {
        set<string>::iterator TempSetStrIter = freq1_ItemSetMapIter->first.begin();
        for(TempSetStrIter; TempSetStrIter != freq1_ItemSetMapIter->first.end(); TempSetStrIter++) {
            cout<<*TempSetStrIter<<" || ";
        }
        cout<<freq1_ItemSetMapIter->second<<endl;
    }
#endif
    return freq1_ItemSetMap;
}

/**
 * 获取候选1项集
 **/
map<string, int> C_Apriori::getCand1ItemSet()
{
    map<string, int> candidate1ItemSetTemp;
    map<long, set<string>>::iterator mapIter = textDatabase.begin();
    set<string>::iterator setTempIter = mapIter->second.begin();

#if 0
    qDebug()<<"getCand1ItemSet()";
#endif

    while(mapIter != textDatabase.end()) {
        while(setTempIter != mapIter->second.end()) {
            /**
             * 有一个键—值pair 形参的insert 版本将返回一个值：包含一个迭代器和一个bool 值的pair 对象。其中迭代器指向map中具有相
             * 应键的元素，而bool 值则表示是否插入了该元素。如果该元素已经在容器中，则其关联的值保持不变，返回bool值为false；如
             * 果该键不在容器中，则插入新元素，且bool值为true。在这两种情况下，迭代器都将指向具有给定键的元素。
             */
            pair<map<string, int>::iterator, bool> ret = candidate1ItemSetTemp.insert(make_pair(*setTempIter,1));
            if(!ret.second) {
                ++ret.first->second;
            }
            ++setTempIter;


        }

#if 0
        /**
         * 遍历当前数据库
         */
        cout<<mapIter->first<<" || ";
        set<string>::iterator TestIter = mapIter->second.begin();
        for(int iTemp = 0; iTemp < mapIter->second.size(); iTemp++) {
            cout<<*TestIter<<" ";
            TestIter++;
        }
        cout<<endl;
#endif

        ++mapIter;
        setTempIter = mapIter->second.begin();

    }

#if 0
    /**
     * 检查candidate1ItemSetTemp
     **/
    cout<<"check candidate1ItemSetTemp"<<endl;
    map<string, int>::iterator candidate1ItemSetTempIter = candidate1ItemSetTemp.begin();
    for(candidate1ItemSetTempIter; candidate1ItemSetTempIter != candidate1ItemSetTemp.end(); candidate1ItemSetTempIter++) {
        cout<<candidate1ItemSetTempIter->first<<" || "<<candidate1ItemSetTempIter->second<<endl;
    }

#endif

    return candidate1ItemSetTemp;
}

/**
 * 根据频繁k-1项集键集获取频繁k项集(K > 1)
 **/
map< set<string>, int > C_Apriori::getFreqKItemSet(int k, set< set<string>> freqMItemSet)
{
    map< set<string>, int > freqKItemSetMap;
    map< set<string>, int> candFreqKItemSetMap;
    set< set<string>> candFreqKItemSet = aprioriK_1Gen(k-1, freqMItemSet);

    //效率是根据min_sup的值的大小决定的，大，效率高，小效率高
    map<long, set<string>>::iterator mapIter = textDatabase.begin();
    //下面的while循环效率很低
    while(mapIter != textDatabase.end()){
        set<string> itValue = mapIter->second;
        set< set<string>>::iterator kit = candFreqKItemSet.begin();
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
            /**
             * 对于阈值有个筛选
             **/
            freqKItemSetMap[candIt->first] = candIt->second;
        }
        ++candIt;
    }

#if 0
    cout<<"In getFreqKItemSet()"<<endl;
    map< set<string>, int >::iterator TempMapIter = freqKItemSetMap.begin();
    for(TempMapIter; TempMapIter != freqKItemSetMap.end(); TempMapIter++) {
        set<string>::iterator TempSetIter = TempMapIter->first.begin();
        for(TempSetIter; TempSetIter != TempMapIter->first.end(); TempSetIter++) {
            cout<<*TempSetIter<<" ";
        }
        cout<<"|| "<<TempMapIter->second<<endl;
    }

#endif

    return freqKItemSetMap;
}

/**
 * 获取mapSet的键值，存放于set中
 **/
set<set<string>> C_Apriori::keySet(map<set<string>, int > &mapSet)
{
    map<set<string>, int>::iterator it = mapSet.begin();
    set<set<string>> retSet;

#if 0
    cout<<"check KeySet()"<<endl;
#endif


    while(it != mapSet.end()){
        retSet.insert(it->first);
        ++it;
    }

#if 0
    set<set<string>>::iterator TempSetSetIter = retSet.begin();
    for(TempSetSetIter; TempSetSetIter != retSet.end(); TempSetSetIter++) {
        set<string>::iterator TempSetIter = TempSetSetIter->begin();
        for(TempSetIter; TempSetIter != TempSetSetIter->end(); TempSetIter++) {
            cout<<*TempSetIter<<" ";
        }
        cout<<endl;
    }

#endif

    return retSet;
}


/**
 * @brief C_Apriori::printMapSet
 * @param mapSet
 */
void C_Apriori::printMapSet(map<set<string> ,int> &mapSet)
{
    map<set<string>, int >::iterator it = mapSet.begin();
    while(it != mapSet.end()){
        set<string>::iterator itSet = it->first.begin();
        cout <<"#" << it->second <<"\t";
        cout <<"[" ;
        while(itSet != it->first.end()){
            cout <<*itSet <<"," ;
            ++itSet;
        }
        cout <<"]" << endl;
        ++it;
    }
}


/**
 * 根据频繁(k-1)项集获取候选k项集
 * m = k-1
 * freqMItemSet:频繁k-1项集
 **/
set< set<string>> C_Apriori::aprioriK_1Gen(int m, set< set<string>> &freqMItemSet)
{
    set< set<string>> candFreqKItemSet;
    set< set<string>>::iterator it = freqMItemSet.begin();
    set<string> originalItemSet;

    set<string> identicalSetRetain;

    //cout <<"aprioriK_1Gen start" <<endl;
    while(it != freqMItemSet.end()){
        originalItemSet = *it;

        /*itr其实就是当前it自加一次所指*/
        set< set<string>>::iterator itr = ++it;
        while(itr != freqMItemSet.end()){
            set<string> identicalSet(originalItemSet.begin(),originalItemSet.end());
            set<string> setS(*itr);
            identicalSetRetain.erase(identicalSetRetain.begin(),identicalSetRetain.end());
            //是取originalItemSet和setS的交集
            identicalSetRetain = addAll(identicalSet,setS);

            if(identicalSetRetain.size() == m+1){
                if(!has_infrequent_subset(identicalSetRetain, freqMItemSet))
                    candFreqKItemSet.insert(identicalSetRetain);
            }
            ++itr;
        }
    }
    //cout <<"aprioriK_1Gen end" <<endl;
    return candFreqKItemSet;
}


/**
 * 返回set1中去除了set2的数据集
 **/
void C_Apriori::removeAll(set<string> &set1, set<string> &set2)
{
    set<string>::iterator set2It = set2.begin();
    while(set2It != set2.end()){
        set1.erase(*set2It);
        ++set2It;
        if(set1.size() == 0)break;
    }
}


/**
 * 取取交集
 **/
set<string> C_Apriori::addAll(set<string> &set1, set<string> &set2)
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


/**
 * 使用先验知识，剪枝。删除候选k项集中存在k-1项的子集
 **/
bool C_Apriori::has_infrequent_subset(set<string> &candKItemSet, set<set<string>> &freqMItemSet)
{
    int occurs = 0;

    if(freqMItemSet.count(candKItemSet))
        return true;

    return false;
}


