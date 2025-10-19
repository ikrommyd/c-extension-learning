#include <Python.h>

typedef struct {
    PyObject q_base;
    Py_ssize_t q_maxsize;
    PyObject* q_elements;
} queue;

static PyObject*
queue_new(PyTypeObject* cls, PyObject* args, PyObject* kwargs)
{
    static char* keywords[] = {"maxsize", NULL};

    queue* self;
    Py_ssize_t maxsize = -1;
    if (!PyArg_ParseTupleAndKeywords(args,
                                     kwargs,
                                     "|n:Queue",
                                     keywords,
                                     &maxsize)) {
        return NULL;
    }

    if (!(self = (queue*) cls->tp_alloc(cls, 0))) {
        return NULL;
    }

    if (!(self->q_elements = PyList_New(0))) {
        Py_DECREF(self);
        return NULL;
    }

    self->q_maxsize = maxsize;

    return (PyObject*) self;
}

static void
queue_dealloc(queue* self)
{
    PyObject_GC_UnTrack(self);

    Py_CLEAR(self->q_elements);

    Py_TYPE(self)->tp_free(self);
}

static int
queue_traverse(queue* self, visitproc visit, void* arg)
{
    Py_VISIT(self->q_elements);
    return 0;
}

static int
queue_clear(queue* self)
{
    if (PyList_SetSlice(self->q_elements,
                        0,
                        PyList_GET_SIZE(self->q_elements),
                        NULL)) {
        return 1;
    }

    return 0;
}

static PyObject*
queue_repr(queue* self)
{
    return PyUnicode_FromFormat("Queue(maxsize=%zd, size=%zd)",
                                self->q_maxsize,
                                PyList_GET_SIZE(self->q_elements));
}

static PyObject*
queue_push(queue* self, PyObject* item)
{
    if (self->q_maxsize > 0 &&
        PyList_GET_SIZE(self->q_elements) == self->q_maxsize) {
        PyErr_SetString(PyExc_ValueError, "Queue is full");
        return NULL;
    }

    if (PyList_Append(self->q_elements, item)) {
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject*
queue_pop(queue* self)
{
    if (PyList_GET_SIZE(self->q_elements) == 0) {
        PyErr_SetString(PyExc_ValueError, "Queue is empty");
        return NULL;
    }

    PyObject* item = PyList_GET_ITEM(self->q_elements, 0);
    Py_INCREF(item);

    if (PyList_SetSlice(self->q_elements, 0, 1, NULL)) {
        Py_DECREF(item);
        return NULL;
    }

    return item;
}

static PyObject*
queue_to_list(queue* self)
{
    return PyList_GetSlice(self->q_elements, 0, PyList_GET_SIZE(self->q_elements));
}

PyMethodDef queue_methods[] = {
    {"push", (PyCFunction) queue_push, METH_O, "Push an item onto the queue"},
    {"pop", (PyCFunction) queue_pop, METH_NOARGS, "Pop an item from the queue"},
    {"to_list", (PyCFunction) queue_to_list, METH_NOARGS, "Return a list of all items in the queue"},
    {NULL, NULL, 0, NULL} /* sentinel */
};

static Py_ssize_t
queue_size(queue* self)
{
    return PyList_GET_SIZE(self->q_elements);
}

static PyObject*
queue_item(queue* self, Py_ssize_t ix)
{
    PyObject* item = PyList_GET_ITEM(self->q_elements, ix);
    Py_XINCREF(item);
    return item;
}

static int
queue_contains(queue* self, PyObject* item)
{
    return PySequence_Contains(self->q_elements, item);
}

PySequenceMethods queue_as_sequence = {
    (lenfunc) queue_size,                       /* sq_length */
    0,                                          /* sq_concat */
    0,                                          /* sq_repeat */
    (ssizeargfunc) queue_item,                  /* sq_item */
    0,                                          /* placeholder */
    0,                                          /* sq_ass_item */
    0,                                          /* placeholder */
    (objobjproc) queue_contains,                /* sq_contains */
    0,                                          /* sq_inplace_concat */
    0,                                          /* sq_inplace_repeat */
};

static PyObject*
queue_get_maxsize(queue* self, void* context)
{
    return PyLong_FromSsize_t(self->q_maxsize);
}

static int
queue_set_maxsize(queue* self, PyObject* value_ob, void* context)
{
    Py_ssize_t value = PyLong_AsSsize_t(value_ob);
    if (PyErr_Occurred()) {
        return -1;
    }

    if (value < 0) {
        self->q_maxsize = -1;
        return 0;
    }

    if (value < PyList_GET_SIZE(self->q_elements)) {
        PyErr_SetString(PyExc_ValueError,
                        "cannot drop the maxsize below the current size");
        return 1;
    }

    self->q_maxsize = value;
    return 0;
}

static PyObject*
queue_get_elements(queue* self, void* context)
{
    return PyList_GetSlice(self->q_elements, 0, PyList_GET_SIZE(self->q_elements));
}

PyGetSetDef queue_getset[] = {
    {"maxsize",
     (getter) queue_get_maxsize,
     (setter) queue_set_maxsize,
     "Maximum size of the queue",
     NULL},
    {"elements",
     (getter) queue_get_elements,
     NULL,
     "List of elements in the queue",
     NULL},
    {NULL, NULL, NULL, NULL, NULL} /* sentinel */
};

PyDoc_STRVAR(queue_doc, "A simple queue.");

static PyTypeObject queue_type = {
    PyVarObject_HEAD_INIT(NULL, 0) "queue.Queue", /* tp_name */
    sizeof(queue),                               /* tp_basicsize */
    0,                                           /* tp_itemsize */
    (destructor) queue_dealloc,                  /* tp_dealloc */
    0,                                           /* tp_vectorcall_offset */
    0,                                           /* tp_getattr */
    0,                                           /* tp_setattr */
    0,                                           /* tp_as_async */
    (reprfunc) queue_repr,                       /* tp_repr */
    0,                                           /* tp_as_number */
    &queue_as_sequence,                          /* tp_as_sequence */
    0,                                           /* tp_as_mapping */
    0,                                           /* tp_hash */
    0,                                           /* tp_call */
    0,                                           /* tp_str */
    0,                                           /* tp_getattro */
    0,                                           /* tp_setattro */
    0,                                           /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,     /* tp_flags */
    queue_doc,                                   /* tp_doc */
    (traverseproc) queue_traverse,               /* tp_traverse */
    (inquiry) queue_clear,                       /* tp_clear */
    0,                                           /* tp_richcompare */
    0,                                           /* tp_weaklistoffset */
    0,                                           /* tp_iter */
    0,                                           /* tp_iternext */
    queue_methods,                               /* tp_methods */
    0,                                           /* tp_members */
    queue_getset,                               /* tp_getset */
    0,                                           /* tp_base */
    0,                                           /* tp_dict */
    0,                                           /* tp_descr_get */
    0,                                           /* tp_descr_set */
    0,                                           /* tp_dictoffset */
    0,                                           /* tp_init */
    0,                                           /* tp_alloc */
    queue_new,                                  /* tp_new */
};

PyModuleDef queuemodule = {
    PyModuleDef_HEAD_INIT,
    "queue",
    NULL,
    -1,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC
PyInit_myqueue(void)
{
    PyObject* m;

    if (PyType_Ready(&queue_type) < 0) {
        return NULL;
    }

    if (!(m = PyModule_Create(&queuemodule))) {
        return NULL;
    }

    if (PyObject_SetAttrString(m, "Queue", (PyObject*) &queue_type)) {
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
