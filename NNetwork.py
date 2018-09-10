#!/usr/bin/python
from nmatrix import Matrix
#from activation import sigmoid, dsigmoid, relu, drelu
from act import sigmoid, sigmoid, tanh, dtanh
import sys

class NeuralNetwork(object):

	def __init__(self, input_nodes, hidden_layers, hidden_nodes, output_nodes, learning_rate):

		self.i_nodes = input_nodes
		self.h_layers = hidden_layers
		self.h_nodes_count = hidden_nodes
		self.h_outputs = []
		self.h_weights = []
		self.h_bias = []
		self.o_nodes = output_nodes

		if(len(self.h_nodes_count) != self.h_layers):
			print("Could not init Neural Network\nHidden layers arg do not match length of hidden nodes")
			sys.exit(1)

		self.ih_weights = Matrix(self.h_nodes_count[0], self.i_nodes)
		for i in range(self.h_layers-1):
			self.h_weights.append(Matrix(self.h_nodes_count[i+1], self.h_nodes_count[i]))

		self.ho_weights = Matrix(self.o_nodes, self.h_nodes_count[self.h_layers-1])

		for i in range(self.h_layers):
			self.h_bias.append(Matrix(self.h_nodes_count[i], 1))
		for i in range(self.h_layers-1):
			self.h_weights[i].randomize()

		self.ih_weights.randomize()
		self.ho_weights.randomize()
		for i in self.h_bias:
			i.randomize()

		self.o_bias = Matrix(self.o_nodes, 1)
		self.o_bias.randomize()
		self.learning_rate = learning_rate

	def feedforward(self, input_array):

		self.h_outputs = []

		# Generate the hidden outputs
		inputs = Matrix.inputFromArray(input_array)
		hidden = self.ih_weights.matrixProduct(inputs)
		hidden.matrixAdd(self.h_bias[0])
		hidden.map(relu)
		self.h_outputs.append(hidden)

		for i in range(len(self.h_weights)):
			tmp = self.h_weights[i].matrixProduct(self.h_outputs[i])
			tmp.matrixAdd(self.h_bias[i+1])
			tmp.map(relu)
			self.h_outputs.append(tmp)

		output = self.ho_weights.matrixProduct(self.h_outputs[len(self.h_outputs)-1])
		output.matrixAdd(self.o_bias)
		output.map(relu)

		self.h_outputs = []

		return output.matrixToArray()

	def train(self, input_array, target_array):

		self.h_outputs = []

		inputs = Matrix.inputFromArray(input_array)
		hidden = self.ih_weights.matrixProduct(inputs)
		hidden.matrixAdd(self.h_bias[0])
		hidden.map(relu)
		self.h_outputs.append(hidden)

		for i in range(len(self.h_weights)):
			tmp = self.h_weights[i].matrixProduct(self.h_outputs[i])
			tmp.matrixAdd(self.h_bias[i+1])
			tmp.map(relu)
			self.h_outputs.append(tmp)

		output = self.ho_weights.matrixProduct(self.h_outputs[len(self.h_outputs)-1])
		output.matrixAdd(self.o_bias)
		output.map(relu)

		targets = Matrix.targetsFromArray(target_array)
		output_errors = Matrix.matrixSubtract(targets, output)
		gradients = Matrix.staticMap(output, drelu)
		gradients.matrixMultiply(output_errors)
		gradients.multiply(self.learning_rate)
		hidden_output_T = self.h_outputs[len(self.h_outputs)-1].transposeMatrix()
		deltas = gradients.matrixProduct(hidden_output_T)
		self.ho_weights.matrixAdd(deltas)
		self.o_bias.matrixAdd(gradients)

		for i in range(self.h_layers-1):
			if(i == 0):
				hw_t = self.ho_weights.transposeMatrix()
				hidden_errors = hw_t.matrixProduct(output_errors)
			else:
				hw_t = self.h_weights[len(self.h_weights)-i].transposeMatrix()
				hidden_errors = hw_t.matrixProduct(hidden_errors)
			hidden_gradient = Matrix.staticMap(self.h_outputs[len(self.h_outputs)-i-1], drelu)
			hidden_gradient.matrixMultiply(hidden_errors)
			hidden_gradient.multiply(self.learning_rate)
			hidden_T = self.h_outputs[len(self.h_outputs)-i-2].transposeMatrix()
			weight_h_deltas = hidden_gradient.matrixProduct(hidden_T)
			self.h_weights[len(self.h_weights)-i-1].matrixAdd(weight_h_deltas)
			self.h_bias[len(self.h_bias)-i-1].matrixAdd(hidden_gradient)

		if(self.h_layers == 1):
			hw_t = self.ho_weights.transposeMatrix()
			hidden_errors = hw_t.matrixProduct(output_errors)
		else:
			hw_t = self.h_weights[0].transposeMatrix()
			hidden_errors = hw_t.matrixProduct(hidden_errors)
		hidden_gradient = Matrix.staticMap(self.h_outputs[0], drelu)
		hidden_gradient.matrixMultiply(hidden_errors)
		hidden_gradient.multiply(self.learning_rate)
		inputs_T = inputs.transposeMatrix()
		weight_ih_deltas = hidden_gradient.matrixProduct(inputs_T)
		self.ih_weights.matrixAdd(weight_ih_deltas)
		self.h_bias[0].matrixAdd(hidden_gradient)


	def predict(self, input_array):
		output = self.feedforward(input_array)
		return output
