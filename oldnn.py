#!/usr/bin/python
from nmatrix import Matrix
from activation import sigmoid, dsigmoid
from random import SystemRandom, uniform
import sys
import pdb

"""
TO DO:

    Have a matrix to store each hidden answer so backpropogation can work
    [*] Actually did this in a new file where I can have an expandable Neural
        Network

"""

class NeuralNetwork(object):

    def __init__(self, input_nodes=0, hidden_nodes=0, output_nodes=0, a=None):
        if(isinstance(a, NeuralNetwork)):
            self.i_nodes = a.i_nodes
            self.h_nodes = a.h_nodes
            self.o_nodes = a.o_nodes

            self.ih_weights = a.ih_weights.copy()
            self.ho_weights = a.ho_weights.copy()

            self.h_bias = a.h_bias.copy()
            self.o_bias = a.o_bias.copy()

        else:
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
            self.learning_rate = 0.01

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

        gradients = Matrix.staticMap(outputs, dsigmoid)
        gradients.matrixMultiply(output_errors)
        gradients.multiply(self.learning_rate)

        hidden_T = hidden.transposeMatrix()
        weight_ho_deltas = gradients.matrixProduct(hidden_T)


        self.ho_weights.matrixAdd(weight_ho_deltas)
        self.o_bias.matrixAdd(gradients)

        # Hidden layer weights start here
        # Transpose the weights between the nodes of the layer
        #   the program is working with and the previous changed
        #   weights
        who_t = self.ho_weights.transposeMatrix()

        # Find the product of the transposed matrix and the errors
        #   of the previous nodes
        hidden_errors = who_t.matrixProduct(output_errors)

        # Calculate the gradients for the hidden outputs
        hidden_gradient = Matrix.staticMap(hidden, dsigmoid)
        hidden_gradient.matrixMultiply(hidden_errors)
        hidden_gradient.multiply(self.learning_rate)

        # Get the gradients for the nodes in the layer
        #   before and multiply them by the gradient
        inputs_T = inputs.transposeMatrix()
        weight_ih_deltas = hidden_gradient.matrixProduct(inputs_T)

        self.ih_weights.matrixAdd(weight_ih_deltas)
        self.h_bias.matrixAdd(hidden_gradient)

    def predict(self, input_array):
        output = self.feedforward(input_array)
        return output

    def copy(self):
        return NeuralNetwork(self, a=self)

    def mutate(self, rate):
        self.ih_weights.mutate(rate)
        self.ho_weights.mutate(rate)
        self.h_bias.mutate(rate)
        self.o_bias.mutate(rate)


https://docs.gitlab.com/ee/install/installation.html