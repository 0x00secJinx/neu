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

    def __init__(self, input_nodes, hidden_nodes, output_nodes):
        self.i_nodes = input_nodes
        self.h_nodes = hidden_nodes
        self.o_nodes = output_nodes

        self.ih_weights = Matrix(self.h_nodes, self.i_nodes)
        self.ih_weights.randomize()
        self.ho_weights = Matrix(self.o_nodes, self.h_nodes)
        self.ho_weights.randomize()

        self.h_bias = Matrix(self.h_nodes, 1)
        self.h_bias.randomize()
        self.o_bias = Matrix(self.o_nodes, 1)
        self.o_bias.randomize()
        self.learning_rate = 0.9999

    def feedforward(self, input_array):

        # Generate the hidden outputs
        inputs = Matrix.inputFromArray(input_array)
        hidden = self.ih_weights.matrixProduct(inputs)
        hidden.matrixAdd(self.h_bias)
        # activation function
        hidden.map(sigmoid)
        #hidden.printMatrix()

        output = self.ho_weights.matrixProduct(hidden)
        output.matrixAdd(self.o_bias)
        output.map(sigmoid)
#        output.printMatrix()
        #sys.exit()
        return output.matrixToArray()

    def train(self, input_array, target_array):

        pdb.set_trace()
        #send to the first hidden layer
        inputs = Matrix.inputFromArray(input_array)
        hidden = self.ih_weights.matrixProduct(inputs)

        hidden.matrixAdd(self.h_bias)
        hidden.map(sigmoid)

        outputs = self.ho_weights.matrixProduct(hidden)
        outputs.matrixAdd(self.o_bias)
        outputs.map(sigmoid)

        targets = Matrix.targetsFromArray(target_array)

        output_errors = Matrix.matrixSubtract(targets, outputs)

        slope_output_layer = Matrix.staticMap(outputs, dsigmoid)
        slope_hidden_layer = Matrix.staticMap(hidden, dsigmoid)
        
        delta_output = Matrix.matrixMultiply(output_errors, slope_output_layer)
        hidden_errors = delta_output.matrixProduct(self.ho_weights.transpose())
        
        delta_hidden = Matrix.matrixMultiply(hidden_errors, slope_hidden_layer)
        self.ho_weights = self.ho_weights.matrixAdd(hidden.Transpose().matrixProduct(delta_output))
        self.ho_weights = self.ho_weights.multiply(self.learning_rate)
        
        self.ih_weights = self.ih_weights.matrixAdd(inputs.Transpose().matrixProduct(delta_hidden))
        self.ih_weights = self.ih_weights.multiply(self.learning_rate)

    def predict(self, input_array):
        output = self.feedforward(input_array)
        return output
