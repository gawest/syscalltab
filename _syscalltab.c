#include <Python.h>
#include "syscalltab_func.h"

static char module_docstring[] =
    "Simple methods getting system call information from built system";
static char syscall_number_docstring[] =
    "Returns system call number in unitstd.h based on input system call name \
     string. 'None' is returned if the input system call doesn't exist";
static char syscall_count_docstring[] =
    "Returns a total count of system calls available on build system";
static char syscall_build_docstring[] =
    "Returns a dict of the kernel release and machine identifier strings \
     that this module is built against. This _should_ be consistent with \
     'uname -r' and 'uname -m' output from the shell.";
    

static PyObject* syscalltab_syscall_count(PyObject *self, PyObject *args);
static PyObject* syscalltab_syscall_number(PyObject *self, PyObject *args);
static PyObject* syscalltab_syscall_buildinfo(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"count", syscalltab_syscall_count, METH_VARARGS, syscall_count_docstring},
    {"number", syscalltab_syscall_number, METH_VARARGS, syscall_number_docstring},
    {"buildinfo", syscalltab_syscall_buildinfo, METH_VARARGS, syscall_build_docstring},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initsyscalltab(void)
{
    int kverify = _syscall_kverify();

    if (kverify != 0)
    {
        PyErr_Format(PyExc_ImportError, "Failed compatibility with kernel version: %s and machine arch: %s", syscall_krelease(), syscall_karch());

        return;
    }

    PyObject *sct = Py_InitModule3("syscalltab", module_methods, module_docstring);

    if (! sct)
    {
        return;
    }
}

static PyObject *syscalltab_syscall_count(PyObject *self, PyObject *args)
{
    int value;

    if (PyArg_ParseTuple(args, ""))
    {
        value = syscall_count();
        return Py_BuildValue("i", value);
    }

    return NULL;
}

static PyObject *syscalltab_syscall_number(PyObject *self, PyObject *args)
{
    char* syscallname;
    int value;

    if (PyArg_ParseTuple(args, "s", &syscallname))
    {
        value = syscall_number(syscallname);

        if (value == -1)
        {
            Py_INCREF(Py_None);
            return Py_None;
        }

        return Py_BuildValue("i", value);
    }

    return NULL;
}

static PyObject *syscalltab_syscall_buildinfo(PyObject *self, PyObject *args)
{
    if (PyArg_ParseTuple(args, ""))
    {
        char* kmachine = syscall_karch();
        char* krelease = syscall_krelease();

        return Py_BuildValue("{s:s,s:s}",
                             "release", krelease, "machine", kmachine);
    }

    return NULL;
}
