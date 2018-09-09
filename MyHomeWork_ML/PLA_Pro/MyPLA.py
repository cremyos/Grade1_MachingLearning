#coding:utf-8
#设置编码模式为utf-8
#+++++++++++++++++++++++++++++++++++++++++++++
#Name: MyPLA.py
#Author: 吕跃鹏(Lniper)
#Date: 2017-11-21
#Description: Preception Learning Algorithm
#+++++++++++++++++++++++++++++++++++++++++++++
from numpy import *
import operator
import random
import matplotlib.pyplot as plt
import csv

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#说明：随意的一组数据，array()是来自于《模式识别》p68 例3.2中的数据，
#      后面是来自网上随便找的测试数据
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def createDataRandom():
#    group = array([[-1, 0, 1], [0, 1, 1], [0, -1, -1], [1, 0, -1]])
    group = [
        [10, 300, -1],
        [15, 377, -1],
        [50, 137, 1],
        [65, 92 , 1],
        [45, 528, -1],
        [61, 542, 1],
        [26, 394, -1],
        [37, 703, -1],
        [39, 244, 1],
        [41, 398, 1],
        [53, 495, 1],
        [32, 119, 1],
        [24, 577, -1],
        [56, 412, 1]
    ]
    labels = [1, 1, -1, -1]
    return group# , labels

#++++++++++++++++++++++++++
#从UCI数据集中读取数据,
# 数据集：Iris中的数据
#++++++++++++++++++++++++++
def ReadDataFromUCI(FileName):
   
    with open(FileName, "rb") as csvfile:
        Iris_setosa = []
        Iris_versicolor = []
        Iris_virginica = []

        lines = csv.reader(csvfile)
        dataset = list(lines)

        Countnum1 = 0
        Countnum2 = 0
        Countnum3 = 0        
        
        for x in range(1, (len(dataset) - 1)):
                if dataset[x][4] == 'Iris-setosa':
                    Countnum1 += 1
                    for y in range(4):
                        dataset[x][y] = float(dataset[x][y])
                    Iris_setosa.append(dataset[x])
                elif dataset[x][4] == 'Iris-versicolor':
                    Countnum2 += 1
                    for y in range(4):
                        dataset[x][y] = float(dataset[x][y])
                    Iris_versicolor.append(dataset[x])
                elif dataset[x][4] == 'Iris-virginica':
                    Countnum3 += 1
                    for y in range(4):
                        dataset[x][y] = float(dataset[x][y])
                    Iris_virginica.append(dataset[x])
    return Iris_setosa, Iris_versicolor, Iris_virginica


        

#+++++++++++++++++++++++
# 扩展矩阵，返回矩阵的转置
#+++++++++++++++++++++++
def ExtendMatrix(XVector):
    return mat(XVector).transpose()

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# 说明：处理数据，将训练前的数据，改变样本的style，
#       变成(x1, x2, TypeNum)，TypeNum表示该样本点属于哪个类
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def processData(BFTrainingData):
    AfterTrainingData = [(ExtendMatrix([sample[0], sample[1], 1]), sample[2]) for sample in BFTrainingData]
    #返回处理后的矩阵
    return AfterTrainingData

#+++++++++++++++++++++++++++++++++++++++++
# 说明：三组数据
#+++++++++++++++++++++++++++++++++++++++++
def processDataUCI(BFTrainingData1, BFTrainingData2, BFTrainingData3):
    # 1、2组分为两类
    Seto_sico_TrainingData = [(ExtendMatrix([sample[0], sample[1], sample[2], sample[3]]), 1) for sample in BFTrainingData1]
    Seto_sico_TrainingData = [(ExtendMatrix([sample[0], sample[1], sample[2], sample[3]]), -1) for sample in BFTrainingData2]

    # 1、3组分为两类
    Seto_Vir_TrainingData = [(ExtendMatrix([sample[0], sample[1], sample[2], sample[3]]), 1) for sample in BFTrainingData1]
    Seto_Vir_TrainingData = [(ExtendMatrix([sample[0], sample[1], sample[2], sample[3]]), -1) for sample in BFTrainingData2]

    # 2、 3组分两类
    Ver_Vir_TrainingData = [(ExtendMatrix([sample[0], sample[1], sample[2], sample[3]]), 1) for sample in BFTrainingData2]
    Ver_Vir_TrainingData = [(ExtendMatrix([sample[0], sample[1], sample[2], sample[3]]), -1) for sample in BFTrainingData3]

    #返回处理后的矩阵
    return  Seto_sico_TrainingData, Seto_Vir_TrainingData, Ver_Vir_TrainingData



#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#说明： 感知机核心算法，根据数据是否线性可分，不断的改变分类器函数
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def PreceptionLA(TrainingData):
    W = mat([0, 0, 0]).transpose()
    k = 0

    while True:
        k += 1

        #判断当前的权向量是否对合理
        status, x, y = RightPoint(TrainingData, W)
        if status == 'Right':
            #分类正确
            print "Random k success", k
            return W
        else :
            # 修改感知机的权向量
            W = W + y * x

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#说明： 感知机核心算法，根据数据是否线性可分，不断的改变分类器函数
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def PreceptionLAUCI(TrainingData):
    W = mat([0, 0, 0, 0]).transpose()
    b = random.uniform(0, 10)
    k = 0
    rate = random.uniform(0, 1)


    while True:
        k += 1

        #判断当前的权向量是否对合理
 	status, x, y = RightPointUCI(TrainingData, W, b)
        if status == 'Right':
            #分类正确
            print "UCI K success ", k
            return W, b
        else :
            # 修改感知机的权向量
            W = W + rate * y * x
            b = b + rate * y
            

            
#+++++++++++++++++++++++++++++++++++++++++
#说明：检测当前的样本对于当前的线性分类器，
#      属于哪个样本空间中。
#+++++++++++++++++++++++++++++++++++++++++

def CheckMatrixSign(m):
    x = m.tolist()[0][0]
    if x > 0:
        return 1
    elif x < 0:
        return -1

#+++++++++++++++++++++++++++++++++++++++++
#说明：检测当前的样本对于当前的线性分类器，
#      属于哪个样本空间中。针对于
#	sign(w * x + b)
#+++++++++++++++++++++++++++++++++++++++++

def CheckMatrixSign2(m, b):
    x = m.tolist()[0][0] + b
    if x > 0:
        return 1
    elif x < 0:
        return -1

    
#++++++++++++++++++++++++++++++++++++++++
#说明：判断当前的分类器是否满足所有的样本，
#      针对UCI版
#++++++++++++++++++++++++++++++++++++++++
def RightPointUCI(TrainingData, W, b):
    status = 'Right'
    for (x, y) in TrainingData:
        # 依据当前的线性分类器判断出的样本类别是否是样本真正的类别，
        # 如果不是，则返回错误，和当前的样本点坐标
        if CheckMatrixSign2(W.transpose() * x, b) <> sign(y):
            status = 'Wrong'
            return status, x, y

    return status, None, None

#++++++++++++++++++++++++++++++++++++++++
#说明：判断当前的分类器是否满足所有的样本
#      随机版。
#++++++++++++++++++++++++++++++++++++++++
def RightPoint(TrainingData, W):
    status = 'Right'
    for (x, y) in TrainingData:
        # 依据当前的线性分类器判断出的样本类别是否是样本真正的类别，
        # 如果不是，则返回错误，和当前的样本点坐标
        if CheckMatrixSign(W.transpose() * x) <> sign(y):
            status = 'Wrong'
            return status, x, y

    return status, None, None



#入口函数，在命令行中输入python XX.py即可执行
if __name__ == "__main__":

    BFData = createDataRandom()
    TrainingData = processData(BFData)
    wRandom = PreceptionLA(TrainingData)
    print wRandom

    
    Iris_setosa, Iris_versicolor, Iris_virginica = ReadDataFromUCI('Iris.data')
    set_ver, set_vir, ver_vir = processDataUCI(Iris_setosa, Iris_versicolor, Iris_virginica)
    W12, b12 = PreceptionLAUCI(set_ver)
    W13, b13 = PreceptionLAUCI(set_vir)
    W23, b23 = PreceptionLAUCI(ver_vir)
    print "W12 is ", W12
    print "b12 = ", b12
    print "W13 is ", W13
    print "b13 = ", b13
    print "W23 is ", W23
    print "b23 = ", b23


