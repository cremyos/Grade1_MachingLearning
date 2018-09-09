import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data

learning_rate = 0.001
batch_size = 20


def inference(input, vgg_type):
    # 28*28
    layer1 = conv_layer(input, 1, 64)
    layer2 = conv_layer(layer1, 64, 64)
    layer3 = tf.nn.max_pool(layer2, [1, 2, 2, 1], [1, 2, 2, 1], 'VALID')
    # 14*14
    layer4 = conv_layer(layer3, 64, 128)
    layer5 = conv_layer(layer4, 128, 128)
    layer6 = tf.nn.max_pool(layer5, [1, 2, 2, 1], [1, 2, 2, 1], 'VALID')
    # 7*7
    layer7 = conv_layer(layer6, 128, 256)
    layer8 = conv_layer(layer7, 256, 256)
    layer9 = conv_layer(layer8, 256, 256)
    # layer10 = tf.nn.max_pool(layer9, [1, 2, 2, 1], [1, 2, 2, 1], 'NONE')
    layer10 = fc_layer(tf.reshape(layer9, [-1, 7 * 7 * 256]), 7 * 7 * 256, 4096)
    layer11 = fc_layer(layer10, 4096, 10)

    # w = tf.Variable(tf.random_normal([28 * 28, 10]))
    # b = tf.Variable(tf.constant(0, dtype=tf.float32, shape=[10]))
    # layer = tf.nn.relu(tf.matmul(tf.reshape(input, [-1, 28 * 28]), w) + b)

    return layer11


def fc_layer(input, deep_pre, deep_now):
    w = tf.Variable(tf.random_normal([deep_pre, deep_now]))
    b = tf.Variable(tf.constant(0, dtype=tf.float32, shape=[deep_now]))
    layer = tf.nn.relu(tf.matmul(input, w) + b)
    return layer


def conv_layer(input, deep_pre, deep_now):
    w = tf.Variable(tf.random_normal([3, 3, deep_pre, deep_now]))
    b = tf.Variable(tf.constant(0, dtype=tf.float32, shape=[deep_now]))
    layer = tf.nn.relu(tf.nn.conv2d(input, w, [1, 1, 1, 1], padding='SAME') + b)
    return layer


def loss(real, pred):
    # lossv = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=real))
    lossv = tf.reduce_mean(tf.norm(real - pred, axis=1) ** 2) / 2
    return lossv


def train_optimazer(lossv, global_step=None):
    train_step = tf.train.GradientDescentOptimizer(learning_rate).minimize(lossv)
    return train_step


def train():
    mnist = input_data.read_data_sets('MNIST_data/', one_hot=True)
    x = tf.placeholder(tf.float32, [None, 28 * 28 * 1])
    y = tf.placeholder(tf.float32, [None, 10])
    x_image = tf.reshape(x, [-1, 28, 28, 1])
    net = inference(x_image, '1')
    loss_v = loss(net, y)
    train = train_optimazer(loss_v)
    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        for step in range(10000):
            train_data, train_lable = mnist.train.next_batch(batch_size)
            loss_value, _ = sess.run([loss_v, train], feed_dict={x: train_data, y: train_lable})
            if step % 100 == 0:
                print('step %d loss is %g' % (step, loss_value))
        print('after training, loss is %g' % (loss_value))


train()