import tensorflow as tf

welcome = tf.constant('Hello I am Lniper!')
sess = tf.Session()
print(sess.run(welcome))