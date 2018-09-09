#include "cshowdata.h"
#include "ui_cshowdata.h"

CShowData::CShowData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CShowData)
{
    ui->setupUi(this);

    rfd.ReadData();

    imageEnable.load("Pic/Green.png");
    imageDisable.load("Pic/Red.png");

    ShowTheRes();
}

int CShowData::ShowTheRes()
{
    QMap<int, QVector<ST_CLASS_DATA_SHOWTYPE>>::iterator MapVecSTCDSIter = rfd.MapVecSTCDS.begin();
    for(MapVecSTCDSIter; MapVecSTCDSIter != rfd.MapVecSTCDS.end(); MapVecSTCDSIter++) {
        qDebug()<<"Number = " + QString::number(MapVecSTCDSIter.key());
        ui->label_class->setText(QString::number(MapVecSTCDSIter.key()));
        QVector<ST_CLASS_DATA_SHOWTYPE> VecStCDS = MapVecSTCDSIter.value();
        for(int i32TempIndex = 0; i32TempIndex < VecStCDS.size(); i32TempIndex++) {
            ST_CLASS_DATA_SHOWTYPE stCDS = VecStCDS.at(i32TempIndex);
            switch(stCDS.i32ShowType) {
                /**
                 * 1:LCD数字显示
                 * 2：普通数字显示
                 * 3：使能开关
                 * 4：曲线图
                 **/
                case 0: {
//                    qDebug()<<"case 0: " + QString::number(stCDS.i32ShowType);
//                    qDebug()<<"data: " + QString::number(stCDS.dData);
                    ui->lcdNumber->display(stCDS.dData);
                    break;
                }
                case 1: {
//                    qDebug()<<"case 1: " + QString::number(stCDS.i32ShowType);
                    ui->textBrowser->append(QString::number(stCDS.dData));
                    break;
                }
                case 2: {
//                    qDebug()<<"case 2: " + QString::number(stCDS.i32ShowType);
                    if(stCDS.dData == 1) {
                        /**
                         * 有效
                         **/
                        ui->label_Enable->setPixmap(imageEnable);
                    } else if(stCDS.dData == 0) {
                        /**
                         * 无效
                         **/
                        ui->label_Disable->setPixmap(imageDisable);
                    }
                    break;
                }
                case 3: {
//                    qDebug()<<"case 3: " + QString::number(stCDS.i32ShowType);
                    QString TempShowData = QString::number(stCDS.dData) + "曲线表示";
                    ui->textBrowser_2->append(TempShowData);
                    break;
                }

            }
        }

    }
}

CShowData::~CShowData()
{
    delete ui;
}
