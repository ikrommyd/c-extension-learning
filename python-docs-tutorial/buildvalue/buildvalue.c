#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *
build_value(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    PyObject *result = Py_BuildValue("{s:i,s:i}", "abc", 123, "def", 456);
    return result;
}

static PyMethodDef buildvalue_methods[] = {
    {"build_value",  build_value, METH_VARARGS, "module"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef buildvalue_module = {
    .m_methods = buildvalue_methods,
};

PyMODINIT_FUNC
PyInit_buildvalue(void)
{
    return PyModuleDef_Init(&buildvalue_module);
}
