# -- coding: UTF-8 --
# Name: MNISTTrain.py
# Date: 2018.05.16
# Author: Lniper
# Aim:  构建如下图所示识别模型：该模型由两个相同的网络G(x)组成。两个网络共享相同的参数W.
#       例如，输入  负样本对：X1=6的图片 ， X2=9的图片 输出：1
#       输入  正样本对：X1=3的图片 ， X2=3的图片 输出：0
#       G(x)是一个一般的全连接网络（两边的网络结构是一样的！共享参数W、b等），由结构可以自己设计。
#       比如建议两层网络：hidden1：784(28x28)->500; hidden2: 500->10，使用relu。也可以尝试其他节点数组合，和其他非线性变换函数。
#
#       本程序用于测试MNIST神经网络。
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
import MNISTInference as mnist_inference
import os

# 设置参数
BATCH_SIZE = 100
# 基本学习率
LEARNING_RATE_BASE = 0.8
# 学习率衰减率
LEARNING_RATE_DECAY = 0.99
# 正则化在顺势函数中的系数
REGULARIZATION_RATE = 0.0001
# 训练论述
TRAINING_STEPS = 20000
# 滑动平均衰减率
MOVING_AVERAGE_DECAY = 0.99

# 保存路径和保存文件名
MODEL_SAVE_PATH="MNIST_model/"
MODEL_NAME="mnist_model"


def train(mnist):
    x = tf.placeholder(tf.float32, [None, mnist_inference.INPUT_NODE], name='x-input')
    y_ = tf.placeholder(tf.float32, [None, mnist_inference.OUTPUT_NODE], name='y-input')

    regularizer = tf.contrib.layers.l2_regularizer(REGULARIZATION_RATE)
    y = mnist_inference.inference(x, regularizer)
    # 一般将代表训练轮数的变量指定为不可训练的参数
    global_step = tf.Variable(0, trainable=False)

    # 给定滑动平均衰减率
    variable_averages = tf.train.ExponentialMovingAverage(MOVING_AVERAGE_DECAY, global_step)
    variables_averages_op = variable_averages.apply(tf.trainable_variables())
    cross_entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(logits=y, labels=tf.argmax(y_, 1))
    cross_entropy_mean = tf.reduce_mean(cross_entropy)
    loss = cross_entropy_mean + tf.add_n(tf.get_collection('losses'))
    learning_rate = tf.train.exponential_decay(LEARNING_RATE_BASE, global_step, mnist.train.num_examples / BATCH_SIZE, LEARNING_RATE_DECAY, staircase=True)

    # 优化损失
    train_step = tf.train.GradientDescentOptimizer(learning_rate).minimize(loss, global_step=global_step)
    with tf.control_dependencies([train_step, variables_averages_op]):
        train_op = tf.no_op(name='train')


    saver = tf.train.Saver()
    with tf.Session() as sess:
        tf.global_variables_initializer().run()

        for i in range(TRAINING_STEPS):
            xs, ys = mnist.train.next_batch(BATCH_SIZE)
            _, loss_value, step = sess.run([train_op, loss, global_step], feed_dict={x: xs, y_: ys})
            if i % 1000 == 0:
                print("After %d training step(s), loss on training batch is %g." % (step, loss_value))
                saver.save(sess, os.path.join(MODEL_SAVE_PATH, MODEL_NAME), global_step=global_step)

def runTrain(mnist):
    train(mnist)

# 生成网络
if __name__ == '__main__':
    mnist = input_data.read_data_sets('./data/mnist', one_hot=True)
    runTrain(mnist)



