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

    int width = 512, height = 512;
    Mat image = Mat::zeros(height, width, CV_8UC3);  //创建窗口可视化

    // 设置训练数据
    int labels[10] = { 1, -1, 1, 1,-1,1,-1,1,-1,-1 };
    Mat labelsMat(10, 1, CV_32SC1, labels);

    float trainingData[10][2] = { { 501, 150 }, { 255, 10 }, { 501, 255 }, { 10, 501 }, { 25, 80 },
    { 150, 300 }, { 77, 200 } , { 300, 300 } , { 45, 250 } , { 200, 200 } };
    Mat trainingDataMat(10, 2, CV_32FC1, trainingData);

    // 创建分类器并设置参数
    Ptr<SVM> model =SVM::create();
    model->setType(SVM::C_SVC);
    model->setKernel(SVM::LINEAR);  //核函数

    //设置训练数据
    Ptr<TrainData> tData =TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);

    // 训练分类器
    model->train(tData);

    Vec3b green(0, 255, 0), blue(255, 0, 0);
    // Show the decision regions given by the SVM
    for (int i = 0; i < image.rows; ++i)
    for (int j = 0; j < image.cols; ++j)
    {
        Mat sampleMat = (Mat_<float>(1, 2) << j, i);  //生成测试数据
        float response = model->predict(sampleMat);  //进行预测，返回1或-1

        if (response == 1)
            image.at<Vec3b>(i, j) = green;
        else if (response == -1)
            image.at<Vec3b>(i, j) = blue;
    }

    // 显示训练数据
    int thickness = -1;
    int lineType = 8;
    Scalar c1 = Scalar::all(0); //标记为1的显示成黑点
    Scalar c2 = Scalar::all(255); //标记成-1的显示成白点
    //绘图时，先宽后高，对应先列后行
    for (int i = 0; i < labelsMat.rows; i++)
    {
        const float* v = trainingDataMat.ptr<float>(i); //取出每行的头指针
        Point pt = Point((int)v[0], (int)v[1]);
        if (labels[i] == 1)
            circle(image, pt, 5, c1, thickness, lineType);
        else
            circle(image, pt, 5, c2, thickness, lineType);

    }

    imshow("SVM Simple Example", image);
    waitKey(0);




    return a.exec();
}
