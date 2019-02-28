import numpy as np
import struct
import matplotlib.pyplot as plt
import sys
from sklearn import ensemble
from sklearn.naive_bayes import GaussianNB 
from PIL import Image

path = sys.path[0]
# 训练集文件
train_images_idx3_ubyte_file = path+'/data/train-images-idx3-ubyte/train-images.idx3-ubyte'
# 训练集标签文件
train_labels_idx1_ubyte_file = path+'/data//train-labels-idx1-ubyte/train-labels.idx1-ubyte'

# 测试集文件
test_images_idx3_ubyte_file = path+'/data/t10k-images-idx3-ubyte/t10k-images.idx3-ubyte'
# 测试集标签文件
test_labels_idx1_ubyte_file = path+'/data/t10k-labels-idx1-ubyte/t10k-labels.idx1-ubyte'


def decode_idx3_ubyte(idx3_ubyte_file):
    # 读取二进制数据
    bin_data = open(idx3_ubyte_file, 'rb').read()

    # 解析文件头信息，依次为魔数、图片数量、每张图片高、每张图片宽
    offset = 0
    fmt_header = '>iiii'
    magic_number, num_images, num_rows, num_cols = struct.unpack_from(fmt_header, bin_data, offset)
    print('魔数:%d, 图片数量: %d张, 图片大小: %d*%d' % (magic_number, num_images, num_rows, num_cols))

    # 解析数据集
    image_size = num_rows * num_cols
    offset += struct.calcsize(fmt_header)
    fmt_image = '>' + str(image_size) + 'B'
    images = np.empty((num_images, num_rows, num_cols))
    for i in range(num_images):
        if (i + 1) % 10000 == 0:
            print('已解析 %d' % (i + 1) + '张')
        images[i] = np.array(struct.unpack_from(fmt_image, bin_data, offset)).reshape((num_rows, num_cols))
        offset += struct.calcsize(fmt_image)
    return images


def decode_idx1_ubyte(idx1_ubyte_file):
    # 读取二进制数据
    bin_data = open(idx1_ubyte_file, 'rb').read()

    # 解析文件头信息，依次为魔数和标签数
    offset = 0
    fmt_header = '>ii'
    magic_number, num_images = struct.unpack_from(fmt_header, bin_data, offset)
    print ('魔数:%d, 图片数量: %d张' % (magic_number, num_images))

    # 解析数据集
    offset += struct.calcsize(fmt_header)
    fmt_image = '>B'
    labels = np.empty(num_images)
    for i in range(num_images):
        if (i + 1) % 10000 == 0:
            print('已解析 %d' % (i + 1) + '张')
        labels[i] = struct.unpack_from(fmt_image, bin_data, offset)[0]
        offset += struct.calcsize(fmt_image)
    return labels


def load_train_images(idx_ubyte_file=train_images_idx3_ubyte_file):
    return decode_idx3_ubyte(idx_ubyte_file)


def load_train_labels(idx_ubyte_file=train_labels_idx1_ubyte_file):
    return decode_idx1_ubyte(idx_ubyte_file)


def load_test_images(idx_ubyte_file=test_images_idx3_ubyte_file):
    return decode_idx3_ubyte(idx_ubyte_file)


def load_test_labels(idx_ubyte_file=test_labels_idx1_ubyte_file):
    return decode_idx1_ubyte(idx_ubyte_file)




def run():
    train_images = load_train_images()
    train_labels = load_train_labels()
    test_images = load_test_images()
    test_labels = load_test_labels()

    # 查看前十个数据及其标签以读取是否正确
    for i in range(10):
        print(train_labels[i])
        print(train_images[i])
        plt.imshow(train_images[i], cmap='gray')
        plt.show()
    print('done')


def AdaBoosting():
    train_images = load_train_images()
    train_labels = load_train_labels()
    test_images = load_test_images()
    test_labels = load_test_labels()
    nsamples, nx, ny = train_images.shape
    train_images2 = train_images.reshape((nsamples,nx*ny))
    clf = ensemble.AdaBoostClassifier()
    clf.fit(train_images2, train_labels)

    nsamples, nx, ny = test_images.shape
    test_images2 = test_images.reshape((nsamples,nx*ny))
    predict_labels = clf.predict(test_images2)
    accuracy = np.mean(predict_labels==test_labels)*100
    print("the accuracy of AdaBoost model is {0}%".format(accuracy))

    myImg = np.array(Image.open(path+"/1.jpg").convert('L'), 'f')
    myImg2 = np.array(Image.open(path+"/2.jpg").convert('L'), 'f')
    myImgs = []
    myImgs.append(myImg)
    myImgs.append(myImg2)
    myImgs = np.array(myImgs)
    plt.imshow(myImgs[0], cmap='gray')
    plt.show()
    nsamples, nx, ny = myImgs.shape
    myImgs2 = myImgs.reshape((nsamples,nx*ny))
    predict_label = clf.predict(myImgs2)
    print(predict_label)



if __name__ == '__main__':
    #run()
    AdaBoosting()
