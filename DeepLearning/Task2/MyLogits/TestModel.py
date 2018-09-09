# -- coding: UTF-8 --
# Name: TestModel.py
# Date: 2018.05.07
# Author: Lniper
# Aim: 采用逻辑斯蒂回归（如课程中例子）建立二维数据二分类模型。
#       两类数据服从二维高斯分布：
#       N(μ1, μ2;σ1, σ2, ρ)
#       类A：（x,y)~N(3, 6, 1, 1, 0)
#       类B：（x,y)~N(6, 3, 1, 1, 0)
#       1）分别为类A，类B各随机生成100个样本作为训练数据train_data，30个样本作为验证数据validation_data，30个样本作为测试数据test_data。
#       3）建立逻辑斯蒂回归模型并用GradientDescentOptimizer优化器（参数默认0.01）进行优化学习。
#       4）采用参数复用方式构建学习、验证两个计算路径。学习过程中，进行500次迭代，每次按顺序取train_data中的20个数据进行训练。每100次迭代用30个验证样本validation_data进行验证。训练过程中，打印训练的损失函数值及模型在验证集上的精度。
#       5) 用ckpt方式保存模型，每100次迭代保存一次。
#       6）在另一个py文件中写测试流程，用import_meta_graph导入计算图，get_tensor_by_name得到输入placeholder，以及ACC的tensor，建立测试流程，并用test_data对模型进行测试，输出ACC。
#       7)  将分类结果绘图（例如用matplotlib），（如课程中例子）A、B类测试数据(分别在图中用‘+’以及‘o’表示)， 分类模型以分割线表示。
#
#       要求：
#       1.测试train_data和验证validation_data用两个不同的placeholder_train，placeholder_validation。采用参数复用的方式构建训练和验证两个计算路径
#         （不能用同一个计算路径喂不同的数据！！！因为train 和validation的batch size 不一样（20，30），所以placeholder的size不一样）。
#       2.提交代码和运行截图。
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf

# from . import MyLogistic_Regression
# from . import MyLogistic_Regression as MyLogistic
from MyLogits import MyLogistic_Regression as MyLogistic

TestX_data = tf.placeholder(shape=(60, 2), dtype=tf.float32, name='TestX_data')
TestY_Target = tf.placeholder(shape=(60, 1), dtype=tf.float32, name='TestY_Target')

OpenModel = tf.train.import_meta_graph('MyLogistic\LogitsModel-100.meta')

# 参数复用，载入模型，测试测试数据
with tf.Session() as sess:
	TempTest_step, TemplossTe, TempACCTest = MyLogistic.Train_validationFunc(3, TestX_data, TestY_Target)

	OpenModel.restore(sess, "MyLogistic\LogitsModel-100")
	Weight = tf.get_default_graph().get_tensor_by_name('Logistic_Regression/Modelweight:0')
	bias = tf.get_default_graph().get_tensor_by_name('Logistic_Regression/Modelbias:0')
	ACC = sess.run(TempACCTest, feed_dict={TestX_data: MyLogistic.test_data[:, 0:2], TestY_Target: MyLogistic.test_dataLabels[:]})
	print('Test Data ACC = ', ACC)

	# 计算分类线
	# XShowData = tf.placeholder(shape=(100, 1), dtype=tf.float32, name='XShowData')
	X = np.ones((100, 1)).astype(np.float32)
	TempData = np.arange(0.0, 10.0, 0.1)
	X[0:100, 0] = TempData
	Y = tf.matmul(Weight, np.transpose(X)) + bias * np.ones((2, 100))
	TempY = sess.run(Y)

	# 绘图
	plt.plot(TempData, TempY[1, 0:100], label = "Logistic")
	plt.plot(MyLogistic.test_data[0:30, 0], MyLogistic.test_data[0:30, 1], "+", label = "classA")
	plt.plot(MyLogistic.test_data[30:60, 0], MyLogistic.test_data[30:60, 1], 'ro', label = "classB")
	plt.legend()
	plt.show()
