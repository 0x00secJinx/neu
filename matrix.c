#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <structmember.h>
#include <math.h>


// Get a true random seed for the random function
unsigned long long rdtsc(void){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

float randomwithoutzero(void) {
    float randomint = -1.0;
    while(randomint == -1.0){
        randomint = (((double)(rand() % 10)/(double)10)*2-1);
    }

    return randomint;
}

// Matrix Class Functions
typedef struct {
    PyObject_HEAD;
    int rows;
    int cols;
    float **matrix; // 2d array
} Matrix;


static void Matrix_dealloc(Matrix* self){

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject * Matrix_new(PyTypeObject *type, PyObject *args) {
    /*
        Allocate the new matrix object when a new instance is created
     */

    Matrix *self;

    self = (Matrix *)type->tp_alloc(type, 0);

    // Init all instance variables to 0
    if (self != NULL) {
        self->rows = 0;
        self->cols = 0;
        self->matrix = 0;
    }

    return (PyObject *)self;

}

static int Matrix_init(Matrix *self, PyObject *args) {
    /*
        The class __init__ function
            @param int row - number of rows of matrix
            @param int col - number of columns of matrix
     */

    //PyObject *selfv;

    // __init__ takes two args, rows, cols
    if (!PyArg_ParseTuple(args,"ii",&self->rows,&self->cols)){
        perror("ParseTuples");
        return -1;
    }

    // Create the matrix by allocing on the heap
    self->matrix = malloc(self->rows*sizeof(float *));
    if (self->matrix == NULL) {
        return -1;
    }
    for (int i = 0; i < self->rows; i++) {
        self->matrix[i] = malloc(self->cols*sizeof(float));
        if (self->matrix[i] == NULL) {
            return -1;
        }
    }

    //init the matrix with all 0
    for (int i = 0; i < self->rows; i++) {
        for (int j = 0; j < self->cols; j++) {
            self->matrix[i][j] = 0;
        }
    }


    return 0;
}

// The class variables and where to find them from the struct
static PyMemberDef Matrix_members[] = {
    {"rows", T_INT, offsetof(Matrix, rows), 0, "# of rows"},
    {"cols", T_INT, offsetof(Matrix, cols), 0, "# of cols"},
    {"matrix", T_FLOAT, offsetof(Matrix, matrix), 0, "matrix"},
    {NULL}
};


static PyObject * Matrix_randomize(Matrix *self, PyObject *args) {

    // Get the random seed and init the matrix
    // with values obtained divided by the
    // modulus of 10 to get a value between 0-9
    srand(rdtsc());
    for (int i = 0; i < self->rows; i++) {
        for (int j = 0; j < self->cols; j++) {
            self->matrix[i][j] = randomwithoutzero();
        }
    }
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject * Matrix_printMatrix(Matrix *self, PyObject *args) {

    // Display the matrix as you would see a matrix
    for (int i = 0; i < self->rows; i++) {
        printf("[");
        for (int j = 0; j < self->cols; j++) {
            printf(" %.6f ", self->matrix[i][j]);
        }
        printf("]\n");
    }

    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject * Matrix_add(Matrix *self, PyObject *args) {
    /*
        Add a number to each element in the matrix
     */

    int num;
    if (!PyArg_ParseTuple(args,"i",&num)){
        perror("ParseTuples");
        return NULL;
    }

    for (int i = 0; i < self->rows; i++) {
        for (int j = 0; j < self->cols; j++) {
            self->matrix[i][j] = self->matrix[i][j] + (float)num;
        }
    }

    Py_INCREF(Py_None);
    return Py_None;

}


static PyObject * Matrix_multiply(Matrix *self, PyObject *args) {
    /*
        Multiply a number to each element in the matrix
     */

    float num;
    if (!PyArg_ParseTuple(args,"f",&num)){
        perror("ParseTuples");
        return NULL;
    }

    for (int i = 0; i < self->rows; i++){
        for (int j = 0; j < self->cols; j++){
            self->matrix[i][j] = self->matrix[i][j] * num;
        }
    }

    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject * Matrix_product(Matrix *self, PyObject *args) {
    /*
        Multiply a matrix by a matrix
     */

    PyObject *matrix_array;
    Matrix *n;
    int result_rows;
    int result_cols;
    float sum;
    int array_len;
    Py_INCREF(Py_None);

    // Get the second matrix as a python object
    if(!PyArg_ParseTuple(args, "O", &n)){
        printf("product ParseTuples error\n");

        return Py_None;
    }

    /*
        In order to perform this operation, the
        columns of the first matrix must equal
        the rows of the second matrix. The result
        of the operation will have the dimensions
        of the first matrix rows by the second
        matrix cols
     */
    if(self->cols != n->rows){
        printf("Could not perform matrix product\nProduct Col v. Row check\n");
        return Py_None;
    }
    result_rows = self->rows;
    result_cols = n->cols;
    array_len = result_rows * result_cols;

    float flat_matrix[array_len];

    for (int i = 0; i < result_rows; i++) {
        for (int j = 0; j < result_cols; j++) {
            sum = 0;
            for (int k = 0; k < self->cols; k++){
                sum += self->matrix[i][k] * n->matrix[k][j];
//                printf("%d\n", sum);
            }
            flat_matrix[j+(i*result_cols)] = sum;
        }
    }

    matrix_array = PyList_New(array_len);
    if (!matrix_array){
        return Py_None;
    }
    for (int i = 0; i < array_len; i++){
        PyObject *num = PyFloat_FromDouble(flat_matrix[i]);
        if (!num) {
            Py_DECREF(matrix_array);
            return Py_None;
        }
        PyList_SET_ITEM(matrix_array, i, num);
    }
    Py_DECREF(Py_None);
    return Py_BuildValue("iiO",result_rows, result_cols, matrix_array);

}

static PyObject * Matrix_fromArray(Matrix *self, PyObject *args) {
    PyObject *matrix_array;
    int array_len;
    Py_INCREF(Py_None);

    if(!PyArg_ParseTuple(args, "Oi", &matrix_array, &array_len)) {
        perror("ParseTuples");
        return Py_None;
    }
    float flat_matrix[array_len];
    for (int i = 0; i < array_len; i++) {
        PyObject *num = PyList_GetItem(matrix_array, i);
        if (!num) {
            Py_DECREF(num);
            return Py_None;
        }
        flat_matrix[i] = PyFloat_AsDouble(num);
    }

    for (int i = 0; i < self->rows; i++) {
        for (int j = 0; j < self->cols; j++) {
            self->matrix[i][j] = flat_matrix[j+(i*self->cols)];
        }
    }

    return Py_None;

}

static PyObject *Matrix_transpose(Matrix *self, PyObject *args) {
    PyObject *matrix_array;
    int result_cols = self->rows;
    int result_rows = self->cols;
    int array_len = result_rows*result_cols;
    float Ctransposedmatrix[result_rows][result_cols];
    Py_INCREF(Py_None);

    for(int i = 0; i < self->rows; i++) {
        for(int j = 0; j < self->cols; j++) {
            Ctransposedmatrix[j][i] = self->matrix[i][j];
        }
    }

    matrix_array = PyList_New(array_len);
    if (!matrix_array){
        return Py_None;
    }
    for (int i = 0; i < result_rows; i++){
        for (int j = 0; j < result_cols; j++) {
            PyObject *num = PyFloat_FromDouble(Ctransposedmatrix[i][j]);
            if (!num) {
                Py_DECREF(matrix_array);
                return Py_None;
            }
            PyList_SET_ITEM(matrix_array, j+i*result_cols, num);
        }
    }

    return Py_BuildValue("iiO",result_rows, result_cols, matrix_array);

}


static PyObject *Matrix_matrixAdd(Matrix *self, PyObject *args) {
    Matrix *n;
    if(!PyArg_ParseTuple(args, "O", &n)){
        printf("The argument is not a matrix!\n");
        Py_INCREF(Py_None);
        return Py_None;
    }

    for(int i = 0; i < self->rows; i++) {
        for(int j = 0; j < self->cols; j++) {
            self->matrix[i][j] = self->matrix[i][j] + n->matrix[i][j];
        }
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *Matrix_getValue(Matrix *self, PyObject *args) {
    int row, col;
    if(!PyArg_ParseTuple(args, "ii", &row, &col)){
        printf("getMatrixValue requires a row and column param\n");
        Py_INCREF(Py_None);
        return Py_None;
    }
    if(row > self->rows) {
        printf("That index does not exist\n");
        Py_INCREF(Py_None);
        return Py_None;
    }
    if(col > self->cols) {
        printf("That index does not exist\n");
        Py_INCREF(Py_None);
        return Py_None;
    }
    //printf("%f\n", self->matrix[row][col]);
    return Py_BuildValue("d", self->matrix[row][col]);
}

static PyObject *Matrix_setValue(Matrix *self, PyObject *args) {
    int row, col;
    float val;
    if(!PyArg_ParseTuple(args, "iif", &row, &col, &val)){
        printf("Usage: matrix.setMatrixValue(row, col, val)\n");
        Py_INCREF(Py_None);
        return Py_None;
    }
    self->matrix[row][col] = val;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *Matrix_fullMultiply(Matrix *self, PyObject *args) {
    Matrix *n;
    Py_INCREF(Py_None);
    if(!PyArg_ParseTuple(args, "O", &n)){
        printf("matrixMultiply needs a matrix object\n");
        return Py_None;
    }
    if(self->rows != n->rows){
        printf("Matricies need the same dimensions\n");
        return Py_None;
    }
    if(self->cols != n->cols){
        printf("Matricies need the same dimensions\n");
        return Py_None;
    }
    for(int i = 0; i < self->rows; i++){
        for(int j = 0; j < self->cols; j++){
            self->matrix[i][j] = self->matrix[i][j] * n->matrix[i][j];
        }
    }
    return Py_None;
}

static PyObject *Matrix_subtract(Matrix *self, PyObject *args) {
    Matrix *n;
    PyObject *matrix_array;
    int array_len = self->rows * self->cols;
    float flat_matrix[array_len];

    Py_INCREF(Py_None);

    if(!PyArg_ParseTuple(args, "O", &n)){
        printf("ParseTuples error\n");
        return Py_None;
    }
    if(self->rows != n->rows){
        printf("Matricies need the same dimensions\n");
        return Py_None;
    }
    if(self->cols != n->cols){
        printf("Matricies need the same dimensions\n");
        return Py_None;
    }

    for(int i = 0; i < self->rows; i++) {
        for(int j = 0; j < self->cols; j++){
            flat_matrix[j+(i*self->cols)] = self->matrix[i][j] - n->matrix[i][j];
        }
    }

    matrix_array = PyList_New(array_len);
    if(!matrix_array){
        printf("!matrix_array\n");
        return Py_None;
    }
    for(int i = 0; i < array_len; i++){
        PyObject *num = PyFloat_FromDouble(flat_matrix[i]);
        if(!num){
            printf("!num\n");
            Py_DECREF(matrix_array);
            return Py_None;
        }
        PyList_SET_ITEM(matrix_array, i, num);
    }
    return Py_BuildValue("iiO", self->rows, self->cols, matrix_array);

}

static PyObject *Matrix_toArray(Matrix *self, PyObject *args){
    PyObject *matrix_array;
    int array_len = self->rows * self->cols;

    matrix_array = PyList_New(array_len);
    if(!matrix_array){
        return Py_None;
    }
    for(int i = 0; i < self->rows; i++){
        for(int j = 0; j < self->cols; j++) {
            PyObject *num = PyFloat_FromDouble(self->matrix[i][j]);
            if(!num){
                Py_DECREF(matrix_array);
                Py_DECREF(num);
                return Py_None;
            }
            PyList_SET_ITEM(matrix_array, j+(i*self->cols), num);
        }
    }

    return Py_BuildValue("O", matrix_array);

}

static PyMethodDef Matrix_Methods[] = {

     {"__init__",        (PyCFunction)Matrix_init,             METH_VARARGS,   NULL},
     {"randomize",       (PyCFunction)Matrix_randomize,        METH_VARARGS,   NULL},
     {"printMatrix",     (PyCFunction)Matrix_printMatrix,      METH_VARARGS,   NULL},
     {"add",             (PyCFunction)Matrix_add,              METH_VARARGS,   NULL},
     {"multiply",        (PyCFunction)Matrix_multiply,         METH_VARARGS,   NULL},
     {"matrixProduct",   (PyCFunction)Matrix_product,          METH_VARARGS,   NULL},
     {"matrixFromArray", (PyCFunction)Matrix_fromArray,        METH_VARARGS,   NULL},
     {"transposeMatrix", (PyCFunction)Matrix_transpose,        METH_VARARGS,   NULL},
     {"matrixAdd",       (PyCFunction)Matrix_matrixAdd,        METH_VARARGS,   NULL},
     {"getMatrixValue",  (PyCFunction)Matrix_getValue,         METH_VARARGS,   NULL},
     {"setMatrixValue",  (PyCFunction)Matrix_setValue,         METH_VARARGS,   NULL},
     {"matrixMultiply",  (PyCFunction)Matrix_fullMultiply,     METH_VARARGS,   NULL},
     {"matrixSubtract",  (PyCFunction)Matrix_subtract,         METH_VARARGS,   NULL},
     {"matrixToArray",   (PyCFunction)Matrix_toArray,          METH_VARARGS,   NULL},
    // ending sentinel*/
    {NULL}

};

static PyTypeObject MatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Matrix.Matrix",
    sizeof(Matrix),
    0,
    (destructor)Matrix_dealloc,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,
    "Matrix objects",
    0,
    0,
    0,
    0,
    0,
    0,
    Matrix_Methods,
    Matrix_members,
    0,
    0,
    0,
    0,
    0,
    0,
    (initproc)Matrix_init,
    0,
    Matrix_new,
};

static PyMethodDef module_methods[] = {
    {NULL}
};

PyMODINIT_FUNC initMatrix(void) {

    PyObject *m;

    if (PyType_Ready(&MatrixType) < 0) {
        return;
    }

    m = Py_InitModule3("Matrix", module_methods, "Module to perform matrix ops");
    if (m == NULL) {
        return;
    }

    Py_INCREF(&MatrixType);
    PyModule_AddObject(m, "Matrix", (PyObject *)&MatrixType);

}
