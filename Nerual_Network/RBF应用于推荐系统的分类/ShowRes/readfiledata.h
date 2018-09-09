#ifndef READFILEDATA_H
#define READFILEDATA_H

#include <QDebug>
#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <QVector>
#include <QMap>

#define MAXLINE 15000
//#define MAXLINE 100

/**
 * 类-数据-显示方式
 **/
typedef struct {
    double dData;
    int i32ShowType;
}ST_CLASS_DATA_SHOWTYPE;


class ReadFileData
{
public:
    QVector<ST_CLASS_DATA_SHOWTYPE> VecSTCDS;                           /// 数据结构体
    QMap<int, QVector<ST_CLASS_DATA_SHOWTYPE>> MapVecSTCDS;             /// 显示类和数据
    ST_CLASS_DATA_SHOWTYPE stCDS;                                       /// 数据结构体

public:
    ReadFileData();
    ~ReadFileData(){};

    int ReadData();

};

#endif // READFILEDATA_H
