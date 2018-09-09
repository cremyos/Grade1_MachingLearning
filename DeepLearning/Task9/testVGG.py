"""
Simple tester for the vgg19_trainable
"""

import tensorflow as tf

import VGG as vgg19
import utils
import cv2

# img1 = utils.load_image("./test_data/a (1).jpeg")
# img1 = utils.load_image("MNIST_data/mnist_train/0/mnist_train_1.png")
img1 = cv2.imread("MNIST_data/mnist_train/0/mnist_train_1.png", cv2.IMREAD_GRAYSCALE)

img1_true_result = [1 if i == 292 else 0 for i in range(1000)]  # 1-hot result for tiger

# batch1 = img1.reshape((1, 224, 224, 3))
batch1 = cv2.resize(img1, (224, 224), 3)

with tf.device('/cpu:0'):
    sess = tf.Session()

    images = tf.placeholder(tf.float32, [1, 224, 224, 3])
    true_out = tf.placeholder(tf.float32, [1, 1000])
    train_mode = tf.placeholder(tf.bool)

    # vgg = vgg19.Vgg19('./vgg19.npy')
    vgg = vgg19.Vgg19()
    vgg.build(images, train_mode)

    # print number of variables used: 143667240 variables, i.e. ideal size = 548MB
    print(vgg.get_var_count())

    sess.run(tf.global_variables_initializer())

    # test classification
    prob = sess.run(vgg.prob, feed_dict={images: batch1, train_mode: True})
    utils.print_prob(prob[0], './synset.txt')

    # simple 1-step training
    cost = tf.reduce_sum((vgg.prob - true_out) ** 2)
    train = tf.train.GradientDescentOptimizer(0.0001).minimize(cost)
    sess.run(train, feed_dict={images: batch1, true_out: [img1_true_result], train_mode: True})

    # test classification again, should have a higher probability about tiger
    prob = sess.run(vgg.prob, feed_dict={images: batch1, train_mode: False})
    utils.print_prob(prob[0], './synset.txt')

    # test save
    vgg.save_npy(sess, './test-save.npy')