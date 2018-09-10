#!/usr/bin/python
from random import SystemRandom, uniform
import sys

class Matrix(object):

	def __init__(self, rows, cols):
		self.rows = rows
		self.cols = cols
		self.matrix = []

		for i in range(self.rows):
			arr = []
			for j in range(self.cols):
				arr.append(0.0)
			self.matrix.append(arr)

	def printMatrix(self):
		for i in self.matrix:
			print(i)

	def getCols(self):
		col_arr = []
		for j in range(self.cols):
			for i in range(self.rows):
				col_arr.append(self.matrix[i][j])
		return col_arr

	def randomize(self):
		secure_random = SystemRandom()
		for i in range(self.rows):
			for j in range(self.cols):
				self.matrix[i][j] = secure_random.uniform(-0.99, 1)

	def add(self, x):
		for i in range(self.rows):
			for j in range(self.cols):
				self.matrix[i][j] += float(x)

	def multiply(self, x):
		for i in range(self.rows):
			for j in range(self.cols):
				self.matrix[i][j] *= float(x)

	def matrixProduct(self, n):
		if(not isinstance(n, Matrix)):
			print("matrixProduct: param is not a matrix object")
			raise TypeError

		if(self.cols != n.rows):
			print("Cannot perform matrix product")
			print("self.rows: %d\tn.rows: %d\nself.cols: %d\tn.cols: %d" % (self.rows, n.rows, self.cols, n.cols))
			raise Exception

		result = Matrix(self.rows, n.cols)

		for i in range(self.rows):
			for j in range(n.cols):
				summ = 0
				for k in range(self.cols):
					summ += self.matrix[i][k] * n.matrix[k][j]
				result.matrix[i][j] = summ

		return result

	def matrixFromArray(self, arr):
		if (len(arr) != (self.rows * self.cols)):
			print("Array elements do not equal the size of the matrix")
			raise Exception

		for i in range(self.rows):
			for j in range(self.cols):
				self.matrix[i][j] = arr[j+(i*self.cols)]

	@staticmethod
	def targetsFromArray(arr):
		new_arr = []
		for i in range(len(arr)):
			new_arr.append(float(arr[i]))
		m = Matrix(len(arr), 1)
		m.matrixFromArray(new_arr)
		return m

	@staticmethod
	def inputFromArray(arr):
		new_arr = []
		for i in range(len(arr)):
			new_arr.append(float(arr[i]))
		m = Matrix(len(arr),1)
		m.matrixFromArray(new_arr)
		return m


	def transposeMatrix(self):
		result = Matrix(self.cols, self.rows)
		for i in range(self.rows):
			for j in range(self.cols):
				result.matrix[j][i] = self.matrix[i][j]

		return result

	def matrixAdd(self, n):
		if(not isinstance(n, Matrix)):
			print("matrixAdd param not a Matrix")
			raise TypeError

		if((self.rows != n.rows) or (self.cols != n.cols)):
			print("self.rows: %d\t n.rows: %d" % (self.rows, n.rows))
			print("self.cols: %d\t n.cols: %d" % (self.cols, n.cols))
			print("matrixAdd: Matricies are not the same dimensions")
			raise Exception

		for i in range(self.rows):
			for j in range(self.cols):
				self.matrix[i][j] += n.matrix[i][j]

	def matrixMultiply(self, n):
		if(not isinstance(n, Matrix)):
			print("matrixMultiply param not a Matrix")
			raise TypeError

		if((self.rows != n.rows) or (self.cols != n.cols)):
			print("self.rows: %d\t n.rows: %d" % (self.rows, n.rows))
			print("self.cols: %d\t n.cols: %d" % (self.cols, n.cols))
			print("matrixMultiply: Matricies are not the same dimensions")
			raise Exception

		for i in range(self.rows):
			for j in range(self.cols):
				self.matrix[i][j] *= n.matrix[i][j]

	def matrixSubtract(self, n):
		if(not isinstance(n, Matrix)):
			print("matrixSubtract param not a Matrix")
			raise TypeError

		if((self.rows != n.rows) or (self.cols != n.cols)):
			print("matrixSubtract: Matricies are not the same dimensions")
			raise Exception

		result = Matrix(self.rows, self.cols)
		for i in range(self.rows):
			for j in range(self.cols):
				result.matrix[i][j] = float(self.matrix[i][j] - n.matrix[i][j])

		return result

	@staticmethod
	def staticMap(matrx, fn):
		result = Matrix(matrx.rows, matrx.cols)
		for i in range(matrx.rows):
			for j in range(matrx.cols):
				result.matrix[i][j] = fn(matrx.matrix[i][j])

		return result

	def map(self, fn):
		for i in range(self.rows):
			for j in range(self.cols):
				self.matrix[i][j] = fn(self.matrix[i][j])

	def matrixToArray(self):
		arr = []
		for i in range(self.rows):
			for j in range(self.cols):
				arr.append(self.matrix[i][j])

		return arr

	def fillMatrix(self, number):
		for i in range(self.rows):
			for j in range(self.cols):
				self.matrix[i][j] = number