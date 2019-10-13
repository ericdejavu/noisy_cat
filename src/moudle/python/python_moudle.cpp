#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <python3.7m/Python.h>
#include "python_module.h"
#include "../include/paser.h"

using namespace std;

Python::Python() {
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./service')");
}

Python::~Python() {
	Py_Finalize();
}

int Python::python3_load(string url, std::vector<std::string> datas) {

	int size = post_datas.size();

	PyObject *pModule = PyImport_ImportModule(url.c_str());
	PyObject *pFunc = PyObject_GetAttrString(pModule, "run");

	PyObject *py_args = NULL;
	if (size != 0) {
		py_args = PyTuple_New(size);
		for (int i=0;i<size;i++) {
			std::string data = datas[i];
			PyTuple_SetItem(py_args, i, PyBytes_FromString(data.c_str()));
		}
	}

	PyObject* object = PyEval_CallObject(pFunc, py_args);

}


Respnose *PythonParser::dy_service(std::vector<std::string> datas) {
	return pPython->python3_load(url, datas);
}
