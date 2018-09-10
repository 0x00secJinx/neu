#!/usr/bin/python
from nmatrix import Matrix
from activation improt sigmoid, dsigmoid
from random import SystemRandom, uniform
import sys


"""
	A class that uses a reinforcement learning Neural Network/Artificial Intelligence
	algorithm to solve problems. Using Gradient decent and backpropogation, the
	network gets fined tuned to answering number based, non-linear problems. A classic example
	is the XOR problem. A single perceptron cannont solve this as it is a non-linear
	problem, but multiple perceptrons in a network can be trained to correctly answer these
	upto a 99% accuracy. (Note: 100% is technically possible but we need to account for the
	occasional errror). Provided in the comment at the bottom is a program to run this program
	using the XOR conditions.

from NNetwork import NeuralNetwork
import random

def main():

	# We need both the input and output to train the Neural Network
	# ^ denotes XOR
	# 0 ^ 0 = 0
	# 1 ^ 0 = 1
	# 0 ^ 1 = 1
	# 1 ^ 1 = 0

	training_data = [
	{
		'inputs': [0,0],
		'output': [0]
	},
	{
		'inputs': [0,1],
		'output': [1]
	},
	{
		'inputs': [1,0],
		'output': [1]
	},
	{
		'inputs': [1,1],
		'output': [0]
	}]

	nn = NeuralNetwork(2, 2, [16, 16], 1, 0.1)
	print("Before Training:")
	print("\t[1,1]: Output should be close to 0")
	print("\t\t" + str(nn.predict([1,1])))
	print("\n\t[1,0]: Output should be close to 1")
	print("\t\t"+ str(nn.predict([1,0])))
	print("\n\t[0,1]: Output should be close to 1")
	print("\t\t"+ str(nn.predict([0,1])))
	print("\n\t[0,0]: Output should be close to 0")
	print("\t\t"+ str(nn.predict([0,0])))

	secure_random = random.SystemRandom()

	for _ in range(10000):
		data = secure_random.choice(training_data)
		nn.train(data['inputs'], data['output'])

	print("After Training:")
	print("\t[1,1]: Output should be close to 0")
	print("\t\t" + str(nn.predict([1,1])))
	print("\n\t[1,0]: Output should be close to 1")
	print("\t\t"+ str(nn.predict([1,0])))
	print("\n\t[0,1]: Output should be close to 1")
	print("\t\t"+ str(nn.predict([0,1])))
	print("\n\t[0,0]: Output should be close to 0")
	print("\t\t"+ str(nn.predict([0,0])))

if __name__ == "__main__":
	main()
"""
