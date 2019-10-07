#include <iostream>
#include <cstring>
#include <cstdio>
#include <python3.7m/Python.h>
#include "python_module.h"

using namespace std;

Python::Python() {
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./service')");
}

Python::~Python() {
	Py_Finalize();
}

int Python::python3_load(string url, string data) {

	PyObject *pModule = PyImport_ImportModule(url.c_str());
	PyObject *pFunc = PyObject_GetAttrString(pModule, "run");

	PyObject *py_args = PyTuple_New(1);

	PyTuple_SetItem(py_args, 0, PyBytes_FromString(data.c_str()));

	PyEval_CallObject(pFunc, py_args);

}
