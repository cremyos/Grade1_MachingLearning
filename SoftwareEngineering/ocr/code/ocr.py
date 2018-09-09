import csv
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np
from numpy import matrix
from math import pow
from collections import namedtuple
import math
import random
import os
import json
import tensorflow.examples.tutorials.mnist.input_data as input_data
from PIL import Image, ImageFilter
import tensorflow as tf


"""
This class does some initial training of a neural network for predicting drawn
digits based on a data set in data_matrix and data_labels. It can then be used to
train the network further by calling train() with any array of data or to predict
what a drawn digit is by calling predict().

The weights that define the neural network can be saved to a file, NN_FILE_PATH,
to be reloaded upon initilization.
"""



class OCRNeuralNetwork:
    LEARNING_RATE = 0.1
    WIDTH_IN_PIXELS = 20
    NN_FILE_PATH = 'nn.json'

    def __init__(self, num_hidden_nodes, data_matrix, data_labels, training_indices, use_file=True):
        # mnist = input_data.read_data_sets("mnist_data/", one_hot=True)
        # 初始训练
        # self.Inittrain(mnist)
        # self.sess = self.MyPredict()
        self.sigmoid = np.vectorize(self._sigmoid_scalar)
        self.sigmoid_prime = np.vectorize(self._sigmoid_prime_scalar)
        self._use_file = use_file
        self.data_matrix = data_matrix
        self.data_labels = data_labels

        if (not os.path.isfile(OCRNeuralNetwork.NN_FILE_PATH) or not use_file):
            # Step 1: Initialize weights to small numbers
            self.theta1 = self._rand_initialize_weights(400, num_hidden_nodes)
            self.theta2 = self._rand_initialize_weights(num_hidden_nodes, 10)
            self.input_layer_bias = self._rand_initialize_weights(1, num_hidden_nodes)
            self.hidden_layer_bias = self._rand_initialize_weights(1, 10)

            # Train using sample data
            TrainData = namedtuple('TrainData', ['y0', 'label'])
            self.train([TrainData(self.data_matrix[i], int(self.data_labels[i])) for i in training_indices])
            self.save()
        else:
            self._load()

    def _rand_initialize_weights(self, size_in, size_out):
        return [((x * 0.12) - 0.06) for x in np.random.rand(size_out, size_in)]

    # The sigmoid activation function. Operates on scalars.
    def _sigmoid_scalar(self, z):
        return 1 / (1 + math.e ** -z)

    def _sigmoid_prime_scalar(self, z):
        return self.sigmoid(z) * (1 - self.sigmoid(z))

    def _draw(self, sample):
        pixelArray = [sample[j:j+self.WIDTH_IN_PIXELS] for j in range(0, len(sample), self.WIDTH_IN_PIXELS)]
        plt.imshow(zip(*pixelArray), cmap = cm.Greys_r, interpolation="nearest")
        plt.show()

    def Origin_train(self, training_data_array):
        for data in training_data_array:
            # Step 2: Forward propagation
            y1 = np.dot(np.mat(self.theta1), np.mat(data['y0']).T)#修改后#y1 = np.dot(np.mat(self.theta1), np.mat(data['y0']).T)
            sum1 =  y1 + np.mat(self.input_layer_bias) # Add the bias
            y1 = self.sigmoid(sum1)


            y2 = np.dot(np.array(self.theta2), y1)
            y2 = np.add(y2, self.hidden_layer_bias) # Add the bias
            y2 = self.sigmoid(y2)

            # Step 3: Back propagation
            actual_vals = [0] * 10 # actual_vals is a python list for easy initialization and is later turned into an np matrix (2 lines down).
            # actual_vals[data.label] = 1#修改后#
            actual_vals[data['label']] = 1
            output_errors = np.mat(actual_vals).T - np.mat(y2)
            hidden_errors = np.multiply(np.dot(np.mat(self.theta2).T, output_errors), self.sigmoid_prime(sum1))

            # Step 4: Update weights
            # self.theta1 += self.LEARNING_RATE * np.dot(np.mat(hidden_errors), np.mat(data.y0))#修改后#
            self.theta1 += self.LEARNING_RATE * np.dot(np.mat(hidden_errors), np.mat(data['y0']))
            self.theta2 += self.LEARNING_RATE * np.dot(np.mat(output_errors), np.mat(y1).T)
            self.hidden_layer_bias += self.LEARNING_RATE * output_errors
            self.input_layer_bias += self.LEARNING_RATE * hidden_errors

    def weight_variable(self, shape):
        initial = tf.truncated_normal(shape, stddev=0.1)
        return tf.Variable(initial)

    def bias_variable(self, shape):
        initial = tf.constant(0.1, shape=shape)
        return tf.Variable(initial)

    def conv2d(self, x, W):
        return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

    def max_pool_2x2(self, x):
        return tf.nn.max_pool(x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')

    def Inittrain(self, mnist):
        # mnist = self.mnist
        x = tf.placeholder(tf.float32, [None, 784])

        y_ = tf.placeholder(tf.float32, [None, 10])

        W_conv1 = self.weight_variable([5, 5, 1, 32])
        b_conv1 = self.bias_variable([32])

        x_image = tf.reshape(x, [-1, 28, 28, 1])

        h_conv1 = tf.nn.relu(self.conv2d(x_image, W_conv1) + b_conv1)
        h_pool1 = self.max_pool_2x2(h_conv1)

        W_conv2 = self.weight_variable([5, 5, 32, 64])
        b_conv2 = self.bias_variable([64])

        h_conv2 = tf.nn.relu(self.conv2d(h_pool1, W_conv2) + b_conv2)
        h_pool2 = self.max_pool_2x2(h_conv2)

        W_fc1 = self.weight_variable([7 * 7 * 64, 1024])
        b_fc1 = self.bias_variable([1024])

        h_pool2_flat = tf.reshape(h_pool2, [-1, 7 * 7 * 64])
        h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

        keep_prob = tf.placeholder("float")
        h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

        W_fc2 = self.weight_variable([1024, 10])
        b_fc2 = self.bias_variable([10])

        y_conv = tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)

        cross_entropy = -tf.reduce_sum(y_ * tf.log(y_conv))
        train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
        correct_prediction = tf.equal(tf.argmax(y_conv, 1), tf.argmax(y_, 1))
        accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))

        saver = tf.train.Saver()  # 定义saver

        with tf.Session() as sess:
            sess.run(tf.global_variables_initializer())
            for i in range(1500):
                batch = mnist.train.next_batch(50)
                if i % 500 == 0:
                    train_accuracy = accuracy.eval(feed_dict={x: batch[0], y_: batch[1], keep_prob: 1.0})
                    print('step %d, training accuracy %g' % (i, train_accuracy))
                train_step.run(feed_dict={x: batch[0], y_: batch[1], keep_prob: 0.5})
            saver.save(sess, 'NewModel/model.ckpt')  # 模型储存位置

        print("Finish init")

    # 原先的predict
    def predict(self, test):
        y1 = np.dot(np.mat(self.theta1), np.mat(test).T)
        y1 =  y1 + np.mat(self.input_layer_bias) # Add the bias
        y1 = self.sigmoid(y1)

        y2 = np.dot(np.array(self.theta2), y1)
        y2 = np.add(y2, self.hidden_layer_bias) # Add the bias
        y2 = self.sigmoid(y2)

        results = y2.T.tolist()[0]
        return results.index(max(results))

    def MyPredict(self, test):

        Tempres = (np.array(test, dtype=np.float32) * 1.0).reshape(784, ).tolist()
        print("Tempres = ", Tempres)

        result = Tempres  # predint = prediction.eval(feed_dict={x: [result], keep_prob: 1.0}, session=sess)

        # temp = self.imageprepare()

        x = tf.placeholder(tf.float32, [None, 784])
        y_ = tf.placeholder(tf.float32, [None, 10])

        W_conv1 = self.weight_variable([5, 5, 1, 32])
        b_conv1 = self.bias_variable([32])

        x_image = tf.reshape(x, [-1, 28, 28, 1])

        h_conv1 = tf.nn.relu(self.conv2d(x_image, W_conv1) + b_conv1)
        h_pool1 = self.max_pool_2x2(h_conv1)

        W_conv2 = self.weight_variable([5, 5, 32, 64])
        b_conv2 = self.bias_variable([64])

        h_conv2 = tf.nn.relu(self.conv2d(h_pool1, W_conv2) + b_conv2)
        h_pool2 = self.max_pool_2x2(h_conv2)

        W_fc1 = self.weight_variable([7 * 7 * 64, 1024])
        b_fc1 = self.bias_variable([1024])

        h_pool2_flat = tf.reshape(h_pool2, [-1, 7 * 7 * 64])
        h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

        keep_prob = tf.placeholder("float")
        h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

        W_fc2 = self.weight_variable([1024, 10])
        b_fc2 = self.bias_variable([10])

        y_conv = tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)

        cross_entropy = -tf.reduce_sum(y_ * tf.log(y_conv))
        train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
        correct_prediction = tf.equal(tf.argmax(y_conv, 1), tf.argmax(y_, 1))
        accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))

        saver = tf.train.Saver()

        print("Line 224")

        with tf.Session() as sess:
            # new_saver = tf.train.import_meta_graph("model_v/model.ckpt.meta")
            sess.run(tf.global_variables_initializer())
            saver.restore(sess, "CNNModel/model.ckpt")  # 使用模型，参数和之前的代码保持一致

            print("Line 231")

            prediction = tf.argmax(y_conv, 1)

            print("Line 249")
            # predint = prediction.eval(feed_dict={x: [result], keep_prob: 1.0}, session=sess)
            predint = sess.run(prediction, feed_dict = {x: [result], keep_prob: 1.0})

            print("Line 232")

            print('识别结果:')
            # print(predint[0])
            print(predint.flatten().tolist())

        return predint[0]

    # 预测
    # def PreNum(self, test):
    #
    #
    #     predint = self.sess.run(self.prediction, feed_dict = {self.x: [result]})
    #
    #     print("Line 232")
    #
    #     print('识别结果:')
    #     # print(predint[0])
    #     print(predint.flatten().tolist())
    #
    #     return predint[0]


    # 临时使用
    def imageprepare(self):
        im = Image.open('3.png')  # 读取的图片所在路径，注意是28*28像素
        # plt.imshow(im)  # 显示需要识别的图片
        # plt.show()
        im = im.convert('L')
        tv = list(im.getdata())
        tva = [(255 - x) * 1.0 / 255.0 for x in tv]
        return tva


    def save(self):
        if not self._use_file:
            return

        json_neural_network = {
            "theta1":[np_mat.tolist()[0] for np_mat in self.theta1],
            "theta2":[np_mat.tolist()[0] for np_mat in self.theta2],
            "b1":self.input_layer_bias[0].tolist()[0],
            "b2":self.hidden_layer_bias[0].tolist()[0]
        };
        with open(OCRNeuralNetwork.NN_FILE_PATH,'w') as nnFile:
            json.dump(json_neural_network, nnFile)

    def _load(self):
        if not self._use_file:
            return

        with open(OCRNeuralNetwork.NN_FILE_PATH) as nnFile:
            nn = json.load(nnFile)
        self.theta1 = [np.array(li) for li in nn['theta1']]
        self.theta2 = [np.array(li) for li in nn['theta2']]
        self.input_layer_bias = [np.array(nn['b1'][0])]
        self.hidden_layer_bias = [np.array(nn['b2'][0])]

