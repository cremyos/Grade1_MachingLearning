# _*_ coding: utf-8 _*_
import tensorflow as tf

# 配置神经网络的参数
INPUT_NODE = 784
OUTPUT_NODE = 10

IMAGE_SIZE = 28
NUM_CHANNELS = 1
NUM_LABELS = 10

# 第一个卷积层的尺寸和深度
CONV1_DEEP = 32
CONV1_SIZE = 5
# 第二个卷积层的尺寸和深度
CONV2_DEEP = 64
CONV2_SIZE = 5
# 全连接层的节点个数
FC_SIZE = 512

# 定义卷积神经网络的前向传播过程。这里添加了一个新的参数train，用于区别训练过程和测试过程。在这个程序中将用到dropout方法
# dropout可以进一步提升模型可靠性并防止过拟合（dropout过程只在训练时使用）
def inference(input_tensor, train, regularizer):
    with tf.variable_scope('layer1-conv1'):
        conv1_weights = tf.get_variable('weight', [CONV1_SIZE, CONV1_SIZE, NUM_CHANNELS, CONV1_DEEP],
                                        initializer=tf.truncated_normal_initializer(stddev=0.1))
        conv1_biases = tf.get_variable('bias', [CONV1_DEEP],
                                       initializer=tf.constant_initializer(0.0))
        conv1 = tf.nn.conv2d(input_tensor, conv1_weights, strides=[1, 1, 1, 1], padding='SAME')
        relu1 = tf.nn.relu(tf.nn.bias_add(conv1, conv1_biases))
    with tf.name_scope('visual') as v_s:
        # scale weights to [0 1], type is still float
        x_min = tf.reduce_min(conv1_weights)
        x_max = tf.reduce_max(conv1_weights)
        kernel_0_to_1 = (conv1_weights - x_min) / (x_max - x_min)
        # to tf.image_summary format [batch_size, height, width, channels]

        # this will display random 3 filters from the 64 in conv1
        # kernel_transposed = tf.transpose (kernel_0_to_1, [3, 0, 1, 2])
        # conv_W_img=tf.cond(tf.greater( filter_shape[2],1), lambda: tf.slice(kernel_transposed,[0,0,0,0],[-1,filter_shape[0],filter_shape[1],3]),lambda: kernel_transposed)
        kernel_transposed = tf.transpose(kernel_0_to_1, [3, 2, 0, 1])
        conv_W_img = tf.reshape(kernel_transposed, [-1, filter_shape[0], filter_shape[1], 1])
        tf.summary.image('conv_w', conv_W_img, max_outputs=filter_shape[3])
        feature_img = conv[0:1, :, :, 0:filter_shape[3]]
        feature_img = tf.transpose(feature_img, perm=[3, 1, 2, 0])
        tf.summary.image('feature', feature_img, max_outputs=filter_shape[3])

    with tf.name_scope('layer2-pool1'):
        pool1 = tf.nn.max_pool(relu1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')

    with tf.variable_scope('layer3-conv2'):
        conv2_weights = tf.get_variable('weight', [CONV2_SIZE, CONV2_SIZE, CONV1_DEEP, CONV2_DEEP],
                                        initializer=tf.truncated_normal_initializer(stddev=0.1))
        conv2_biases = tf.get_variable('bias', [CONV2_DEEP],
                                       initializer=tf.constant_initializer(0.0))
        conv2 = tf.nn.conv2d(pool1,conv2_weights, strides=[1, 1, 1, 1], padding='SAME')
        relu2 = tf.nn.relu(tf.nn.bias_add(conv2, conv2_biases))

    with tf.name_scope('layer4-pool2'):
        pool2 = tf.nn.max_pool(relu2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')

    pool2_shape = pool2.get_shape().as_list()
    nodes = pool2_shape[1] * pool2_shape[2] * pool2_shape[3]

    reshaped = tf.reshape(pool2, [pool2_shape[0], nodes])

    with tf.variable_scope('layer5-fc1'):
        fc1_weights = tf.get_variable('weight', [nodes, FC_SIZE],
                                      initializer=tf.truncated_normal_initializer(stddev=0.1))
        if regularizer != None:
            tf.add_to_collection('losses', regularizer(fc1_weights))
        fc1_biases = tf.get_variable('bias', [FC_SIZE],
                                     initializer=tf.constant_initializer(0.0))
        fc1 = tf.nn.relu(tf.matmul(reshaped, fc1_weights) + fc1_biases)
        if train:
            fc1 = tf.nn.dropout(fc1, 0.5)

    with tf.variable_scope('layer6-fc2'):
        fc2_weights = tf.get_variable('weight', [FC_SIZE, NUM_LABELS],
                                      initializer=tf.truncated_normal_initializer(stddev=0.1))
        if regularizer != None:
            tf.add_to_collection('losses', regularizer(fc2_weights))
        fc2_biases = tf.get_variable('bias', [NUM_LABELS],
                                     initializer=tf.constant_initializer(0.0))
        logit = tf.matmul(fc1, fc2_weights) + fc2_biases

    return logit