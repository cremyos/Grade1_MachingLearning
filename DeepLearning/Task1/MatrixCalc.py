# Name: MatrixCalc.py
# Date: 2018.04.27
# Author: Lniper
# Aim:  输入：A,B,C
#       其中A是3x4 float32 随机矩阵，B是4x3 float32 随机矩阵， C是3x3 float32 随机矩阵
#       且A,B,C 各不相等
#       用tensorflow实现：AxB+C 并打印出A, B, C及（AxB+C）的数值结果

import tensorflow as tf
from numpy import *

# X = tf.placeholder(dtype=tf.float32, shape=[144, 10], name='X')
# dtype：数据类型，必填，默认为value的数据类型，传入参数为tensorflow下的枚举值（float32，float64.......）
# shape：数据形状，选填，不填则随传入数据的形状自行变动，可以在多次调用中传入不同形状的数据
# name：常量名，选填，默认值不重复，根据创建顺序为（Placeholder，Placeholder_1，Placeholder_2.......）
#A = tf.placeholder(tf.float32, shape = [3, 4])
#B = tf.placeholder(tf.float32, shape = [4, 3])

# 生成随机矩阵
TempMatA = random.rand(3, 4)
print("TempMatA:")
print(TempMatA)
TempMatB = random.rand(4, 3)
print("TempMatB:")
print(TempMatB)
TempMatC = random.rand(3, 3)
print("TempMatC:")
print(TempMatC)

# 画图，画出Y=A * B + C
A = tf.placeholder(tf.float32, shape=[3, 4])
B = tf.placeholder(tf.float32, shape=[4, 3])
C = tf.placeholder(tf.float32, shape=[3, 3])

# 结果1 表示A * B的结果
Res1 = tf.matmul(A, B, name = 'MatMul')
# 结果2 表示A * B + C 的结果
ResFinal = tf.add(Res1, C, name = 'matadd')

sess = tf.Session()

print("Add result")
print(sess.run(Res1, feed_dict = {A: TempMatA, B: TempMatB}))
print("Final result")
print(sess.run(ResFinal, feed_dict={A: TempMatA, B: TempMatB, C: TempMatC}))

sess.close()
