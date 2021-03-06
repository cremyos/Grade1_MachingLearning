# -- coding: UTF-8 --
# Name: main.py
# Date: 2018.07.10
# Author: Lniper
# Aim:  使用Guided Grad-CAM算法对训练好的分类器网络进行可视化评估，
#       深入理解梯度、反向传播、网络特征等物理含义，了解Tensorflow中采用自定义方法修改梯度等计算的过程。
#       要求：对已训练好的，用于MNIST分类的Lenet网络进行效果可视化，0-9类分别选一张图片进行该类型的效果展示。提交源码及可视化结果图片。
#


from vgg import Myvgg16
import tensorflow as tf
import numpy as np
from skimage import io
from skimage.transform import resize
from matplotlib import pyplot as plt
from imagenet_classes import class_names
from scipy.misc import imread, imresize

flags = tf.app.flags
flags.DEFINE_string("input", "3.png", "Path to input image ['3.png']")
flags.DEFINE_string("output", "3_save.png", "Path to input image ['3_save.png']")
flags.DEFINE_string("layer_name", "pool5", "Layer till which to backpropagate ['pool5']")

FLAGS = flags.FLAGS


def load_image(img_path):
	print("Loading image")
	img = imread(img_path, mode='RGB')
	img = imresize(img, (224, 224))
	# Converting shape from [224,224,3] tp [1,224,224,3]
	x = np.expand_dims(img, axis=0)
	# Converting RGB to BGR for VGG
	x = x[:,:,:,::-1]
	return x, img


def grad_cam(x, vgg, sess, predicted_class, layer_name, nb_classes):
	print("Setting gradients to 1 for target class and rest to 0")
	# Conv layer tensor [?,7,7,512]
	conv_layer = vgg.layers[layer_name]
	# [1000]-D tensor with target class index set to 1 and rest as 0
	one_hot = tf.sparse_to_dense(predicted_class, [nb_classes], 1.0)
	signal = tf.multiply(vgg.layers['fc3'], one_hot)
	loss = tf.reduce_mean(signal)

	grads = tf.gradients(loss, conv_layer)[0]
	# Normalizing the gradients
	norm_grads = tf.div(grads, tf.sqrt(tf.reduce_mean(tf.square(grads))) + tf.constant(1e-5))

	output, grads_val = sess.run([conv_layer, norm_grads], feed_dict={vgg.imgs: x})
	output = output[0]           # [7,7,512]
	grads_val = grads_val[0]	 # [7,7,512]

	weights = np.mean(grads_val, axis = (0, 1)) 			# [512]
	cam = np.ones(output.shape[0 : 2], dtype = np.float32)	# [7,7]

	# Taking a weighted average
	for i, w in enumerate(weights):
	    cam += w * output[:, :, i]

	# Passing through ReLU
	cam = np.maximum(cam, 0)
	cam = cam / np.max(cam)
	cam = resize(cam, (224,224))

	# Converting grayscale to 3-D
	cam3 = np.expand_dims(cam, axis=2)
	cam3 = np.tile(cam3,[1,1,3])

	return cam3


def main(_):
	x, img = load_image(FLAGS.input)

	sess = tf.Session()

	print("Loading Vgg")
	imgs = tf.placeholder(tf.float32, [None, 224, 224, 3])
	vgg = Myvgg16(imgs, 'vgg16_weights.npz', sess)

	print("Feedforwarding")
	prob = sess.run(vgg.probs, feed_dict={vgg.imgs: x})[0]
	preds = (np.argsort(prob)[::-1])[0:5]


	# Target class
	predicted_class = preds[0]
	# Target layer for visualization
	layer_name = FLAGS.layer_name
	# Number of output classes of model being used
	nb_classes = 1000

	cam3 = grad_cam(x, vgg, sess, predicted_class, layer_name, nb_classes)

	img = img.astype(float)
	img /= img.max()

	# Superimposing the visualization with the image.
	new_img = img+3*cam3
	new_img /= new_img.max()

	# Display and save
	io.imshow(new_img)
	plt.show()
	io.imsave(FLAGS.output, new_img)

if __name__ == '__main__':
	tf.app.run()

