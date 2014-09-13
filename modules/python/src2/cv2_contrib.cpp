#if defined(_MSC_VER) && (_MSC_VER >= 1800)
// eliminating duplicated round() declaration
#define HAVE_ROUND
#endif

#include <Python.h>

#define MODULESTR "cv2_contrib"
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/ndarrayobject.h>

#include "pyopencv_generated_include.h"
#include "pyopencv_generated_contrib_include.h"
#include "opencv2/core/types_c.h"

#include "pycv2.hpp"
///////////////////////////////////////////////////////////////////////////////////////

static int convert_to_char(PyObject *o, char *dst, const char *name = "no_name")
{
  if (PyString_Check(o) && PyString_Size(o) == 1) {
    *dst = PyString_AsString(o)[0];
    return 1;
  } else {
    (*dst) = 0;
    return failmsg("Expected single character string for argument '%s'", name);
  }
}

#if PY_MAJOR_VERSION >= 3
#define MKTYPE2(NAME) pyopencv_##NAME##_specials(); if (!to_ok(&pyopencv_##NAME##_Type)) return NULL;
#else
#define MKTYPE2(NAME) pyopencv_##NAME##_specials(); if (!to_ok(&pyopencv_##NAME##_Type)) return
#endif

#ifdef __GNUC__
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include "pyopencv_generated_types.h"
#include "pyopencv_generated_contrib_types.h"
#include "pyopencv_generated_contrib_funcs.h"

static PyMethodDef special_methods[] = {

//#include "pyopencv_generated_contrib_func_tab.h"
  {NULL, NULL},
};

/************************************************************************/
/* Module init */
struct ConstDef
{
    const char * name;
    long val;
};

static void init_submodule(PyObject * root, const char * name, PyMethodDef * methods, ConstDef * consts)
{
  // traverse and create nested submodules
  std::string s = name;
  size_t i = s.find('.');
  while (i < s.length() && i != std::string::npos)
  {
    size_t j = s.find('.', i);
    if (j == std::string::npos)
        j = s.length();
    std::string short_name = s.substr(i, j-i);
    std::string full_name = s.substr(0, j);
    i = j+1;

    PyObject * d = PyModule_GetDict(root);
    PyObject * submod = PyDict_GetItemString(d, short_name.c_str());
    if (submod == NULL)
    {
        submod = PyImport_AddModule(full_name.c_str());
        PyDict_SetItemString(d, short_name.c_str(), submod);
    }
    root = submod;
  }

  // populate module's dict
  PyObject * d = PyModule_GetDict(root);
  for (PyMethodDef * m = methods; m->ml_name != NULL; ++m)
  {
    PyObject * method_obj = PyCFunction_NewEx(m, NULL, NULL);
    PyDict_SetItemString(d, m->ml_name, method_obj);
    Py_DECREF(method_obj);
  }
  for (ConstDef * c = consts; c->name != NULL; ++c)
  {
    PyDict_SetItemString(d, c->name, PyInt_FromLong(c->val));
  }

}

#include "pyopencv_generated_contrib_ns_reg.h"

static int to_ok(PyTypeObject *to)
{
  to->tp_alloc = PyType_GenericAlloc;
  to->tp_new = PyType_GenericNew;
  to->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  return (PyType_Ready(to) == 0);
}


#if PY_MAJOR_VERSION >= 3
extern "C" CV_EXPORTS PyObject* PyInit_cv2_contrib();
static struct PyModuleDef cv2_contrib_moduledef =
{
    PyModuleDef_HEAD_INIT,
    MODULESTR,
    "Python wrapper for OpenCV.",
    -1,     /* size of per-interpreter state of the module,
               or -1 if the module keeps state in global variables. */
    special_methods
};

PyObject* PyInit_cv2_contrib()
#else
extern "C" CV_EXPORTS void initcv2_contrib();

void initcv2_contrib()
#endif
{
  import_array();

#include "pyopencv_generated_type_reg.h"

#if PY_MAJOR_VERSION >= 3
  PyObject* m = PyModule_Create(&cv2_contrib_moduledef);
#else
  PyObject* m = Py_InitModule(MODULESTR, special_methods);
#endif
  init_submodules(m); // from "pyopencv_generated_ns_reg.h"

  PyObject* d = PyModule_GetDict(m);

  PyDict_SetItemString(d, "__version__", PyString_FromString(CV_VERSION));

  opencv_error = PyErr_NewException((char*)MODULESTR".error", NULL, NULL);
  PyDict_SetItemString(d, "error", opencv_error);

#define PUBLISH(I) PyDict_SetItemString(d, #I, PyInt_FromLong(I))
//#define PUBLISHU(I) PyDict_SetItemString(d, #I, PyLong_FromUnsignedLong(I))
#define PUBLISH2(I, value) PyDict_SetItemString(d, #I, PyLong_FromLong(value))

  PUBLISH(CV_8U);
  PUBLISH(CV_8UC1);
  PUBLISH(CV_8UC2);
  PUBLISH(CV_8UC3);
  PUBLISH(CV_8UC4);
  PUBLISH(CV_8S);
  PUBLISH(CV_8SC1);
  PUBLISH(CV_8SC2);
  PUBLISH(CV_8SC3);
  PUBLISH(CV_8SC4);
  PUBLISH(CV_16U);
  PUBLISH(CV_16UC1);
  PUBLISH(CV_16UC2);
  PUBLISH(CV_16UC3);
  PUBLISH(CV_16UC4);
  PUBLISH(CV_16S);
  PUBLISH(CV_16SC1);
  PUBLISH(CV_16SC2);
  PUBLISH(CV_16SC3);
  PUBLISH(CV_16SC4);
  PUBLISH(CV_32S);
  PUBLISH(CV_32SC1);
  PUBLISH(CV_32SC2);
  PUBLISH(CV_32SC3);
  PUBLISH(CV_32SC4);
  PUBLISH(CV_32F);
  PUBLISH(CV_32FC1);
  PUBLISH(CV_32FC2);
  PUBLISH(CV_32FC3);
  PUBLISH(CV_32FC4);
  PUBLISH(CV_64F);
  PUBLISH(CV_64FC1);
  PUBLISH(CV_64FC2);
  PUBLISH(CV_64FC3);
  PUBLISH(CV_64FC4);

#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}