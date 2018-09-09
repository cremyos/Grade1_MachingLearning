# -- coding: UTF-8 --
# Name: MNISTInference.py
# Date: 2018.05.16
# Author: Lniper
# Aim:  构建如下图所示识别模型：该模型由两个相同的网络G(x)组成。两个网络共享相同的参数W.
#       例如，输入  负样本对：X1=6的图片 ， X2=9的图片 输出：1
#       输入  正样本对：X1=3的图片 ， X2=3的图片 输出：0
#       G(x)是一个一般的全连接网络（两边的网络结构是一样的！共享参数W、b等），由结构可以自己设计。
#       比如建议两层网络：hidden1：784(28x28)->500; hidden2: 500->10，使用relu。也可以尝试其他节点数组合，和其他非线性变换函数。

import tensorflow as tf

INPUT_NODE = 784
OUTPUT_NODE = 10
LAYER1_NODE = 500

def get_weight_variable(shape, regularizer):
    weights = tf.get_variable("weights", shape, initializer=tf.truncated_normal_initializer(stddev=0.1))
    if regularizer != None:
        tf.add_to_collection('losses', regularizer(weights))
    return weights


def inference(input_tensor, regularizer, reuse = False):
    with tf.variable_scope('layer1', reuse = reuse):
        # 第一层
        weights = get_weight_variable([INPUT_NODE, LAYER1_NODE], regularizer)
        biases = tf.get_variable("biases", [LAYER1_NODE], initializer=tf.constant_initializer(0.0))
        layer1 = tf.nn.relu(tf.matmul(input_tensor, weights) + biases)

    with tf.variable_scope('layer2', reuse=reuse):
        # 第二层
        weights = get_weight_variable([LAYER1_NODE, OUTPUT_NODE], regularizer)
        biases = tf.get_variable("biases", [OUTPUT_NODE], initializer=tf.constant_initializer(0.0))
        layer2 = tf.matmul(layer1, weights) + biases

    return layer2