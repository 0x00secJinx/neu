#!/usr/bin/python
#from NNetwork import NeuralNetwork
from fixednn import NeuralNetwork
import random


def getWeightArray(matrix):
	weights = matrix.getCols()
	new_weights = []
	for i in range(len(weights)):
		new_weights.append(int((weights[i])+10))
	for i in range(len(weights)):
		if (new_weights[i] < 1):
			new_weights[i] = 1
	return new_weights

def main():
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

#	nn = NeuralNetwork(2, 1, [3], 1)
	nn = NeuralNetwork(2, 5, 1)
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

	for _ in range(20000):
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
