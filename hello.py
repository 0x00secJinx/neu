#!/usr/bin/python
from mnist import MNIST
from fixednn import NeuralNetwork
from random import randrange, SystemRandom
from time import sleep
import sys
from datetime import datetime

mndata = MNIST("./")
images, label = mndata.load_training()
test_img, test_label = mndata.load_testing()


def createLabelArray(expected):
	arr = []
	for i in range(10):
		if(i != expected):
			arr.append(0.0)
		elif(i == expected):
			arr.append(1.0)
	return arr


def getPrediction(arr):
	highest = 0
	print(arr)
	for i in range(1, len(arr)):
		if arr[i] > arr[highest]:
			highest = i

	return highest


nn = NeuralNetwork(784, 100, 10)

print("Training...")
a = datetime.now()
#for i in range(2500):
for i in range(len(images)):
	sys.stdout.write(str(i) + '\r')
	sys.stdout.flush()
	label_arr = createLabelArray(label[i])
	image_arr = []
	for j in range(len(images[i])):
		image_arr.append(float(images[i][j]/255.0))
	nn.train(image_arr, label_arr)

b = datetime.now()


number_correct = 0.0
for i in range(len(test_img)):
	img_arr = []
	for j in range(len(test_img[i])):
		img_arr.append(float(images[i][j]/255.0))
	predicted = nn.predict(img_arr)
	prediction = getPrediction(predicted)
	print("The expected output is %d\nPrediction: %d\n" % (test_label[i],prediction))
	if (test_label[i] == prediction):
		number_correct += 1
print("Percent Correct: %.2f" % (number_correct/len(test_img)))

print("Time to train: " + str(b-a))
