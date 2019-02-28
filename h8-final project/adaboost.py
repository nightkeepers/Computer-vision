import numpy as np
import struct
import matplotlib.pyplot as plt
import sys
import csv
from sklearn import ensemble
from sklearn.naive_bayes import GaussianNB 
from sklearn.tree import DecisionTreeClassifier
from PIL import Image
import os

path = sys.path[0]
# 训练集文件
train_images_idx3_ubyte_file = path+'/data/train-images-idx3-ubyte/train-images.idx3-ubyte'
# 训练集标签文件
train_labels_idx1_ubyte_file = path+'/data//train-labels-idx1-ubyte/train-labels.idx1-ubyte'

# 测试集文件
test_images_idx3_ubyte_file = path+'/data/t10k-images-idx3-ubyte/t10k-images.idx3-ubyte'
# 测试集标签文件
test_labels_idx1_ubyte_file = path+'/data/t10k-labels-idx1-ubyte/t10k-labels.idx1-ubyte'

correct = [
                 [[1,5,3,3,1,3,5,3],
                 [1,3,7,7,7,1,3,2,4,6,1],
                 [4,5,6,7,5,7,1,3,2,4,6,6,3,3,1,5,7,8]],

                 [[1,5,3,3,1,1,8,0],
                 [1,3,2,6,0,8,3,1,0,4,8],
                 [4,4,2,3,1,3,1,9,9,8,0,3,2,7,3,0,5,9],
                 [1,5,3,3,1,1,8,0],
                 [1,3,2,6,0,8,3,1,0,4,8],
                 [4,4,2,3,1,3,1,9,9,8,0,3,2,7,3,0,5,9],
                 [1,5,3,3,1,1,8,0],
                 [1,3,2,6,0,8,3,1,0,4,8],
                 [4,4,2,3,1,3,1,9,9,8,0,3,2,7,3,0,5,9]],

                 [[1,5,3,3,1,0,2,9],
                 [1,3,8,2,7,4,1,8,3,9,2],
                 [4,4,1,7,2,1,1,9,9,6,1,2,2,7,3,0,1,2],
                 [1,5,3,3,1,0,2,9],
                 [1,3,8,2,7,4,1,8,3,9,2],
                 [4,4,1,7,2,1,1,9,9,6,1,2,2,7,3,0,1,2],
                 [1,5,3,3,1,0,2,9],
                 [1,3,8,2,7,4,1,8,3,9,2],
                 [4,4,1,7,2,1,1,9,9,6,1,2,2,7,3,0,1,2]],

                 [[1,5,3,3,1,3,4,4],
                 [1,5,5,2,1,1,4,5,6,7,8],
                 [3,5,0,1,0,2,1,9,9,6,0,2,2,9,4,3,2,1],
                 [1,5,3,3,1,3,4,4],
                 [1,5,5,2,1,1,4,5,6,7,8],
                 [3,5,0,1,0,2,1,9,9,6,0,2,2,9,4,3,2,1],
                 [1,5,3,3,1,3,4,4],
                 [1,5,5,2,1,1,4,5,6,7,8],
                 [3,5,0,1,0,2,1,9,9,6,0,2,2,9,4,3,2,1]],

                 [[1,5,3,3,1,3,6,4],
                 [1,3,6,3,2,5,5,2,8,3,1],
                 [4,4,1,2,8,1,2,3,4,5,0,3,0,9,3,4,3,5],
                 [1,5,3,3,1,3,6,4],
                 [1,3,6,3,2,5,5,2,8,3,1],
                 [4,4,1,2,8,1,2,3,4,5,0,3,0,9,3,4,3,5],
                 [1,5,3,3,1,3,6,4],
                 [1,3,6,3,2,5,5,2,8,3,1],
                 [4,4,1,2,8,1,2,3,4,5,0,3,0,9,3,4,3,5]],

                 [[1,5,3,3,1,0,4,6],
                 [1,3,6,1,9,1,5,4,7,2,1],
                 [4,4,5,3,0,1,1,9,9,8,1,0,2,5,2,7,3,4],
                 [1,5,3,3,1,0,4,6],
                 [1,3,6,1,9,1,5,4,7,2,1],
                 [4,4,5,3,0,1,1,9,9,8,1,0,2,5,2,7,3,4],
                 [1,5,3,3,1,0,4,6],
                 [1,3,6,1,9,1,5,4,7,2,1],
                 [4,4,5,3,0,1,1,9,9,8,1,0,2,5,2,7,3,4]],

                 [[1,5,3,3,1,3,5,1],
                 [1,3,7,1,9,2,7,4,5,0,6],
                 [4,4,5,2,0,2,2,0,0,0,0,1,0,1,0,0,5,8],
                 [1,5,3,3,1,3,5,1],
                 [1,3,7,1,9,2,7,4,5,0,6],
                 [4,4,5,2,0,2,2,0,0,0,0,1,0,1,0,0,5,8],
                 [1,5,3,3,1,3,5,1],
                 [1,3,7,1,9,2,7,4,5,0,6],
                 [4,4,5,2,0,2,2,0,0,0,0,1,0,0,5,8]],

                 [[1,5,3,3,1,0,5,2],
                 [1,5,5,6,0,5,6,3,9,8,3],
                 [3,5,0,4,2,6,1,9,9,7,1,2,2,2,1,0,1,2],
                 [1,5,3,3,1,0,5,2],
                 [1,5,5,6,0,5,6,3,9,8,3],
                 [3,5,0,4,2,6,1,9,9,7,1,2,2,2,1,0,1,2],
                 [1,5,3,3,1,0,5,2],
                 [1,5,5,6,0,5,6,3,9,8,3],
                 [3,5,0,4,2,6,1,9,9,7,1,2,2,2,1,0,1,2]],

                 [[1,5,3,3,1,3,4,8],
                 [1,5,6,2,6,4,1,1,4,1,8],
                 [4,4,0,3,0,2,1,9,9,6,1,0,0,2,2,2,1,8],
                 [1,5,3,3,1,3,4,8],
                 [1,5,6,2,6,4,1,1,4,1,8],
                 [4,4,0,3,0,2,1,9,9,6,1,0,0,2,2,2,1,8],
                 [1,5,3,3,1,3,4,8],
                 [1,5,6,2,6,4,1,1,4,1,8],
                 [4,4,0,3,0,2,1,9,9,6,1,0,0,2,2,2,1,8]],

                 [[1,5,3,3,1,3,4,7],
                 [1,8,9,5,0,1,8,2,3,2,3],
                 [3,5,0,1,0,5,1,9,9,7,0,5,0,4,2,3,1,4],
                 [1,5,3,3,1,3,4,7],
                 [1,8,9,5,0,1,8,2,3,2,3],
                 [3,5,0,1,0,5,1,9,9,7,0,5,0,4,2,3,1,4],
                 [1,5,3,3,1,3,4,7],
                 [1,8,9,5,0,1,8,2,3,2,3],
                 [3,5,0,1,0,5,1,9,9,7,0,5,0,4,2,3,1,4]]
]


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


def preprocessImg(img1):
    h = len(img1)
    w = len(img1[0])
    ratio = 0.0
    if w > h:
        ratio = w / 28.0
    else:
        ratio = h / 28.0
    newimg = []
    for j in range(28):
        row = []
        for k in range(28):
            x = int((h-1)/2.0+(j - 13.5) * ratio)
            y = int((w-1)/2.0+(k - 13.5) * ratio)
            if x < 0 or x >= h or y < 0 or y >= w:
                row.append(0)                    
            else:
                row.append(img1[x][y])
        newimg.append(row)
    newimg2 = np.array(newimg)
    return newimg2

def cutImg(img):
    h = len(img)
    w = len(img[0])

    newimg = []
    img1 = []
    img2 = []
    for j in range(h):
        row1 = []
        row2 = []
        for k in range(w):
            if k <= w/2:
                row1.append(img[j][k])
            else:
                row2.append(img[j][k])
        img1.append(row1)
        img2.append(row2)
    newimg.append(np.array(img1))
    newimg.append(np.array(img2))
    return newimg

def AdaBoosting():
    train_images = load_train_images()
    train_labels = load_train_labels()
    test_images = load_test_images()
    test_labels = load_test_labels()
    train_images = np.round(train_images/256)
    test_images = np.round(test_images/256)
    nsamples, nx, ny = train_images.shape
    train_images2 = train_images.reshape((nsamples,nx*ny))
    clf = ensemble.AdaBoostClassifier(base_estimator=DecisionTreeClassifier(max_depth=2), learning_rate = 0.1)
    #clf = ensemble.AdaBoostClassifier( learning_rate = 0.1)
    clf.fit(train_images2, train_labels)

    nsamples, nx, ny = test_images.shape
    test_images2 = test_images.reshape((nsamples,nx*ny))
    predict_labels = clf.predict(test_images2)
    accuracy = np.mean(predict_labels==test_labels)*100
    print("the accuracy of AdaBoost model is {0}%".format(accuracy))
    myImgs = []
    list1 = os.listdir(path+"/Data2")
    #list1 =sorted(list1)
    print(list1)

    imgN = 0
    rowN = 0
    prowN = 0
    rows = []
    ws = []
    for i in range(0,len(list1)):
        if prowN != int(list1[i][2]):
            if len(rows)>=3:
                
                print(imgN," ",rowN," ", len(rows), " ",len(correct[imgN][rowN]))
                while len(rows) <  len(correct[imgN][rowN]):
                    maxW = 0
                    maxI = 0
                    for j in range(len(rows)):
                        tw = len(rows[j][0])
                        if tw > maxW:
                            maxW = tw
                            maxI = j
                    twoimg = cutImg(rows[maxI])
                    rows2 = rows
                    for j in range(len(rows)):
                        if j == maxI:
                            rows.append(twoimg[0])
                            rows.append(twoimg[1])
                        else:
                            rows.append(rows2[j])
                cnt = 0
                for q in rows:
                    if cnt>= len(correct[imgN][rowN]):
                        break
                    cnt+=1
                    myImgs.append(preprocessImg(q))
            else:
                rowN-=1
            rowN+=1
            prowN = int(list1[i][2])
            if prowN == 0:
                rowN = 0
            rows = []
            ws = []
        imgN=int(list1[i][0])
        if rowN >= len(correct[imgN]):
            continue

        img1 = np.array(Image.open(path+"/Data2/"+list1[i]).convert('L'), 'f')
        rows.append(img1)
        #plt.imshow(newimg2, cmap='gray')
        #plt.show()


    if len(rows)>=3:
        while len(rows) <  len(correct[imgN][rowN]):
            maxW = 0
            maxI = 0
            for j in range(len(rows)):
                tw = len(rows[j][0])
                if tw > maxW:
                    maxW = tw
                    maxI = j
            twoimg = cutImg(rows[maxI])
            rows2 = rows
            for j in range(len(rows)):
                if j == maxI:
                    rows.append(twoimg[0])
                    rows.append(twoimg[1])
                else:
                    rows.append(rows2[j])

        for q in rows:
            myImgs.append(preprocessImg(q))

    myImgs = np.array(myImgs)
    nsamples, nx, ny = myImgs.shape
    myImgs2 = myImgs.reshape((nsamples,nx*ny))
    predict_label = clf.predict(myImgs2)
    cnt = 0
    sum2 = 0
    writer = csv.writer(open(path+'/result.csv', 'w+', newline=''))
    row = []
    for i in range(len(correct)):
        for j in range(len(correct[i])):
            row = []
            for k in range(len(correct[i][j])):
                row.append(predict_label[cnt])
                if int(predict_label[cnt]) == correct[i][j][k]:
                    sum2+=1.0
                cnt+=1
            writer.writerow(row)
        writer.writerow([str(i)+'th image'])
    if cnt == len(predict_label):
        print("good")
    print(sum2/cnt)








def run():
    train_images = load_train_images()
    train_labels = load_train_labels()
    test_images = load_test_images()
    test_labels = load_test_labels()
    """for i in range(len(train_images)):
        for j in range(len(train_images[i])):
            for k in range(len(train_images[i][j])):
                train_images[i][j][k] = round(train_images[i][j][k]/256)"""

    # 查看前十个数据及其标签以读取是否正确
    for i in range(10):
        print(train_labels[i])
        print(train_images[i])
        plt.imshow(train_images[i], cmap='gray')
        plt.show()
    print('done')

if __name__ == '__main__':
    #run()
    AdaBoosting()
