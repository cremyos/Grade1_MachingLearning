#include "apriori.h"

/**
 * @brief Apriori::printMapSet
 * @param mapSet
 */
void Apriori::printMapSet(map<set<string> ,int> &mapSet)
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
 * @brief Apriori::printsetSet
 * @param setSet
 */
void Apriori::printsetSet(set< set<string>> &setSet)
{
    set< set<string>>::iterator c2It = setSet.begin();
    while(c2It != setSet.end()){
       set<string>::iterator ckSetIt = (*c2It).begin();
       cout <<"[";
        while(ckSetIt != (*c2It).end()){
            cout <<*ckSetIt <<"," ;
            ++ckSetIt;
        }
        cout <<"]"<< endl;
        ++c2It;
    }
}

/**
 * @brief Apriori::printSet
 * @param setS
 */
void Apriori::printSet(set<string> &setS)
{
    set<string>::iterator setIt = setS.begin();
    cout <<"[";
    while(setIt != setS.end()){
        cout <<*setIt <<"," ;
        ++setIt;
    }
    cout <<"]" << endl;
}

/**
 * 将文本数据存入到Map中，产生事务数据库D，即textDataBase
 **/
int Apriori::buildData()
{
    /*打开文本文件*/
    ifstream inFile;
    inFile.open(dataFileName.c_str());
    if(!inFile){
        cerr <<"open " <<dataFileName <<"is failed!" << endl;
        return EXIT_FAILURE;
    }
    /*读取文本行*/
    string textline;
    vector<string> lines_of_text;

    while(getline(inFile,textline)) {
        lines_of_text.push_back(textline);
    }


    /*产生事务数据库*/
    int line_num ;
    for(line_num = 0; line_num != lines_of_text.size(); ++line_num){
        istringstream line(lines_of_text[line_num]);
        string word;
//        cout<<"word is "<<endl;
        while(line >> word){
            textDatabase[line_num].insert(word);
//            cout<<word<<endl;
        }
    }
    textDatabaseCount = textDatabase.size();
    cout <<"textDatabaseCount: " << textDatabaseCount <<" " << line_num<< endl;
    return EXIT_SUCCESS;
}

/**
 * 得到事物项集D中每项的重复度，即每项在事物项集中出现的频率
 **/
map< set<string>, int> Apriori::getTextDatabaseFre()
{
    map< set<string>, int> textDatabaseFre;
    map<long ,set<string>>::iterator textDataIt = textDatabase.begin();
    while(textDataIt != textDatabase.end()){
        pair<map< set<string>, int>::iterator, bool> ret =
            textDatabaseFre.insert(make_pair(textDataIt->second,1));
        if(!ret.second)
            ++ret.first->second;
        ++textDataIt;
    }
    return textDatabaseFre;
}

/**
 * 得到事物项集D中每项的支持度,即统计数据集每一项作为子集出现的频率
 * 注:set<string>中的string是按从小到大的顺序排列的
 * map< set<string>, int >
 **/
map< set<string>, int> Apriori::getTextDatabaseSurpport()
{
    map< set<string>, int> textDatabaseSurpport;
    map< set<string>, int> textDatabaseFre = getTextDatabaseFre();
    //cout <<"原始数据集D各项的频度" << endl;
    //apriori.printMapSet(textDatabaseFre);
    map<long ,set<string>>::iterator textDataIt1 = textDatabase.begin();
    while(textDataIt1 != textDatabase.end()){
        textDatabaseSurpport.insert(make_pair(textDataIt1->second,0));
        map<long ,set<string>>::iterator textDataIt2 = textDatabase.begin();
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


/**
 * 获取候选1项集
 **/
map<string, int> Apriori::getCand1ItemSet()
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
 * 获取频繁1项集
 **/
map< set<string>, int > Apriori::findFreq1ItemSet()
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
 * 根据频繁k-1项集键集获取频繁k项集(K > 1)
 **/
map< set<string>, int > Apriori::getFreqKItemSet(int k, set< set<string>> freqMItemSet)
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
 * 取交集
 */
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

/**
 * 返回set1中去除了set2的数据集
 **/
void Apriori::removeAll(set<string> &set1, set<string> &set2)
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

/**
 * 根据频繁(k-1)项集获取候选k项集
 * m = k-1
 * freqMItemSet:频繁k-1项集
 **/
set< set<string>> Apriori::aprioriK_1Gen(int m, set< set<string>> &freqMItemSet)
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
 * 使用先验知识，剪枝。删除候选k项集中存在k-1项的子集
 **/
bool Apriori::has_infrequent_subset(set<string> &candKItemSet, set<set<string>> &freqMItemSet)
{
    int occurs = 0;

    if(freqMItemSet.count(candKItemSet))
        return true;

    return false;
}

/**
 * 获取mapSet的键值，存放于set中
 **/
set<set<string>> Apriori::keySet(map<set<string>, int > &mapSet)
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
