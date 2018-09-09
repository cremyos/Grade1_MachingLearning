#coding:utf-8
#设置编码模式为utf-8
#+++++++++++++++++++++++++++++++++++++++++++++++++++
#Name: MyPLA_Pocket.py
#Author: 吕跃鹏(Lniper)
#Date: 2017-11-22
#Description: Preception Learning Algorithm Pocket
#+++++++++++++++++++++++++++++++++++++++++++++++++++
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
    group = array([[-1, 0, 1], [0, 1, 1], [0, -1, -1], [1, 0, -1]])
#    group = [
#        [10, 300, -1],
#        [15, 377, -1],
#        [50, 137, 1],
#        [65, 92 , 1],
#        [45, 528, -1],
#        [61, 542, 1],
#        [26, 394, -1],
#        [37, 703, -1],
#        [39, 244, 1],
#        [41, 398, 1],
#        [53, 495, 1],
#        [32, 119, 1],
#        [24, 577, -1],
#        [56, 412, 1]
#    ]
#    labels = [1, 1, -1, -1]
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
#说明： pocket感知机核心算法，简单测试，通过《模式识别》中的例题
#      去判断自己的算法对不对，i32iterCount 迭代次数
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def PreceptionLA_pocket(TrainingData, i32iterCount):
    W = mat([0, 0, 0]).transpose()
    k = 0
    # 正确向量个数
    RightVector = 0
    # 错误向量个数
    WrongVector = 0

    # 获取到的迭代次数为i32iterCount，下的最终向量
    WUpdate = W
    # 当前所有线性分类器的结果中，正确样本点的最大个数
    VectorCount = 0
    # 错误的样本个数
    WrongVectorCount = 0

    while True:

        if k >= i32iterCount:
            break

     	status, x, y = RightPoint(TrainingData, W)
        if status == 'Right':
            #分类正确的情况，直接返回正确向量
            return WUpdate, VectorCount, WrongVectorCount
        else:
            # 分类不正确的情况进行比较，判断当前感知机存在的正确向量的个数是否是最多的。
            # 如果不是，就更新存入向量；否则，不用管。然后修改继续判断
            # 修改感知机的权向量
            W = W + y * x
            RightVector, WrongVector = RightPointCount(TrainingData, W)
            if VectorCount <= RightVector :
                WUpdate = W
                VectorCount = RightVector
                WrongVectorCount = WrongVector
        k += 1
            
    return WUpdate, VectorCount, WrongVectorCount


#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#说明： pocket感知机核心算法，根据数据是否线性可分，
#      不断的改变分类器函数, i32iterCount 迭代次数
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def PreceptionLAUCI_pocket(TrainingData, i32iterCount):
    W = mat([0, 0, 0, 0]).transpose()
    k = 0
    b = random.uniform(0, 10)
    rate = random.uniform(0, 1)
    # 正确向量个数
    RightVector = 0
    # 错误向量个数
    WrongVector = 0
    
    # 获取到的迭代次数为i32iterCount，下的最终向量
    WUpdate = W

    bUpdata = b
    # 当前所有线性分类器的结果中，正确样本点的最大个数
    VectorCount = 0
    # 错误的个数
    VectorCountWrong = 0

    while True:
        if k >= i32iterCount:
            break

        #判断当前的权向量是否对合理
 	status, x, y = RightPointUCI(TrainingData, W, b)
        if status == 'Right':
            #分类正确的情况，直接返回正确向量
            return WUpdate, bUpdate, VectorCount, VectorCountWrong
        else:
            # 分类不正确的情况进行比较，判断当前感知机存在的正确向量的个数是否是最多的。
            # 如果不是，就更新存入向量；否则，不用管。然后修改继续判断
            # 修改感知机的权向量
            W = W + rate * y * x
            b = b + rate * y
            
            RightVector, WrongVector = RightPointCountUCI(TrainingData, W, b)
            if VectorCount <= RightVector :
                WUpdate = W
                bUpdate = b
                VectorCount = RightVector
                VectorCountWrong = WrongVector
        k += 1
            
    return WUpdate, bUpdate, VectorCount, VectorCountWrong
            
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
#      属于哪个样本空间中。UCI数据集
#+++++++++++++++++++++++++++++++++++++++++

def CheckMatrixSign2(m, b):
    x = m.tolist()[0][0] + b
    if x > 0:
        return 1
    elif x < 0:
        return -1

    
#+++++++++++++++++++++++++++++++++++++++
#说明：统计样本中，正确样本的个数
#+++++++++++++++++++++++++++++++++++++++
def RightPointCount(TrainingData, W):
    status = 'Right'
    # 正确向量个数
    i32RightVector = 0
    # 错误向量个数
    i32WrongVector = 0
    for (x, y) in TrainingData:
        # 依据当前的线性分类器判断出的样本类别是否是样本真正的类别，
        # 如果不是，则返回错误，和当前的样本点坐标
#        print "++++++++++++++++++++++++++++++++++++++++"
#        print "W = ", W
#        print "x = ", x
#        print "y = ", y
#        print "W.transpose() * x = ", W.transpose() * x
#        print "++++++++++++++++++++++++++++++++++++++++"
        if CheckMatrixSign(W.transpose() * x) <> sign(y):
            status = 'Wrong'
            i32WrongVector += 1
        else:
            i32RightVector += 1

    return i32RightVector, i32WrongVector

#+++++++++++++++++++++++++++++++++++++++
#说明：统计样本中，正确样本的个数，
#      UCI数据集
#+++++++++++++++++++++++++++++++++++++++
def RightPointCountUCI(TrainingData, W, b):
    status = 'Right'
    # 正确向量个数
    i32RightVector = 0
    # 错误向量个数
    i32WrongVector = 0
    for (x, y) in TrainingData:
        # 依据当前的线性分类器判断出的样本类别是否是样本真正的类别，
        # 如果不是，则返回错误，和当前的样本点坐标
#        print "++++++++++++++++++++++++++++++++++++++++"
#        print "W = ", W
#        print "x = ", x
#        print "y = ", y
#        print "W.transpose() * x = ", W.transpose() * x
#        print "++++++++++++++++++++++++++++++++++++++++"
        if CheckMatrixSign2(W.transpose() * x, b) <> sign(y):
            status = 'Wrong'
            i32WrongVector += 1
        else:
            i32RightVector += 1

    return i32RightVector, i32WrongVector


#++++++++++++++++++++++++++++++++++++++++++
# 说明：判断当前的分类器是否满足所有的样本
#++++++++++++++++++++++++++++++++++++++++++
def RightPoint(TrainingData, W):
    status = 'Right'
    for (x, y) in TrainingData:
        # 依据当前的线性分类器判断出的样本类别是否是样本真正的类别，
        # 如果不是，则返回错误，和当前的样本点坐标
        if CheckMatrixSign(W.transpose() * x) <> sign(y):
            status = 'Wrong'
            return status, x, y

    return status, None, None

#++++++++++++++++++++++++++++++++++++++++++
# 说明：判断当前的分类器是否满足所有的样本，
#       UCI数据集
#++++++++++++++++++++++++++++++++++++++++++
def RightPointUCI(TrainingData, W, b):
    status = 'Right'
    for (x, y) in TrainingData:
        # 依据当前的线性分类器判断出的样本类别是否是样本真正的类别，
        # 如果不是，则返回错误，和当前的样本点坐标
        if CheckMatrixSign2(W.transpose() * x, b) <> sign(y):
            status = 'Wrong'
            return status, x, y

    return status, None, None



#入口函数，在命令行中输入python XX.py即可执行
if __name__ == "__main__":

    BFData = createDataRandom()
    TrainingData = processData(BFData)
    wRandom, RCount, WCount = PreceptionLA_pocket(TrainingData, 1)
    print "wRandom", wRandom
    print "RCount = ", RCount
    print "WCount = ", WCount

    
    Iris_setosa, Iris_versicolor, Iris_virginica = ReadDataFromUCI('Iris.data')
    set_ver, set_vir, ver_vir = processDataUCI(Iris_setosa, Iris_versicolor, Iris_virginica)
    W12, b12, R12Count, W12Count = PreceptionLAUCI_pocket(set_ver, 1)
    W13, b13, R13Count, W13Count = PreceptionLAUCI_pocket(set_vir, 1)
    W23, b23, R23Count, W23Count = PreceptionLAUCI_pocket(ver_vir, 1)
    print "W12 is ", W12
    print "b12 = ", b12
    print "R12Count = ", R12Count
    print "W12Count = ", W12Count
    print "W13 is ", W13
    print "b13 = ", b13
    print "R13Count = ", R13Count
    print "W13Count = ", W13Count
    print "W23 is ", W23
    print "b23 = ", b23
    print "R23Count = ", R23Count
    print "W23Count = ", W23Count
