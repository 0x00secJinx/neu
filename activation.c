#include <Python.h>
#include <math.h>

float sigmoid(float x){
	return 1 / (1 + (pow((float)M_E, (float)-x)));
}

float tanh(float y) {
	return (pow((float)M_E, y) - pow((float)M_E, -y)) / (pow((float)M_E, y) + pow((float)M_E, -y));
}

static PyObject *activation_sigmoid(PyObject *self, PyObject *args) {
	float x;
	float result;
	if(!PyArg_ParseTuple(args, "f", &x)) {
		printf("sigmoid requires a float!\n");
		Py_INCREF(Py_None);
		return Py_None;
	}
	result = pow((float)M_E, (float)-x);
	result = 1 / (1 + result);

	return Py_BuildValue("f", result);
}

static PyObject *activation_dsigmoid(PyObject *self, PyObject *args) {
	float y;
	float result;
	if(!PyArg_ParseTuple(args,"f", &y)){
		printf("dsigmoid requires a float!\n");
		Py_INCREF(Py_None);
		return Py_None;
	}
	result = y * (1-y);
	return Py_BuildValue("f", result);
}

static PyObject* activation_relu(PyObject *self, PyObject *args) {
	float y;
	float result;
	if(!PyArg_ParseTuple(args,"f", &y)){
		printf("relu requires a float!\n");
		Py_INCREF(Py_None);
		return Py_None;
	}
	// Basically a max function
	if (y > 0) {
		result = y;
	} else {
		result = 0.0;	
	}
	return Py_BuildValue("f", result);
}

static PyObject *activation_drelu(PyObject *self, PyObject *args) {
	float y;
	float result;
	if(!PyArg_ParseTuple(args,"f", &y)){
		printf("drelu requires a float!\n");
		Py_INCREF(Py_None);
		return Py_None;
	}
	if (y > 0) {
		result = 1.0;
	} else {
		result = 0.0;
	}
	return Py_BuildValue("f", result);
	
}

static PyObject *activation_tanh(PyObject *self, PyObject *args) {
	float y;
	float result;
	if(!PyArg_ParseTuple(args,"f", &y)){
		printf("tanh requires a float!\n");
		Py_INCREF(Py_None);
		return Py_None;
	}
	result = (pow((float)M_E, y) - pow((float)M_E, -y)) / (pow((float)M_E, y) + pow((float)M_E, -y));
	return Py_BuildValue("f", result);
	
}

static PyObject *activation_dtanh(PyObject* self, PyObject *args) {
	float y;
	float result;
	if(!PyArg_ParseTuple(args,"f", &y)){
		printf("dtanh requires a float!\n");
		Py_INCREF(Py_None);
		return Py_None;
	}
	result = 1 - pow(tanh(y), float(2));
	return Py_BuildValue("f", result);
}

static PyMethodDef module_methods[] = {
	{"sigmoid",  (PyCFunction)activation_sigmoid,  METH_VARARGS, NULL},
	{"dsigmoid", (PyCFunction)activation_dsigmoid, METH_VARARGS, NULL},
	{"relu",     (PyCFunction)activation_relu,     METH_VARARGS, NULL},
	{"drelu",    (PyCFunction)activation_drelu,    METH_VARARGS, NULL},
	{"tanh",     (PyCFunction)activation_tanh,     METH_VARARGS, NULL},
	{"dtanh",    (PyCFunction)activation_dtanh,    METH_VARARGS, NULL},
	{NULL}
};

PyMODINIT_FUNC initactivation(void) {
	PyObject *m;

	m = Py_InitModule("activation", module_methods);
	if (m == NULL) {
		return;
	}
}
