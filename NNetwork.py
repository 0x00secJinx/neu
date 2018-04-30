#!/usr/bin/python
from nmatrix import Matrix
from activation import sigmoid, dsigmoid
import sys
import pdb

"""
TO DO:

	Have a matrix to store each hidden answer so backpropogation can work
"""

class NeuralNetwork(object):

	def __init__(self, input_nodes, hidden_layers , hidden_nodes, output_nodes):
		self.i_nodes = input_nodes
		self.h_layers = hidden_layers
		self.h_nodes_count = hidden_nodes
		self.h_outputs = []
		self.h_weights = []
		self.h_bias = []
		self.h_errors = []
		self.o_nodes = output_nodes

		if(len(self.h_nodes_count) != self.h_layers):
			print("Could not init Neural Network\nHidden layers arg do not match length of hidden nodes")
			sys.exit(1)

		self.ih_weights = Matrix(self.h_nodes_count[0], self.i_nodes)
		for i in range(self.h_layers-1):
			self.h_weights.append(Matrix(self.h_nodes_count[i+1], self.h_nodes_count[i]))

		self.ho_weights = Matrix(self.o_nodes, self.h_nodes_count[self.h_layers-1])

		for i in range(self.h_layers):
			#print(self.h_nodes_count[i])
			self.h_bias.append(Matrix(self.h_nodes_count[i], 1))
		for i in range(self.h_layers-1):
			self.h_weights[i].randomize()
			#self.h_weights[i].printMatrix()
			#print("")
		self.ih_weights.randomize()
		self.ho_weights.randomize()
		for i in self.h_bias:
			i.randomize()

		self.o_bias = Matrix(self.o_nodes, 1)
		self.o_bias.randomize()
		self.learning_rate = 0.1

	def feedforward(self, input_array):

		# Generate the hidden outputs
		inputs = Matrix.inputFromArray(input_array)
		hidden = self.ih_weights.matrixProduct(inputs)
		hidden.matrixAdd(self.h_bias[0])
		hidden.map(sigmoid)
		self.h_outputs.append(hidden)

		for i in range(len(self.h_weights)):
			self.h_outputs.append(self.h_weights[i].matrixProduct(self.h_outputs[i]))
			self.h_outputs[i+1].matrixAdd(self.h_bias[i+1])
			self.h_outputs[i+1].map(sigmoid)

		output = self.ho_weights.matrixProduct(self.h_outputs[len(self.h_outputs)-1])
		output.matrixAdd(self.o_bias)
		output.map(sigmoid)

		self.h_outputs = []

		return output.matrixToArray()

	def train(self, input_array, target_array):

		self.h_outputs = []

		#pdb.set_trace()

		inputs = Matrix.inputFromArray(input_array)
		hidden = self.ih_weights.matrixProduct(inputs)
		hidden.matrixAdd(self.h_bias[0])
		hidden.map(sigmoid)
		self.h_outputs.append(hidden)

		for i in range(len(self.h_weights)):
			self.h_outputs.append(self.h_weights[i].matrixProduct(self.h_outputs[i]))
			self.h_outputs[i+1].matrixAdd(self.h_bias[i+1])
			self.h_outputs[i+1].map(sigmoid)

		output = self.ho_weights.matrixProduct(self.h_outputs[len(self.h_outputs)-1])
		output.matrixAdd(self.o_bias)
		output.map(sigmoid)
		#output.printMatrix()

		################################
		# Last hidden to output
		################################
		# Calculate the output errors
		targets = Matrix.targetsFromArray(target_array)
		output_errors = Matrix.matrixSubtract(targets, output)
		# Gradients for output to hidden
		gradients = Matrix.staticMap(output, dsigmoid)
		gradients.matrixMultiply(output_errors)
		gradients.multiply(self.learning_rate)
		# Get the output of the last hidden layer and transpose
		hidden_output_T = self.h_outputs[len(self.h_outputs)-1].transposeMatrix()
		deltas = gradients.matrixProduct(hidden_output_T)
		self.ho_weights.matrixAdd(deltas)
		self.o_bias.matrixAdd(gradients)

		################################
		# Hidden Layers
		################################
		for i in range(self.h_layers-1):
			#pdb.set_trace()
			if(i == self.h_layers-1):
				break
			print("Here")
			if(i == 0):
				hw_t = self.ho_weights.transposeMatrix()
			else:
				hw_t = self.h_weights[i-len(self.h_weights)-1]
			hidden_errors = hw_t.matrixProduct(output_errors)
			hidden_gradient = Matrix.staticMap(self.h_outputs[i-len(self.h_outputs)+1], dsigmoid)
			hidden_gradient.matrixMultiply(hidden_errors)
			hidden_gradient.multiply(self.learning_rate)

		sys.exit()




	def predict(self, input_array):
		output = self.feedforward(input_array)
		return output
