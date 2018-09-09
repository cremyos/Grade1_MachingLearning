#include <QCoreApplication>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv/cxcore.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
