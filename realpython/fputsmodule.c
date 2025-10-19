#include <Python.h>

static PyObject* StringTooShortError;

static PyObject*
method_fputs(PyObject* self, PyObject* args)
{
    char* str = NULL;
    char* filename = NULL;
    int bytes_copied = -1;

    if (!PyArg_ParseTuple(args, "ss", &str, &filename)) {
        return NULL;
    }

    if (strlen(str) < 10) {
        PyErr_SetString(StringTooShortError, "String length must be greater than 10");
        return NULL;
    }

    FILE* fp = fopen(filename, "w");
    bytes_copied = fputs(str, fp);
    fclose(fp);

    return PyLong_FromLong(bytes_copied);
}

PyDoc_STRVAR(fputs_doc, "Python interface for fputs C library function");
static PyMethodDef
FputsMethods[] = {
    {"fputs", method_fputs, METH_VARARGS, fputs_doc},
    {NULL, NULL,0, NULL}
};

PyDoc_STRVAR(fputs_module_doc, "Python interface for fputs C library function");
static struct PyModuleDef
fputsmodule = {
    PyModuleDef_HEAD_INIT,
    "fputs",
    fputs_module_doc,
    -1,
    FputsMethods,
    NULL,
    NULL,
    NULL,
    NULL
};


PyMODINIT_FUNC
PyInit_fputs(void) {
    PyObject* module = PyModule_Create(&fputsmodule);

    StringTooShortError = PyErr_NewException("fputs.StringTooShortError", NULL, NULL);

    PyModule_AddObject(module, "StringTooShortError", StringTooShortError);

    PyModule_AddIntConstant(module, "FPUTS_FLAG", 64);

    #define FPUTS_MACRO 256

    PyModule_AddIntMacro(module, FPUTS_MACRO);

    return module;
}

