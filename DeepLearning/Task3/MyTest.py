# -- coding: UTF-8 --
# Name: MyTest.py
# Date: 2018.05.18
# Author: Lniper
# Aim:  构建如下图所示识别模型：该模型由两个相同的网络G(x)组成。两个网络共享相同的参数W.
#       例如，输入  负样本对：X1=6的图片 ， X2=9的图片 输出：1
#       输入  正样本对：X1=3的图片 ， X2=3的图片 输出：0
#       G(x)是一个一般的全连接网络（两边的网络结构是一样的！共享参数W、b等），由结构可以自己设计。
#       比如建议两层网络：hidden1：784(28x28)->500; hidden2: 500->10，使用relu。也可以尝试其他节点数组合，和其他非线性变换函数。
#       
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
import MNISTInference as mnInferece
import MNISTTrain as mnTrain
import numpy as np

batch_size = 64
learning_rate = 0.005

def evaluate():
    iter = 100
    itera = 0
    for itera in range(iter):
        mnist = input_data.read_data_sets('./data/mnist', one_hot=True)
        x_1, y_1 = mnist.train.next_batch(batch_size)
        # print("x_1 = ", x_1)

        x_2, y_2 = mnist.train.next_batch(batch_size)
        # print("x_2 = ", x_2)

        with tf.Graph().as_default() as g:
            # x1数据
            x = tf.placeholder(tf.float32, [None, mnInferece.INPUT_NODE], name='x-input')
            y_ = tf.placeholder(tf.float32, [None, mnInferece.OUTPUT_NODE], name='y-input')

            # x2数据
            x2 = tf.placeholder(tf.float32, [None, mnInferece.INPUT_NODE], name='x-input2')
            y2_ = tf.placeholder(tf.float32, [None, mnInferece.OUTPUT_NODE], name='y-input2')

            y1 = mnInferece.inference(x, None)
            # tf.argmax(y1, 1)可以得到输入样例中的预测类别
            correct_prediction1 = tf.equal(tf.argmax(y1, 1), tf.argmax(y_, 1))

            y2 = mnInferece.inference(x2, None, True)
            # tf.argmax(y1, 1)可以得到输入样例中的预测类别
            correct_prediction2 = tf.equal(tf.argmax(y2, 1), tf.argmax(y_2, 1))

            accuracy1 = tf.reduce_mean(tf.cast(correct_prediction1, tf.float32))
            accuracy2 = tf.reduce_mean(tf.cast(correct_prediction2, tf.float32))

            variable_averages = tf.train.ExponentialMovingAverage(mnTrain.MOVING_AVERAGE_DECAY)
            variables_to_restore = variable_averages.variables_to_restore()
            saver = tf.train.Saver(variables_to_restore)

            # 计算损失函数
            y_s = np.array(correct_prediction2 != correct_prediction1, dtype = np.float32)
            Q = 5
            # E_w = tf.sqrt(tf.reduce_sum(tf.square(o1-o2),1))，  tf.exp()等请自查
            E_w = tf.sqrt(tf.reduce_sum(tf.square((y1 - y2)), 1))
            L = (1 - y_s) * (2 / Q) * tf.square(E_w) + y_s * 2 * Q * tf.exp(-2.77 / Q)

            OptimLoss = tf.train.GradientDescentOptimizer(learning_rate).minimize(L)

            accuracy3 = tf.reduce_mean(tf.cast(L, tf.float32))

            with tf.Session() as sess:
                ckpt = tf.train.get_checkpoint_state(mnTrain.MODEL_SAVE_PATH)

                if ckpt and ckpt.model_checkpoint_path:
                    saver.restore(sess, ckpt.model_checkpoint_path)
                    global_step = ckpt.model_checkpoint_path.split('/')[-1].split('-')[-1]

                    validate_feed1 = {x: x_1, y_: y_1}
                    Predict1 = sess.run(correct_prediction1, feed_dict=validate_feed1)
                    accuracy_score1 = sess.run(accuracy1, feed_dict=validate_feed1)

                    validate_feed2 = {x2: x_2, y2_: y_2}
                    Predict2 = sess.run(correct_prediction2, feed_dict=validate_feed2)
                    accuracy_score2 = sess.run(accuracy2, feed_dict=validate_feed2)

                    # 例如，输入  负样本对：X1=6的图片 ， X2=9的图片 输出：1
                    #       输入  正样本对：X1=3的图片 ， X2=3的图片 输出：0
                    y_sOut = np.array(Predict1 != Predict2, dtype=np.float32)
                    # y_sOut = sess.run(y_s, {x: x_1, x2: x_2, y_: y_1, y2_: y_2})
                    print("y_sOut = ", y_sOut)

                    L_OutPut = sess.run(OptimLoss, {x: x_1, x2: x_2, y_: y_1, y2_: y_2})
                    print("L_OutPut = ", L_OutPut)

                    accuracy_score3 = sess.run(accuracy3, {x: x_1, x2: x_2, y_: y_1, y2_: y_2})

                    print("After %s training step(s), validation accuracy = %g" % (global_step, accuracy_score1))
                    print("After %s training step(s), validation accuracy = %g" % (global_step, accuracy_score2))
                    print("After %s training step(s), validation accuracy = %g" % (global_step, accuracy_score3))

                else:
                    print('No checkpoint file found')
                    return


def main(argv=None):
    evaluate()

if __name__ == '__main__':
    main()



