# -- coding: UTF-8 --
# Name: MyLogistic_Regression.py
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

import tensorflow as tf
import numpy as np

def Get_train_data(data_size):
	mean1 = (3, 6)
	cov = [[1, 0], [0, 1]]
	mean2 = (6, 3)
	print(data_size)
	Train_Data = np.zeros((data_size, 2))
	Train_Labels = np.zeros((data_size, 1))
	for i in range(data_size):
		if i < 100:
			# A类训练集
			Train_Data[i, 0:2] = np.random.multivariate_normal(mean1, cov, 1)
			Train_Labels[i, 0] = 1.0
		elif i >= 100 and i< 200:
			# B类训练集
			Train_Data[i, 0:2] = np.random.multivariate_normal(mean2, cov, 1)
			Train_Labels[i, 0] = 0.0
		elif i >= 200 and i < 230:
			# A类验证集
			Train_Data[i, 0:2] = np.random.multivariate_normal(mean1, cov, 1)
			Train_Labels[i, 0] = 1.0
		elif i >= 230 and i < 260:
			# B类验证集
			Train_Data[i, 0:2] = np.random.multivariate_normal(mean2, cov, 1)
			Train_Labels[i, 0] = 0.0
		elif i >= 260 and i < 290:
			# A类测试集
			Train_Data[i, 0:2] = np.random.multivariate_normal(mean1, cov, 1)
			Train_Labels[i, 0] = 1.0
		elif i >= 290 and i < 320:
			# B类测试集
			Train_Data[i, 0:2] = np.random.multivariate_normal(mean2, cov, 1)
			Train_Labels[i, 0] = 0.0

	return Train_Data, Train_Labels


def Train_validationFunc(T_VSignal, X_data, Y_Target):

	# T_VSignal = 1 训练集
	# T_VSignal = 2 验证集
	# T_VSignal = 2 测试集

	with tf.variable_scope("Logistic_Regression", reuse = True):

		Weight = tf.get_variable("Modelweight")
		bias = tf.get_variable("Modelbias")

		# 定义模型，定义损失函数，定义优化目标
		model_output = tf.add(tf.matmul(X_data, Weight), bias)
		# sigmoid_cross_entropy_with_logits 求交叉熵
		loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=model_output, labels=Y_Target))
		my_opt = tf.train.GradientDescentOptimizer(0.01)
		train_step = my_opt.minimize(loss)

		prediction = tf.round(tf.sigmoid(model_output))
		prediction_correct = tf.cast(tf.equal(prediction, Y_Target), tf.float32)
		ACC = tf.reduce_mean(prediction_correct)

	return train_step, loss, ACC



# 训练模型参数
TrainX_data = tf.placeholder(shape=(20, 2), dtype=tf.float32, name = 'TrainX_data')
TrainY_Target = tf.placeholder(shape=(20, 1), dtype=tf.float32, name = 'TrainY_Target')

# 验证模型参数
validationX_data = tf.placeholder(shape=(30, 2), dtype=tf.float32, name = 'validationX_data')
validationY_Target = tf.placeholder(shape=(30, 1), dtype=tf.float32, name = 'validationY_Target')

with tf.variable_scope("Logistic_Regression") as scope:
	# 设置weight 和 bias
	Weight = tf.get_variable("Modelweight", initializer=np.random.rand(2, 1).astype(np.float32))
	bias = tf.get_variable("Modelbias", initializer=np.random.rand(1, 1).astype(np.float32))
	scope.reuse_variables()



# 获取训练数据
TrainVec, TrainLabels = Get_train_data(320)

# 设置训练集、验证集和测试集
train_data = TrainVec[0:200,:]
train_dataLabels = TrainLabels[0:200, :]
# print("train_dataLabels: ")
# print(np.shape(train_dataLabels))

validation_data = TrainVec[200:260,:]
validation_dataLabels = TrainLabels[200:260,:]
# print("validation_dataLabels: ")
# print(np.shape(validation_dataLabels))

test_data = TrainVec[260:320,:]
test_dataLabels = TrainLabels[260:320,:]


Temptrain_stepT, TemplossT, TempACCT = Train_validationFunc(1, TrainX_data, TrainY_Target)
Temptrain_stepV, TemplossV, TempACCV = Train_validationFunc(2, validationX_data, validationY_Target)

# 保存模型
saver = tf.train.Saver()

with tf.Session() as sess:
	# 训练模型
	# 初始化变量
	init = tf.global_variables_initializer()
	sess.run(init)

	# 测试项目
	loss_vec = []
	train_acc = []
	valid_acc = []
	test_acc = []

	for i in range(500):
		kStep = i%20
		# print('kStep = ', kStep)
		# 每次按顺序取train_data中的20个数据进行训练。训练过程中，打印训练的损失函数值及模型在验证集上的精度。
		sess.run(Temptrain_stepT, feed_dict = {TrainX_data: TrainVec[kStep:kStep+20, 0:2], TrainY_Target: train_dataLabels[kStep:kStep+20]})
		# print("Line 142")
		temp_loss = sess.run(TemplossT, feed_dict = {TrainX_data: TrainVec[kStep:kStep+20, 0:2], TrainY_Target: train_dataLabels[kStep:kStep+20]})
		loss_vec.append(temp_loss)
		temp_acc_train = sess.run(TempACCT, feed_dict = {TrainX_data: TrainVec[kStep:kStep+20, 0:2], TrainY_Target: train_dataLabels[kStep:kStep+20]})
		# print("line 149")
		train_acc.append(temp_acc_train)
		print('Train loss = ' + str(temp_loss))

		if i%100 == 0:
			# 每100次迭代用30个验证样本 validation_data 进行验证。训练过程中，打印训练的损失函数值及模型在验证集上的精度。
			# print("line 155")
			j = np.random.randint(30)
			# print("line 157")
			sess.run(Train_validationFunc(2, validationX_data, validationY_Target)[0], feed_dict={validationX_data: TrainVec[j:j + 30, 0:2], validationY_Target: validation_dataLabels[j:j + 30]})
			# print("line 159")
			temp_loss = sess.run(Train_validationFunc(2, validationX_data, validationY_Target)[1], feed_dict={validationX_data: TrainVec[j:j + 30, 0:2], validationY_Target: validation_dataLabels[j:j + 30]})
			loss_vec.append(temp_loss)
			temp_acc_valid = sess.run(Train_validationFunc(2, validationX_data, validationY_Target)[2], feed_dict={validationX_data: TrainVec[j:j + 30, 0:2], validationY_Target: validation_dataLabels[j:j + 30]})
			valid_acc.append(temp_acc_valid)
			print('validation ACC = ' + str(temp_acc_valid))
			saver.save(sess, 'MyLogistic\LogitsModel', global_step=100)


