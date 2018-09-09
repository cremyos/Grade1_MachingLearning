import tensorflow as tf
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
# 开始读取测试图片


def ImageToMatrix(filename):
    im = Image.open(filename)
    width, height = im.size
    im = im.convert("L")
    data = im.getdata()
    data = np.matrix(data, dtype='float')/255.0
    new_data = np.reshape(data, (height, width))
    return new_data

def ImageToMatrix2(ima):
    width, height = ima.size
    ima = ima.convert("L")
    data = ima.getdata()
    data = np.matrix(data, dtype='float')/255.0
    new_data = np.reshape(data, (height, width))
    return new_data

def MatrixToImage(data):
    data = data*255
    new_im = Image.fromarray(data.astype(np.uint8))
    return new_im


test_pic = ImageToMatrix('test.png')

# 完成测试图片读取
# -------------------------
# 开始处理测试图片
# 开始寻找图片四边
hang, lie = np.shape(test_pic)
for top in range(0, hang):
    if np.min(test_pic[top, :]) != 1:
        break
for bot in range(hang-1, 0, -1):
    if np.min(test_pic[bot, :]) != 1:
        break
for left in range(0, lie):
    if np.min(test_pic[:, left]) != 1:
        break
for right in range(lie - 1, 0, -1):
    if np.min(test_pic[:, right]) != 1:
        break
new_test_pic = test_pic[top:bot, left:right]
# 完成图片四边寻找
# 开始进行图片尺寸转换
pic = MatrixToImage(new_test_pic)
pic2 = pic.resize((28, 28))
test_data = ImageToMatrix2(pic2)
test_data = np.reshape(test_data, (1, 784))
# 完成图片尺寸转换

# 完成测试图片的处理
# --------------------------
# 开始定义神经网络结构

# 定义占位符x和y_
x = tf.placeholder(tf.float32, shape=[None, 784])
y_ = tf.placeholder(tf.float32, shape=[None, 26])


# 开始定义用于初始化的两个函数
def weight_variable(shape):
    initial = tf.truncated_normal(shape, stddev=0.1)
    return tf.Variable(initial)


def bias_variable(shape):
    initial = tf.constant(0.1, shape=shape)
    return tf.Variable(initial)

# 完成初始化函数定义


# 开始定义卷积和池化的函数
# 卷积使用1步长（stride size），0边距（padding size）的模板，保证输出和输入大小相同
# 池化用简单传统的2x2大小的模板做max pooling，因此输出的长宽会变为输入的一半


def conv2d(x, W):
    return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')


def max_pool_2x2(x):
    return tf.nn.max_pool(x, ksize=[1,2,2,1], strides=[1, 2, 2, 1], padding='SAME')
# 完成卷积池化函数定义


# 开始定义神经网络结构定义
# 第一层卷积，卷积在每个5x5的patch中算出32个特征
W_conv1 = weight_variable([5, 5, 1, 32])
b_conv1 = bias_variable([32])
x_image = tf.reshape(x, [-1, 28, 28, 1])
# 第2、第3维对应图片的宽、高，最后一维代表图片的颜色通道数(因为是灰度图所以这里的通道数为1，如果是rgb彩色图，则为3)
h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)
h_pool1 = max_pool_2x2(h_conv1)

# 第二层卷积，每个5x5的patch会得到64个特征
W_conv2 = weight_variable([5, 5, 32, 64])
b_conv2 = bias_variable([64])
h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
h_pool2 = max_pool_2x2(h_conv2)

# 有1024个神经元的全连接层，此时图片大小为7*7
W_fc1 = weight_variable([7*7*64, 1024])
b_fc1 = bias_variable([1024])
h_pool2_flat = tf.reshape(h_pool2, [-1, 7*7*64])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

# 为了减少过拟合，在输出层之前加入dropout。用一个placeholder代表一个神经元的输出在dropout中保持不变的概率。
# 这样可以在训练过程中启用dropout，在测试过程中关闭dropout。
keep_prob = tf.placeholder(tf.float32)
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)
# softmax输出层
W_fc2 = weight_variable([1024, 26])
b_fc2 = bias_variable([26])
y_conv = tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)
# 应为 y_conv=tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)
# 完成神经网络结构定义

# 开始定义训练和评估操作
# 用更加复杂的ADAM优化器来做梯度最速下降，在feed_dict中加入额外的参数keep_prob来控制dropout比例
cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels=y_, logits=y_conv))
train_step = tf.train.AdamOptimizer(1e-6).minimize(cross_entropy)
correct_prediction = tf.equal(tf.argmax(y_conv, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
# 完成训练和评估操作的定义

# 开始定义储存器操作并装载已经训练过的神经网络
saver = tf.train.Saver(write_version=tf.train.SaverDef.V1)
sess = tf.InteractiveSession()
saver.restore(sess, "cnnres/model.ckpt")
# sess.run(tf.global_variables_initializer())
# 完成定义储存器操作和装载神经网络

# 开始对神经网络进行输入测试
res = sess.run(y_conv, feed_dict={x: test_data, keep_prob: 1.0})
temp = np.argmax(res)
letter = chr(97+temp)
print('The test letter is '+letter)
# 完成测试