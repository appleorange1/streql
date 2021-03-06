#include <Python.h>

// The core function: test two regions of memory for bytewise equality.
static int equals_internal(const char *x, unsigned int xlen, const char *y, unsigned int ylen) {
  int minlen = ( xlen > ylen ) ? ylen : xlen;
  int i, result = 0;

  for (i = 0; i < minlen; i++) result |= x[i] ^ y[i];

  return ( xlen == ylen ) && ( result == 0 );
}


static PyObject *equals(PyObject *self, PyObject *args) {
  const char *x = NULL, *y = NULL; unsigned int xlen, ylen;
  if (!PyArg_ParseTuple(args, "et#et#", "utf8", &x, &xlen, "utf8", &y, &ylen)) return NULL;
  int equal = equals_internal(x, xlen, y, ylen);
  PyMem_Free((void*)x); PyMem_Free((void*)y);

  if (equal) {
    Py_RETURN_TRUE;
  } else {
    Py_RETURN_FALSE;
  }
}


static PyMethodDef streql_methods[] = {
  {"equals", (PyCFunction)equals, METH_VARARGS,
   "equals(x, y): does x == y?\nRuntime does not depend on the bytes in the strings."},
  {NULL, NULL}
};


// The rest of this is rather ugly boilerplate to allow this module to compile on both
// Python 2 and 3. For more details, see http://docs.python.org/3/howto/cporting.html

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef moduledef = {
  PyModuleDef_HEAD_INIT, "streql", NULL, 0, streql_methods, NULL, NULL, NULL, NULL
};
#define INITERROR return NULL
PyObject *PyInit_streql(void)
#else
#define INITERROR return
void initstreql(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
  PyObject *module = PyModule_Create(&moduledef);
#else
  PyObject *module = Py_InitModule("streql", streql_methods);
#endif
  if (module == NULL) INITERROR;
#if PY_MAJOR_VERSION >= 3
  return module;
#endif
}
