#include <QCoreApplication>
#include <iostream>
#include <QSet>
#include <QVector>
#include <QMap>
#include <QDebug>

using namespace std;

typedef QMap<QSet<QString>, int> map_s;

#define ITEMCOUNT_MAX   4


map_s Lk;       ///频繁项集Lk
QVector<QSet<QString>>  data; //原始数据
QVector<QSet<QString>>  L;    //频繁项集合
QVector<QString>        data2; //分割后的原始数据

int n = 0;                  /// 数据项个数

int ItemCount[ITEMCOUNT_MAX] = {3, 4, 3, 5};

int minSup = 2;             /// 最小阈值
int minConf = 0.2;

QSet<QString> ItemSet;

QString QSItem[ITEMCOUNT_MAX] = {"I1 I2 I6", "I1 I2 I3 I5",
                                "I2 I3 I7", "I1 I3 I5 I6 I2"};

void Delete(map_s &Ck)
{
    for(  map_s::iterator l_it = Ck.begin(); l_it != Ck.end(); l_it++) {
        if( l_it.value() < minSup) {
            Ck.erase(l_it);
        }
    }
}

int compset(QSet<QString> s1, QSet<QString> s2)
{
    int flag=0;
    //判断集合s1是不是s2的子集
    for(QSet<QString>::iterator it=s1.begin(); it!=s1.end();it++ ) {
        //s1有元素不在s2中
        if( s2.find(*it)==s2.end()) {
            flag=10;
            break;
        }
    }
    for(QSet<QString>::iterator it=s2.begin(); it!=s2.end();it++ ) {
        //s2有元素不在s1中
        if( s1.find(*it)==s1.end() ) {
            flag+=1;
            break;
        }
    }
    /**
     * 当flag==0,s1元素全部在s2中，s2元素也全部在s1中，s1==s2
     * 当flag==10,s1有元素不在s2中，s2所有元素都在s1中，s1包含了s2
     * 当flag==1,s1所有元素在s2中，s2有元素不在s1中，s2包含了s1
     * 当flag==11,s1 s2集合互不包含
     **/
    return flag;
}

map_s apriori_gen(map_s &Ck,int k)
{
   /**
    * 生成子集
    **/
    map_s Ck_temp;
    QSet<QString> s_temp;

    for(map_s::iterator l_it1 = Ck.begin(); l_it1!=Ck.end(); l_it1++) {

       for(map_s::iterator l_it2 = Ck.begin(); l_it2 != Ck.end(); l_it2++) {

        /**
        * 如果两个set一样，则说明是同一个KEY，跳过
        **/
        if(l_it1.key() == l_it2.key()) {
            continue;
        }

        /**
         * 否则开始组装,遍历整个Ck
         **/
        QSet<QString>::iterator s_it;
        QSet<QString> TempSet = l_it2.key();
        for(s_it = TempSet.begin(); s_it != TempSet.end(); s_it++) {

            /**
            * 如果该值在l_it1 set里面可以找到，不能组装
            **/
            if( l_it1.key().find(*s_it)!=l_it1.key().end()) {
                continue;
            }

            /**
            * 否则进行组装,先把l_it1的set复制进去
            **/
            s_temp = l_it1.key();

            /**
            * 再把l_it2的值放进去
            **/
            s_temp.insert(*s_it);

            /**
             * 判断该组装的set是否已在生成集合中，如果之前已生成，则不需要往下运算
             **/
                if(Ck_temp.find(s_temp)!=Ck_temp.end()) {
                    continue;
                } else {
                    /**
                     * 否则放到生成的子集中
                     **/
                    Ck_temp.insert(s_temp, 0);
                }
            }
        }
    }

   /**
    * 对于k=2的情况，需要扫描原始数据得出计数值
    **/
   if(2 == k) {
     for( map_s::iterator l_it=Ck_temp.begin();l_it!=Ck_temp.end();l_it++ )
      for(int i=0; i < data.size(); i++)
       //l_it集合被data[i]完整包含，则计数值+1
       if( (10 == compset(data[i], l_it.key())) || (0 == compset(data[i], l_it.key()))  )
                Ck_temp[l_it.key()]++;

       //扫描完之后排除 非频繁项
     for( map_s::iterator l_it=Ck_temp.begin();l_it!=Ck_temp.end();l_it++ )
         if( Ck_temp[l_it.key()] < minSup )
             Ck_temp.erase(l_it);
   }
   //如果是大于2的情况，扫描k-1的频繁项子集
   if( 2 < k ) {
      //每次都循环获取每个Ck的k-1子集元素
      //如{I1,I2,I3}C3的子集是{I1,I2} {I2,I3} {I3,I4}
      //如果Ck的子集不在k-1的频繁项子集中，则去掉该Ck项
      for( map_s::iterator l_it=Ck_temp.begin();l_it!=Ck_temp.end();l_it++ )
      {
         int flag;
         QSet<QString> tempSet;
         tempSet = l_it.key();
         for(QSet<QString>::iterator s_it = tempSet.begin(); s_it != tempSet.end(); s_it++ )
         {
           //开始求子集
           //首先把当前Ck项的集合保存
           s_temp=l_it.key();
           //去掉一个元素，即是它的k-1子集
           s_temp.erase(s_it);
           //遍历频繁项集合L，看看是不是在频繁集中
           flag=1;
           for( int i=0;i<L.size();i++  )
           {
             //如果K-1子集在频繁项集中存在，则保留
             if( 0 == compset(s_temp,L[i]) )
             {
                  flag=0;
                  break;
             }
           }
           //如果找到了哪怕一个k-1子集项不在频繁项集中，直接退出
           if( flag ) break;
         }
         //只有所有的k-1子集在频繁项集中，才保留该Ck项
         if( flag ) Ck_temp.erase(l_it);
      }
   }

    cout<<"由L"<<k-1<<"产生的候选集C"<<k<<"   "<<"cout数(k=2以上不做计数)"<<endl;
    for( map_s::iterator l_it=Ck_temp.begin();l_it!=Ck_temp.end();l_it++ )
    {
       QSet<QString> tempSet = l_it.key();
        for(QSet<QString>::iterator s_it = tempSet.begin(); s_it != tempSet.end(); s_it++ )
             qDebug()<<*s_it + "  ";
        qDebug()<<l_it.value();
    }
    return Ck_temp;
}

map_s MapCk;       ///候选集Ck

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /**
     * 生成原始数据集
     */
    n = 2;
    for(int i=0;i<n;i++) {
        ItemSet.clear();
        int i32TempIndexCount = ItemCount[i];
        for(int j=0; j<i32TempIndexCount; j++) {
            ItemSet.insert(QSItem[j]);
            data2.push_back(QSItem[j]);
        }
        data.push_back(ItemSet);
    }

    /**
     * 扫描数据集D，生成C1，对于每个候选集里面的元素
     **/
    for(int j = 0; j < data2.size(); j++ ) {
        int flag=1;
        /**
         * 如果C1中存在该元素，计数值加1
         **/
        for( map_s::iterator l_it = MapCk.begin(); l_it != MapCk.end(); l_it++) {
            if( l_it.key().find(data2[j]) != l_it.key().end()) {
                //QString QStrTemp = data2[j];
                QSet<QString> TempSetStr = l_it.key();
                //QSet<QString>::iterator TempSetIter = TempSetStr.find(QStrTemp);

                MapCk[TempSetStr]++;

                flag = 0;
                break;
            }
        }

        /**
         * 不存在，插入到C1集合中
         **/
        if(flag) {
            ItemSet.clear();
            ItemSet.insert(data2[j]);
            MapCk.insert(ItemSet, 1);
        }
    }

    //去掉支持度不足的
    for(map_s::iterator l_it = MapCk.begin(); l_it != MapCk.end(); l_it++) {
        if( l_it.value() < minSup)
            MapCk.erase(l_it);
    }

  qDebug()<<"C1候选集:";
  qDebug()<<"项集     支持度计数";

    for( map_s::iterator l_it = MapCk.begin(); l_it != MapCk.end(); l_it++ ) {
        QSet<QString> TempSet =  l_it.key();
        for( QSet<QString>::iterator s_it = TempSet.begin(); s_it != TempSet.end(); s_it++)
            qDebug()<<*s_it;
            qDebug()<<" " + QString::number(l_it.value());

    }

    int f_count = 2;
    while(f_count) {
        //将Ck内的k-1频繁集全部保存到L集中
        for( map_s::iterator l_it=MapCk.begin();l_it!=MapCk.end();l_it++ ) {
            L.push_back(l_it.key());
        }
        //获取Ck集，已清除掉小于支持度的候选集
        MapCk = apriori_gen(MapCk, f_count);

        if(MapCk.empty()) {
            break;
        } else {
            f_count++;
        }
    }

    qDebug()<<"最终的频繁集集合";
    for( int i=0; i<L.size(); i++ ) {

        QSet<QString> TempSet = L[i];
        for( QSet<QString>::iterator s_it = TempSet.begin(); s_it!=TempSet.end(); s_it++) {
            qDebug()<<*s_it;
        }
    }

    return a.exec();
}
