# -*- coding: utf-8 -*-
from numpy import *
from cvxopt import *
import csv

# +++++++++++++++++++++++++++++++++++++++++++++++++++++
# Name: MySVM.py
# Author: 吕跃鹏(Lniper)
# Date: 2017-12-02
# Description: 线性SVM算法，采用KKT方法求解SVM
# +++++++++++++++++++++++++++++++++++++++++++++++++++++

# ++++++++++++++++++++++++++
# 从UCI数据集中读取数据,
#  数据集：Iris中的数据
# ++++++++++++++++++++++++++
def ReadDataFromUCI(FileName):
   
    with open(FileName, "rb") as csvfile:
        Iris_setosa_Sepal = []
        Iris_setosa_Petal = []
        Iris_versicolor_Sepal = []
        Iris_versicolor_Petal = []
        Iris_virginica_Sepal = []
        Iris_virginica_Petal = []

        lines = csv.reader(csvfile)
        dataset = list(lines)

        for x in range(0, (len(dataset) - 1)):
                if dataset[x][4] == 'Iris-setosa':
                    for y in range(4):
                        dataset[x][y] = float(dataset[x][y])
                    Iris_setosa_Sepal.append(dataset[x][:2])
                    Iris_setosa_Petal.append(dataset[x][2:4])
                elif dataset[x][4] == 'Iris-versicolor':
                    for y in range(4):
                        dataset[x][y] = float(dataset[x][y])
                    Iris_versicolor_Sepal.append(dataset[x][:2])
                    Iris_versicolor_Petal.append(dataset[x][2:4])
                elif dataset[x][4] == 'Iris-virginica':
                    for y in range(4):
                        dataset[x][y] = float(dataset[x][y])
                    Iris_virginica_Sepal.append(dataset[x][:2])
                    Iris_virginica_Petal.append(dataset[x][2:4])
    return mat(Iris_setosa_Sepal), mat(Iris_setosa_Petal), mat(Iris_versicolor_Sepal), mat(Iris_versicolor_Petal), mat(Iris_virginica_Sepal), mat(Iris_virginica_Petal)

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# 随机选择一个样本
# 输入参数：i，某个样本
#           m，所有样本的数量
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def RandJ(i,m):
    j = i
    #while j == i:
    # 随机选择一个样本的下标，并返回样本下标
    j = int(random.uniform(0, m))
    return j

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# 调整大于H，或小于L的alpha值
# 输入参数：alphasJ，输入样本
#           H，最大值
#           L，最小值
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def AdjSample(alphasJ, H, L):
    if alphasJ > H:
        alphasJ = H
    if L > alphasJ:
        alphasJ = L
    return alphasJ


# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# 简化的SMO算法，SMO算法工作原理，每次循环选择两个alpha进行优化处理。一旦找到一堆合适的alpha，
#     就增大其中一个同时减少另一个。
# 输入参数：dataMatIn，输入数据；
#           classLabels，类型；
#           C，惩罚系数，即如果某个x是属于某一类，但是它偏离了该类，
#              跑到边界上后者其他类的地方去了，C越大表明越不想放弃这个点，边界就会缩小
#           toler，容错率
#           maxIter，最大迭代次数
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def MySMO(DataSet1, DataSet2, C, toler, maxIter):
    # 将数据两个DataSet1和DataSet2 结合
    dataMatIn = vstack((DataSet1, DataSet2))

    numDataSet1 = DataSet1.shape[0]
    numDataSet2 = DataSet2.shape[0]
    classLabels1 = ones(numDataSet1)
    classLabels2 = ones(numDataSet2) * -1
    classLabels  = hstack((classLabels1, classLabels2))

    DataMatrix = mat(dataMatIn)
    LabelMatrix = mat(classLabels).transpose()
    b = 0
    m, n = shape(DataMatrix)
    alphas = mat(zeros((m,1)))
    iter = 0
    while (iter < maxIter):
        alphaPairsChanged = 0
        for i in range(m):
            fXi = float(multiply(alphas, LabelMatrix).T * (DataMatrix*DataMatrix[i,:].T)) + b
            # 误差来自预测和真实结果
            Ei = fXi - float(LabelMatrix[i])
            # 把违背KKT条件的ai作为第一个
            # 满足KKT条件的情况是：
            # yi * f(i) >= 1 and alpha == 0(正确分类)
            # yi * f(i) == 1 and 0 < alpha < C(在边界上的支持向量)
            # yi * f(i) <= 1 and alpha == C(在边界之间)
            if ((LabelMatrix[i]*Ei < -toler) and (alphas[i] < C)) or ((LabelMatrix[i]*Ei > toler) and (alphas[i] > 0)):
                # 如果LabelMatrix[i]*Ei < 0 且 alpha < C 则违反了KKT条件
                # 如果LabelMatrix[i]*Ei > 0 且 alpha > 0 则违反了KKT条件
                j = RandJ(i,m)
                fXj = float(multiply(alphas,LabelMatrix).T*(DataMatrix*DataMatrix[j,:].T)) + b
                Ej = fXj - float(LabelMatrix[j])
                alphaIold = alphas[i].copy(); alphaJold = alphas[j].copy();
                if (LabelMatrix[i] != LabelMatrix[j]):
                    TempMax = alphas[j] - alphas[i]
                    if 0 < TempMax:
                        L = TempMax
                    else:
                        L = 0
                    TempMin = C + alphas[j] - alphas[i]
                    if C < TempMin:
                        H = C
                    else:
                        H = TempMin
                else:
                    TempMax = alphas[j] + alphas[i] - C
                    if 0 < TempMax:
                        L = TempMax
                    else:
                        L = 0
                    TempMin = alphas[j] + alphas[i]
                    if C < TempMin:
                        H = C
                    else:
                        H = TempMin
                if L == H:
                    print "L==H"
                    continue
                eta = 2.0 * DataMatrix[i,:]*DataMatrix[j,:].T - DataMatrix[i,:]*DataMatrix[i,:].T - DataMatrix[j,:]*DataMatrix[j,:].T
                if eta >= 0:
                    print "eta>=0"
                    continue
                alphas[j] -= LabelMatrix[j]*(Ei - Ej)/eta
                alphas[j] = AdjSample(alphas[j], H, L)
                if abs(alphas[j] - alphaJold) < 0.00001 :
                    print "j not moving enough"
                    continue
                alphas[i] += LabelMatrix[j]*LabelMatrix[i]*(alphaJold - alphas[j])
                b1 = b - Ei- LabelMatrix[i]*(alphas[i]-alphaIold)*DataMatrix[i,:]*DataMatrix[i,:].T - LabelMatrix[j]*(alphas[j]-alphaJold)*DataMatrix[i,:]*DataMatrix[j,:].T
                b2 = b - Ej- LabelMatrix[i]*(alphas[i]-alphaIold)*DataMatrix[i,:]*DataMatrix[j,:].T - LabelMatrix[j]*(alphas[j]-alphaJold)*DataMatrix[j,:]*DataMatrix[j,:].T
                if (0 < alphas[i]) and (C > alphas[i]):
                    b = b1
                elif (0 < alphas[j]) and (C > alphas[j]):
                    b = b2
                else:
                    b = (b1 + b2)/2.0
                alphaPairsChanged += 1
                print "iter: %d i:%d, pairs changed %d" % (iter, i, alphaPairsChanged)
        if alphaPairsChanged == 0:
            iter += 1
        else:
            iter = 0
        print "iteration number: %d" % iter
    return b, alphas


# 入口函数，在命令行中输入python XX.py即可执行
if __name__ == "__main__":
    Iris_setosa_Sepal, Iris_setosa_Petal, Iris_versicolor_Sepal, Iris_versicolor_Petal, Iris_virginica_Sepal, Iris_virginica_Petal = ReadDataFromUCI('Iris.data')
    print "Iris_setosa_Sepal ", Iris_setosa_Sepal.shape[0]
    b, alphas = MySMO(Iris_setosa_Sepal, Iris_versicolor_Sepal, 0.6, 0.001, 40)
    print "b ", b
    print "alphas "
    for i in range(alphas.shape[0]):
        if alphas[i] > 0:
            print alphas[i]


