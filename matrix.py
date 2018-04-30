#!/usr/bin/python -B
#from random import randint
from Matrix import Matrix
import sys

class matrix(Matrix):

    @staticmethod
    def product(a,b):

        if(not isinstance(a, matrix)):
            if(not isinstance(b, matrix)):
                raise TypeError

        # matrixProduct returns tuple (rows, cols, array)
        result_tuple = a.matrixProduct(b)
        if (result_tuple == None):
            raise Exception
        result = matrix(result_tuple[0], result_tuple[1])
        result.matrixFromArray(result_tuple[2], len(result_tuple[2]))

        return result


    @staticmethod
    def transpose(m):

        result_tuple = matrix.transposeMatrix(m)
        result = matrix(result_tuple[0], result_tuple[1])
        result.matrixFromArray(result_tuple[2], len(result_tuple[2]))
        return result

    @staticmethod
    def fromArray(mat, arr):
        for i in range(mat.rows):
            for j in range(mat.cols):
                mat.setMatrixValue(i,j,arr[j+i*mat.cols])

    @staticmethod
    def subtract(a,b):
        # add checks for matrix
        result_tuple = a.matrixSubtract(b)
        result = matrix(result_tuple[0], result_tuple[1])
        matrix.fromArray(result, result_tuple[2])
        return result


    @staticmethod
    def inputFromArray(arr):
        m = matrix(len(arr), 1)
        m.matrixFromArray(arr, len(arr))
        return m

    def toArray(self):
        arr = []
        for i in range(self.rows):
            for j in range(self.cols):
                arr.append(self.getMatrixValue(i,j))

        return arr

    @staticmethod
    def staticMap(matrx, fn):
        result = matrix(matrx.rows, matrx.cols)
        for i in range(matrx.rows):
            for j in range(matrx.cols):
                val = matrx.getMatrixValue(i,j)
                result.setMatrixValue(i,j, fn(val))
        return result

    def map(self, fn):
        for i in range(self.rows):
            for j in range(self.cols):
                val = self.getMatrixValue(i,j)
                self.setMatrixValue(i, j, fn(val))



if __name__ == '__main__':
    pass
