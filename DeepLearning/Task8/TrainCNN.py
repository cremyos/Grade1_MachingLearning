# -- coding: UTF-8 --
# Name: TrainCNN.py
# Date: 2018.05.24
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
#		用class封装好的lenet对minist进行分类，训练和模型定义分开成两个文件train.py, lenet.py，打印训练和测试截图，测试分类准确率ACC。
#
import tensorflow as tf
import CNNClass as CNClass
import tensorflow.examples.tutorials.mnist.input_data as input_data

def main(mnist):
	batch_size = 50

	x_data, y_realdata = mnist.train.next_batch(batch_size)
	MyCNN = CNClass.MyCNNClass(0.1, x_data)

	y_real = tf.placeholder(tf.float32, shape=[None, 10])

	# 模型训练评估
	cross_entropy = -tf.reduce_sum(y_real * tf.log(MyCNN.y_preRet))
	train_step = tf.train.GradientDescentOptimizer(1e-3).minimize(cross_entropy)
	correct_prediction = tf.equal(tf.argmax(MyCNN.y_preRet, 1), tf.argmax(y_real, 1))
	accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))

	Train_Step = 20000

	with tf.Session() as sess:
		sess.run(tf.global_variables_initializer())

		for i in range(Train_Step):
			if i % 100 == 0:  # 训练100次
				Train_stepOut, train_accuracy = sess.run([train_step, accuracy], feed_dict={MyCNN.raw_input_image: x_data, y_real: y_realdata, MyCNN.KEEPPRO: 0.5})
				print('step %d,training accuracy %g' % (i, train_accuracy))


		test_accuracy = sess.run(accuracy, feed_dict={MyCNN.raw_input_image: mnist.test.images, y_real: mnist.test.labels, MyCNN.KEEPPRO: 1.0})
		print("test accuracy", test_accuracy)


if __name__ == '__main__':
	# 下载minist数据，创建mnist_data文件夹，one_hot编码
	mnist = input_data.read_data_sets("mnist_data/", one_hot=True)
	main(mnist)


