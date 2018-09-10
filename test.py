#!/usr/bin/python
from nmatrix import Matrix
from activation import sigmoid, dsigmoid
import sys

class NeuralNetwork(object):

	def __init__(self, input_nodes, hidden_layers, hidden_nodes, output_nodes, learning_rate):
		self.i_nodes = input_nodes
		self.h_layers = hidden_layers
		self.h_nodes_count = hidden_nodes
		self.h_outputs = []
		self.weights = []
		self.bias = []
		self.o_nodes = output_nodes
		self.weight_count = self.h_layers + 1

		if(len(self.h_nodes_count) != self.h_layers):
			print("Could not init NN\nHidden layers arg does not match the length of the hidden node arg")
			sys.exit(1)

		for i in range(self.we)

