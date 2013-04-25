#include <Python.h>

extern PyTypeObject WKContextObjectType;
extern PyTypeObject WKPageObjectType;
extern PyTypeObject NIXViewObjectType;

PyMODINIT_FUNC
initnix(void)
{
    if (PyType_Ready(&WKContextObjectType) < 0)
        return;

    if (PyType_Ready(&WKPageObjectType) < 0)
        return;

    if (PyType_Ready(&NIXViewObjectType) < 0)
        return;

    PyObject* module = Py_InitModule3("nix", NULL, "WebKitNix module.");
    if (module == NULL)
        return;

    Py_INCREF(&WKContextObjectType);
    PyModule_AddObject(module, "Context", (PyObject*)&WKContextObjectType);

    Py_INCREF(&WKPageObjectType);
    PyModule_AddObject(module, "Page", (PyObject*)&WKPageObjectType);

    Py_INCREF(&NIXViewObjectType);
    PyModule_AddObject(module, "View", (PyObject*)&NIXViewObjectType);
}
