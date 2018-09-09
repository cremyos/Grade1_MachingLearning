#ifndef CSHOWDATA_H
#define CSHOWDATA_H

#include <QWidget>
#include <QPixmap>
#include "readfiledata.h"

namespace Ui {
class CShowData;
}

class CShowData : public QWidget
{
    Q_OBJECT

public:
    explicit CShowData(QWidget *parent = 0);
    ~CShowData();

    int ShowTheRes();

private:
    Ui::CShowData *ui;

    ReadFileData rfd;
    QMap<int, QVector<ST_CLASS_DATA_SHOWTYPE>> MapVecSTCDS;
    QPixmap imageEnable;
    QPixmap imageDisable;

};

#endif // CSHOWDATA_H
