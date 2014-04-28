/**
 * @author Andre Anjos <andre.anjos@idiap.ch>
 * @date Thu 24 Apr 17:32:07 2014 CEST
 *
 * @brief C/C++ API for bob::machine
 */

#ifndef XBOB_LEARN_MLP_H
#define XBOB_LEARN_MLP_H

#include <Python.h>
#include <xbob.learn.mlp/config.h>
#include <bob/machine/MLP.h>

#define XBOB_LEARN_MLP_MODULE_PREFIX xbob.learn.mlp
#define XBOB_LEARN_MLP_MODULE_NAME _library

/*******************
 * C API functions *
 *******************/

/* Enum defining entries in the function table */
enum _PyBobLearnMLP_ENUM{
  PyXbobLearnMLP_APIVersion_NUM = 0,
  // Bindings for xbob.learn.mlp.Machine
  PyBobLearnMLPMachine_Type_NUM,
  PyBobLearnMLPMachine_Check_NUM,
  PyBobLearnMLPMachine_NewFromSize_NUM,
  // Total number of C API pointers
  PyXbobLearnMLP_API_pointers
};

/**************
 * Versioning *
 **************/

#define PyXbobLearnMLP_APIVersion_TYPE int

/******************************************
 * Bindings for xbob.learn.mlp.Machine *
 ******************************************/

typedef struct {
  PyObject_HEAD
  bob::machine::MLP* cxx;
} PyBobLearnMLPMachineObject;

#define PyBobLearnMLPMachine_Type_TYPE PyTypeObject

#define PyBobLearnMLPMachine_Check_RET int
#define PyBobLearnMLPMachine_Check_PROTO (PyObject* o)

#define PyBobLearnMLPMachine_NewFromSize_RET PyObject*
#define PyBobLearnMLPMachine_NewFromSize_PROTO (Py_ssize_t i, Py_ssize_t o)

#ifdef XBOB_LEARN_MLP_MODULE

  /* This section is used when compiling `xbob.learn.mlp' itself */

  /**************
   * Versioning *
   **************/

  extern int PyXbobLearnMLP_APIVersion;

  /******************************************
   * Bindings for xbob.learn.mlp.Machine *
   ******************************************/

  extern PyBobLearnMLPMachine_Type_TYPE PyBobLearnMLPMachine_Type;

  PyBobLearnMLPMachine_Check_RET PyBobLearnMLPMachine_Check PyBobLearnMLPMachine_Check_PROTO;

  PyBobLearnMLPMachine_NewFromSize_RET PyBobLearnMLPMachine_NewFromSize PyBobLearnMLPMachine_NewFromSize_PROTO;

#else

  /* This section is used in modules that use `xbob.learn.mlp's' C-API */

/************************************************************************
 * Macros to avoid symbol collision and allow for separate compilation. *
 * We pig-back on symbols already defined for NumPy and apply the same  *
 * set of rules here, creating our own API symbol names.                *
 ************************************************************************/

#  if defined(PY_ARRAY_UNIQUE_SYMBOL)
#    define XBOB_LEARN_MLP_MAKE_API_NAME_INNER(a) XBOB_LEARN_MLP_ ## a
#    define XBOB_LEARN_MLP_MAKE_API_NAME(a) XBOB_LEARN_MLP_MAKE_API_NAME_INNER(a)
#    define PyXbobLearnMLP_API XBOB_LEARN_MLP_MAKE_API_NAME(PY_ARRAY_UNIQUE_SYMBOL)
#  endif

#  if defined(NO_IMPORT_ARRAY)
  extern void **PyXbobLearnMLP_API;
#  else
#    if defined(PY_ARRAY_UNIQUE_SYMBOL)
  void **PyXbobLearnMLP_API;
#    else
  static void **PyXbobLearnMLP_API=NULL;
#    endif
#  endif

  /**************
   * Versioning *
   **************/

# define PyXbobLearnMLP_APIVersion (*(PyXbobLearnMLP_APIVersion_TYPE *)PyXbobLearnMLP_API[PyXbobLearnMLP_APIVersion_NUM])

  /******************************************
   * Bindings for xbob.learn.mlp.Machine *
   ******************************************/

# define PyBobLearnMLPMachine_Type (*(PyBobLearnMLPMachine_Type_TYPE *)PyXbobLearnMLP_API[PyBobLearnMLPMachine_Type_NUM])

# define PyBobLearnMLPMachine_Check (*(PyBobLearnMLPMachine_Check_RET (*)PyBobLearnMLPMachine_Check_PROTO) PyXbobLearnMLP_API[PyBobLearnMLPMachine_Check_NUM])

# define PyBobLearnMLPMachine_NewFromSize (*(PyBobLearnMLPMachine_NewFromSize_RET (*)PyBobLearnMLPMachine_NewFromSize_PROTO) PyXbobLearnMLP_API[PyBobLearnMLPMachine_NewFromSize_NUM])

# if !defined(NO_IMPORT_ARRAY)

  /**
   * Returns -1 on error, 0 on success. PyCapsule_Import will set an exception
   * if there's an error.
   */
  static int import_xbob_learn_mlp(void) {

    PyObject *c_api_object;
    PyObject *module;

    module = PyImport_ImportModule(BOOST_PP_STRINGIZE(XBOB_LEARN_MLP_MODULE_PREFIX) "." BOOST_PP_STRINGIZE(XBOB_LEARN_MLP_MODULE_NAME));

    if (module == NULL) return -1;

    c_api_object = PyObject_GetAttrString(module, "_C_API");

    if (c_api_object == NULL) {
      Py_DECREF(module);
      return -1;
    }

#   if PY_VERSION_HEX >= 0x02070000
    if (PyCapsule_CheckExact(c_api_object)) {
      PyXbobLearnMLP_API = (void **)PyCapsule_GetPointer(c_api_object,
          PyCapsule_GetName(c_api_object));
    }
#   else
    if (PyCObject_Check(c_api_object)) {
      XbobLearnMLP_API = (void **)PyCObject_AsVoidPtr(c_api_object);
    }
#   endif

    Py_DECREF(c_api_object);
    Py_DECREF(module);

    if (!XbobLearnMLP_API) {
      PyErr_Format(PyExc_ImportError,
#   if PY_VERSION_HEX >= 0x02070000
          "cannot find C/C++ API capsule at `%s.%s._C_API'",
#   else
          "cannot find C/C++ API cobject at `%s.%s._C_API'",
#   endif
          BOOST_PP_STRINGIZE(XBOB_LEARN_MLP_MODULE_PREFIX),
          BOOST_PP_STRINGIZE(XBOB_LEARN_MLP_MODULE_NAME));
      return -1;
    }

    /* Checks that the imported version matches the compiled version */
    int imported_version = *(int*)PyXbobLearnMLP_API[PyXbobLearnMLP_APIVersion_NUM];

    if (XBOB_LEARN_MLP_API_VERSION != imported_version) {
      PyErr_Format(PyExc_ImportError, "%s.%s import error: you compiled against API version 0x%04x, but are now importing an API with version 0x%04x which is not compatible - check your Python runtime environment for errors", BOOST_PP_STRINGIZE(XBOB_LEARN_MLP_MODULE_PREFIX), BOOST_PP_STRINGIZE(XBOB_LEARN_MLP_MODULE_NAME), XBOB_LEARN_MLP_API_VERSION, imported_version);
      return -1;
    }

    /* If you get to this point, all is good */
    return 0;

  }

# endif //!defined(NO_IMPORT_ARRAY)

#endif /* XBOB_LEARN_MLP_MODULE */

#endif /* XBOB_LEARN_MLP_H */