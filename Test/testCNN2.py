import numpy as np
from PIL import Image
import xlsxwriter


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


# 循环读取测试图片并写入
# 开始进行写excel的准备
book = xlsxwriter.Workbook(r'train_set.xlsx')
sheet1 = book.add_worksheet('train_input1')
sheet2 = book.add_worksheet('train_input2')
sheet3 = book.add_worksheet('train_input3')
sheet4 = book.add_worksheet('train_input4')
# 完成写excel的准备
for i in range(1, 1430+1):
    test_pic = ImageToMatrix(str(i)+'.png')
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
    test_datap = ImageToMatrix2(pic2)
    test_data = np.reshape(test_datap, (1, 784))
# 完成图片尺寸转换
# 对行向量进行储存
    for j in range(0, 200):
        sheet1.write(i-1, j, test_data[0, j])
        sheet2.write(i - 1, j, test_data[0, j+200])
        sheet3.write(i - 1, j, test_data[0, j+400])
        if j+600 <= 783:
            sheet4.write(i - 1, j, test_data[0, j+600])
    print(i)
book.close()