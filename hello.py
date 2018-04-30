#!/usr/bin/python
from mnist import MNIST
from NNetwork import NeuralNetwork
from random import randrange, SystemRandom
from time import sleep
import sys

mndata = MNIST("./")
images, label = mndata.load_training()

def createLabelArray(expected):
	arr = []
	for i in range(0,10):
		if(i != expected):
			arr.append(0.0)
		elif(i == expected):
			arr.append(1.0)
	return arr


def getPrediction(arr):
	highest = arr[0]
	highest_i = 0
	for i in range(len(arr)):
		if arr[i] > highest:
			highest = arr[i]
			highest_i = i

	return highest_i


nn = NeuralNetwork(784, 250, 10)
secure_random = SystemRandom()
for i in range(0,1000):
	sys.stdout.write(str(i) + '\r')
	sys.stdout.flush()
	index = secure_random.randrange(0,len(images))
	label_arr = createLabelArray(label[index])
	image_arr = []
	for i in range(len(images[index])):
		image_arr.append(double(images[index][i]/256.0))
	nn.train(image_arr, label_arr)

number_correct = 0.0
for i in range(200):
	index = secure_random.randrange(0, len(images))
	predicted = nn.predict(images[index])
	print("The expected output is %d\nPrediction: %d\n" % (label[index],getPrediction(predicted)))
	if (label[index] == getPrediction(predicted)):
		number_correct += 1
print("Percent Correct: %.2f" % (number_correct/200))
