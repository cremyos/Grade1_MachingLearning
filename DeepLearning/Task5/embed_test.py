# -- coding: UTF-8 --
# Name: embed_test.py
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
import tensorflow as tf
import tensorflow.examples.tutorials.mnist.input_data as input_data
import os
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.contrib.tensorboard.plugins import projector
import MnistTest as MNT


LOG_DIR = "Embed_Test/"
NAME_TO_VISUALISE_VARIABLE = "mnistEmbedding"
SPRITE_FILE = 'mnist_sprite.jpg'
META_FIEL = "mnist_meta.tsv"

TO_EMBED_COUNT = 640

TENSOR_NAME = "FINAL"
TRAINING_STEPS = 2000

path_for_mnist_sprites = os.path.join(LOG_DIR, SPRITE_FILE)
path_for_mnist_metadata = os.path.join(LOG_DIR, META_FIEL)



def embeding_config():
    embedding_var = tf.Variable(np.zeros([TO_EMBED_COUNT, 84]), name=NAME_TO_VISUALISE_VARIABLE, dtype=tf.float32)

    summary_writer = tf.summary.FileWriter(LOG_DIR)
    summary_writer.add_graph(tf.get_default_graph())

    config = projector.ProjectorConfig()
    embedding = config.embeddings.add()
    embedding.tensor_name = embedding_var.name

    # Specify where you find the metadata
    embedding.metadata_path = path_for_mnist_metadata  # 'metadata.tsv'

    # Specify where you find the sprite (we will create this later)
    embedding.sprite.image_path = path_for_mnist_sprites  # 'mnistdigits.png'
    embedding.sprite.single_image_dim.extend([28, 28])

    # Say that you want to visualise the embeddings
    projector.visualize_embeddings(summary_writer, config)
    return embedding_var

# 将sprits整齐的对齐在方形画布上
def create_sprite_image(images):
    """Returns a sprite image consisting of images passed as argument. Images should be count x width x height"""
    if isinstance(images, list):
        images = np.array(images)
    img_h = images.shape[1]
    img_w = images.shape[2]
    n_plots = int(np.ceil(np.sqrt(images.shape[0])))

    spriteimage = np.ones((img_h * n_plots, img_w * n_plots))

    for i in range(n_plots):
        for j in range(n_plots):
            this_filter = i * n_plots + j
            if this_filter < images.shape[0]:
                this_img = images[this_filter]
                spriteimage[i * img_h:(i + 1) * img_h,
                j * img_w:(j + 1) * img_w] = this_img

    return spriteimage

# 将MNIST的vector数据形式化转化为images
def vector_to_matrix_mnist(mnist_digits):
    """Reshapes normal mnist digit (batch,28*28) to matrix (batch,28,28)"""
    return np.reshape(mnist_digits, (-1, 28, 28))

# 将黑背景改变为白背景
def invert_grayscale(mnist_digits):
    """ Makes black white, and white black """
    return 1 - mnist_digits

def sprite_and_meta_writer(batch_xs, batch_ys, path_for_mnist_sprites, path_for_mnist_metadata):
    to_visualise = batch_xs
    to_visualise = vector_to_matrix_mnist(to_visualise)
    to_visualise = invert_grayscale(to_visualise)

    sprite_image = create_sprite_image(to_visualise)

    plt.imsave(path_for_mnist_sprites, sprite_image, cmap='gray')
    plt.imshow(sprite_image, cmap='gray')

    with open(path_for_mnist_metadata, 'w') as f:
        f.write("Index\tLabel\n")
        for index, label in enumerate(batch_ys):
            f.write("%d\t%d\n" % (index, label))

def visualisation(final_result):
    y = tf.Variable(final_result, name=TENSOR_NAME)
    summary_writer = tf.summary.FileWriter(LOG_DIR)

    config = projector.ProjectorConfig()
    embedding = config.embeddings.add()
    embedding.tensor_name = y.name

    # Specify where you find the metadata
    embedding.metadata_path = META_FIEL

    # Specify where you find the sprite (we will create this later)
    embedding.sprite.image_path = SPRITE_FILE
    embedding.sprite.single_image_dim.extend([28, 28])

    # Say that you want to visualise the embeddings
    projector.visualize_embeddings(summary_writer, config)

    sess = tf.InteractiveSession()
    sess.run(tf.global_variables_initializer())
    saver = tf.train.Saver()
    saver.save(sess, os.path.join(LOG_DIR, "model"), TRAINING_STEPS)


    summary_writer.close()

# 原始mnist数据集
def main():
    mnist = input_data.read_data_sets("mnist_data/", one_hot=False)
    path_for_mnist_sprites = os.path.join(LOG_DIR, SPRITE_FILE)
    path_for_mnist_metadata = os.path.join(LOG_DIR, META_FIEL)

    batch_xs, batch_ys = mnist.train.next_batch(TO_EMBED_COUNT)

    # embedding_var = embeding_config(path_for_mnist_metadata, path_for_mnist_sprites)

    sess = tf.InteractiveSession()
    sess.run(tf.global_variables_initializer())
    saver = tf.train.Saver()
    saver.save(sess, os.path.join(LOG_DIR, "model.ckpt"), 1)
    to_visualise = 1 - np.reshape(mnist.test.images, (-1, 28, 28))
    sprite_image = create_sprite_image(to_visualise)

    plt.imsave(path_for_mnist_sprites, sprite_image, cmap='gray')
    plt.imshow(sprite_image, cmap='gray')

    print(mnist.test.labels)

    with open(path_for_mnist_metadata, 'w') as f:
        f.write("Index\tLabel\n")
        for index, label in enumerate(mnist.test.labels):
            f.write("%d\t%d\n" % (index, label))

def NewMain():
    mnist = input_data.read_data_sets("mnist_data/", one_hot=True)

    images, labels = mnist.train.next_batch(TO_EMBED_COUNT)

    x = tf.placeholder(tf.float32, [None, 784], name='x-input')
    y_ = tf.placeholder(tf.float32, [None, 10], name='y-input')
    # validate_feed = {x: mnist.validation.images, y_: mnist.validation.labels}
    validate_feed = {x: images, y_: labels}
    x_image = tf.reshape(x, [-1, 28, 28, 1])

    # Create the network
    conv1 = MNT.conv_layer(x_image, 1, 32, "conv1")
    with tf.name_scope("Pool1"):
        pool1 = tf.nn.max_pool(conv1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1],
                               padding="SAME", name = "Pool1")
    conv2 = MNT.conv_layer(pool1, 32, 64, "conv2")
    with tf.name_scope("Pool2"):
        pool2 = tf.nn.max_pool(conv2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1],
                               padding="SAME")
    flattened = tf.reshape(pool2, [-1, 7 * 7 * 64])

    keep_prob = tf.placeholder("float")
    fc1 = MNT.fc_layer(flattened, 7 * 7 * 64, 1024, "fc1")
    fc2 = MNT.fc_layer(fc1, 1024, 10, "fc2")
    y_predicted = tf.nn.softmax(fc2)

    fc2_samples = np.zeros((TO_EMBED_COUNT,84))

    cross_entropy = -tf.reduce_sum(y_ * tf.log(y_predicted))
    train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)

    correct_prediction = tf.equal(tf.argmax(y_predicted, 1), tf.argmax(y_, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

    # variable_averages = tf.train.ExponentialMovingAverage(0.9)
    # variables_to_restore = variable_averages.variables_to_restore()
    # saver = tf.train.Saver(variables_to_restore)
    saver = tf.train.Saver()

    with tf.Session() as sess:

        ckpt = tf.train.get_checkpoint_state(MNT.MODEL_SAVE_PATH)
        if ckpt and ckpt.model_checkpoint_path:
            saver.restore(sess, ckpt.model_checkpoint_path)
            global_step = ckpt.model_checkpoint_path.split('/')[-1].split('-')[-1]
            TrainStep, accuracy_score = sess.run([train_step, accuracy], feed_dict=validate_feed)
            print("After %s training step(s), validation accuracy = %g" % (global_step, accuracy_score))
            # final_res = sess.run(y_predicted, feed_dict=validate_feed)
            final_res = sess.run(fc2, feed_dict=validate_feed)
        else:
            print('No checkpoint file found')
            return

    return final_res


if __name__ == '__main__':

    # 原始mnist投影
    # main()

    # 可视化经过lenet的网络结果
    final_res = NewMain()
    # print(final_res)
    visualisation(final_res)