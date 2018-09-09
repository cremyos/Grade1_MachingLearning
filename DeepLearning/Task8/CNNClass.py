# -- coding: UTF-8 --
# Name: CNNClass.py
# Date: 2018.05.22
# Author: Lniper
# Aim:  1. lenet 结构如附件描述。注意：
#         （1）lenet 输入为32x32，而minist为28x28，故需要先对数据进行填充，例如：
#         （2）lenet 输出 10位的 one-hot形式的输出 logits, 故minist的标签读取需采用one-hot的形式。采用softmax 交叉熵作为损失函数。用softmax进行分类。
#       2. 在init函数中传入初始化变量所需的mu， sigma参数，以及其他所需定制化参数。
#       3. 对lenet中常见的conv层，fc层，pooling层定义统一的定制化功能层graph绘图函数. 为层次化组织网络，给每个层定义一个不同的名字空间，例如：
#       def conv(w_shape, scope_name, .......):
#           with tf.name_scope(scope_name) as scope:
#             xxxx.....
#       4. 绘制整个网络计算图的函数，net_build(). 这里要求调用net_build()的过程放在 _init_函数里，这样外部每调用并生成一个class的实例，实际上就自动绘制了一次lenet。
#       5. 在外部调用该模块并通过实例化实现对lenet的绘制，例如：
#
# 		用class封装好的lenet对minist进行分类，训练和模型定义分开成两个文件train.py, lenet.py，打印训练和测试截图，测试分类准确率ACC。

import tensorflow as tf

class MyCNNClass(object):
	def __init__(self, sigma, X_input):
		self.sigma = sigma
		self.raw_input_image = tf.placeholder(tf.float32, [None, 784])
		self.input_x = tf.reshape(self.raw_input_image, [-1, 28, 28, 1])
		self.KEEPPRO = tf.placeholder("float")
		self.y_preRet = self.net_build()


	# 初始化权重
	def weight_variable(self, shape):
		initial = tf.truncated_normal(shape, stddev = self.sigma)  # 正态分布
		return tf.Variable(initial)

	# 初始化偏置
	def bias_variable(self, shape):
		initial = tf.constant(0.1, shape=shape)  # 偏置初始化为0.1
		return tf.Variable(initial)

	# 构建卷积层
	def conv2d(self, X_input, W, name):
		return tf.nn.conv2d(X_input, W, strides=[1, 1, 1, 1], padding='SAME', name=name)  # 卷积步长为1,不足补0

	# 构建池化层
	def max_pool(sekf, X_input, name):
		# 大小2*2,步长为2,不足补0
		return tf.nn.max_pool(X_input, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME', name=name)

	def net_build(self):
		with tf.name_scope('Layer1') as scope:
			# 第一层
			W_conv1 = self.weight_variable([5, 5, 1, 32])
			b_conv1 = self.bias_variable([32])
			h_conv1 = tf.nn.relu(self.conv2d(self.input_x, W_conv1, "h_conv1") + b_conv1)  # 卷积层
			h_pool1 = self.max_pool(h_conv1, "h_pool1")  # 池化层

		with tf.name_scope('Layer2') as scope:
			# 第二层
			W_conv2 = self.weight_variable([5, 5, 32, 64])
			b_conv2 = self.bias_variable([64])
			h_conv2 = tf.nn.relu(self.conv2d(h_pool1, W_conv2, "h_conv2") + b_conv2)
			h_pool2 = self.max_pool(h_conv2, "h_pool2")

		with tf.name_scope('FullConnect1') as scope:
			# 全连接层
			W_fc1 = self.weight_variable([7 * 7 * 64, 1024])
			b_fc1 = self.bias_variable([1024])
			h_pool2_flat = tf.reshape(h_pool2, [-1, 7 * 7 * 64])
			h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)
			# dropout
			keep_prob = tf.placeholder("float")
			h_fc1_drop = tf.nn.dropout(h_fc1, self.KEEPPRO)

		with tf.name_scope('Output') as scope:
			# 输出层
			W_fc2 = self.weight_variable([1024, 10])
			b_fc2 = self.bias_variable([10])
			y_predict = tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)

		return y_predict


