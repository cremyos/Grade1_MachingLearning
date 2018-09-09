# -- coding: UTF-8 --
# Name: Variable.py
# Date: 2018.05.04
# Author: Lniper
# Aim: 变量定义、初始化:
#       1）定义三个变量：a,b,c,d。定义其初始化值initial_value均为0~9的随机正整数。
#       2）将a,b添加入key为'init'的collection中。并只初始化'init'这个集合中的变量。
#       3）此时fetch变量c, 观察出错报告并截图。找到出错原因及对应错误代号。
#       4）使用try-except 异常处理方法，收集未被初始化的变量，并将未初始化的变量再次初始化。
#       要求：不能用tf.global_variables_initializer()，按照步骤完成。提交代码和运行截图。
#       提示：
#       1. tf的异常一般表示为tf.errors.xxxx
#       2. tf.add_to_collection(name,v) 将变量v1添加入key名为name的集合中，c = tf.get_collection(name)获取key名为name的集合。

import tensorflow as tf
from numpy import *

a = tf.Variable(initial_value = random.randint(10), name = 'Variable_a')
b = tf.Variable(initial_value = random.randint(10), name = 'Variable_b')
c = tf.Variable(initial_value = random.randint(10), name = 'Variable_c')
d = tf.Variable(initial_value = random.randint(10), name = 'Variable_d')

name = 'init'

tf.add_to_collection(name, a)
tf.add_to_collection(name, b)

print("Result: ")
with tf.Session() as sess:

	# 使用未能初始化的变量
	# OutPutC = sess.run([c])
	# print('OutPutC = ', OutPutC)

	name1 = 'UninitVariable'
	# 使用try-except机制处理未初始化的变量
	for variable in tf.global_variables():
		try:
			sess.run(variable)
		except tf.errors.FailedPreconditionError:
			tf.add_to_collection(name1, variable)

	# 收集未能初始化的变量
	UninitVariable = tf.get_collection(name1)
	InitVariable = tf.variables_initializer(UninitVariable)
	Res = sess.run(InitVariable)

	print("Init Variable")
	print(sess.run(a))
	print(sess.run(b))
	print(sess.run(c))
	print(sess.run(d))
