# -- coding: UTF-8 --
# Name: MnistTest.py
# Date: 2018.06.06
# Author: Lniper
# Aim: 使用tensorboard中的embedding projector工具，对lenet模型提取的数据特征进行可视化
#      要求：训练过程参数记录：
#            1. 在tensorboard中记录查看训练过程中参数（W,b）的分布变化。
#            2. 记录训练过程中loss和ACC的变化
#           测试过程记录：
#            1. 分别记录两次graph，在tensorboard中，查看对比之前的graph和添加embedding variable之后的graph
#            2. 对比直接拿mnist数据的embedding效果，和用训练好网络提取的fc2特征的embedding效果，感受特征学习的效果。
#
import os
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data

LOGDIR = "MyMnistSave/"
SPRITE_FILE = 'mnist_sprite.jpg'
META_FIEL = "mnist_meta.tsv"

MODEL_SAVE_PATH = "lenet_save/"
MODEL_NAME = "Mnist_model"

def conv_layer(input, channels_in, channels_out, name="conv"):
    with tf.name_scope(name):
        w = tf.Variable(tf.truncated_normal([5, 5, channels_in, channels_out], stddev=0.1), name = "W")
        b = tf.Variable(tf.constant(0.1, shape=[channels_out]), name="B")
        conv = tf.nn.conv2d(input, w, strides=[1, 1, 1, 1], padding="SAME")
        act = tf.nn.relu(conv + b)
        tf.summary.histogram("weights", w)
        tf.summary.histogram("biases", b)
        tf.summary.histogram("activations", act)
        return act

# And a fully connected layer
def fc_layer(input, channels_in, channels_out, name = "fc"):
    with tf.name_scope(name):
        w = tf.Variable(tf.truncated_normal([channels_in, channels_out], stddev=0.1), name="W")
        b = tf.Variable(tf.constant(0.1, shape=[channels_out]), name="B")
        act = tf.nn.relu(tf.matmul(input, w) + b)
        tf.summary.histogram("weights", w)
        tf.summary.histogram("biases", b)
        tf.summary.histogram("activations", act)
        return act

def mnist_training(learning_rate):
    tf.reset_default_graph()
    # Setup placeholders, and reshape the data
    x = tf.placeholder(tf.float32, shape=[None, 784])
    y = tf.placeholder(tf.float32, shape=[None, 10])
    x_image = tf.reshape(x, [-1, 28, 28, 1])
    tf.summary.image('input', x_image, 3)


    # Create the network
    conv1 = conv_layer(x_image, 1, 32, "conv1")
    with tf.name_scope("Pool1"):
        pool1 = tf.nn.max_pool(conv1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1],
                               padding="SAME", name = "Pool1")
    conv2 = conv_layer(pool1, 32, 64, "conv2")
    with tf.name_scope("Pool2"):
        pool2 = tf.nn.max_pool(conv2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1],
                               padding="SAME")
    flattened = tf.reshape(pool2, [-1, 7 * 7 * 64])

    keep_prob = tf.placeholder("float")
    # fc1_drop = tf.nn.dropout(fc1, keep_prob)
    fc1 = fc_layer(flattened, 7 * 7 * 64, 1024, "fc1")
    # y_predicted = fc_layer(fc1, 1024, 10, "fc2")
    fc2 = fc_layer(fc1, 1024, 10, "fc2")
    y_predicted = tf.nn.softmax(fc2)
    tf.summary.histogram("y_predicted", y_predicted)

    with tf.name_scope("cross_entroy"):
        cross_entropy = -tf.reduce_sum(y * tf.log(y_predicted))
        tf.summary.scalar('cross_entropy', cross_entropy)

    # Compute cross entropy as our loss function
    # cross_entropy = tf.reduce_mean(
    # tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=y))
    # Use an AdamOptimizer to train the network
    train_step = tf.train.AdamOptimizer(learning_rate).minimize(cross_entropy)
    # compute the accuracy
    with tf.name_scope("accuracy"):
        correct_prediction = tf.equal(tf.argmax(y_predicted, 1), tf.argmax(y, 1))
        accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
        tf.summary.scalar('accuracy', accuracy)


    # 保存模型
    saver = tf.train.Saver()

    merged_summary = tf.summary.merge_all()
    sess = tf.Session()
    writer = tf.summary.FileWriter(LOGDIR + "1")

    writer.add_graph(sess.graph)
    sess.run(tf.global_variables_initializer()) # Initialize all the variables
    # Train for 2000 steps
    for i in range(2001):
        batch = mnist.train.next_batch(100)
        if i % 5 == 0:
            s = sess.run(merged_summary, feed_dict={x: batch[0], y: batch[1], keep_prob: 1.0})
            writer.add_summary(s, i)
        sess.run(train_step, feed_dict={x: batch[0], y: batch[1], keep_prob: 1.0})
        # Occasionally report accuracy
        if i % 500 == 0:
            [train_accuracy] = sess.run([accuracy], feed_dict={x: batch[0], y: batch[1], keep_prob: 1.0})
            print("step %d, training accuracy %g" % (i, train_accuracy))
            # 存储文件
            saver.save(sess, os.path.join(MODEL_SAVE_PATH, MODEL_NAME), global_step=i)
    # Run the training step
    writer.close()

def main(mnist):
    learning_rate = 1e-4
    mnist_training(learning_rate)


if __name__ == "__main__":
    mnist = input_data.read_data_sets("mnist_data/", one_hot=True)
    main(mnist)

