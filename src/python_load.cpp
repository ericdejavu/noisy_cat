#include <iostream>
#include <cstring>
#include <cstdio>
#include <python3.7m/Python.h>

using namespace std;

int main() {
	string url = "url_test";
	string data = "1234";
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	PyObject *pModule = PyImport_ImportModule(url.c_str());
	PyObject *pFunc = PyObject_GetAttrString(pModule, "run");

	PyObject *py_args = PyTuple_New(1);

	PyTuple_SetItem(py_args, 0, PyBytes_FromString(data.c_str()));


	PyEval_CallObject(pFunc, py_args);
	Py_Finalize();
}
