#include <Python.h>
#include <WebKit2/WKPage.h>
#include <WebKit2/WKURL.h>

extern "C" {
typedef struct {
    PyObject_HEAD
    WKPageRef cptr;
} WKPageObject;

static PyObject* WKPage_loadURL(PyObject* self, PyObject* pyArg)
{
    const char* url = PyString_AsString(pyArg);
    if (!url)
        return 0;

    WKPageObject* cppSelf = reinterpret_cast<WKPageObject*>(self);

    WKPageLoadURL(cppSelf->cptr, WKURLCreateWithUTF8CString(url));

    Py_RETURN_NONE;
}

static PyMethodDef WKPage_methods[] = {
    {"loadURL", (PyCFunction)WKPage_loadURL, METH_O},
    {0} // Sentinel
};

PyTypeObject WKPageObjectType = {
    PyObject_HEAD_INIT(&PyType_Type)
    /* ob_size           */ 0,
    /* tp_name           */ "nix.Page",
    /* tp_basicsize      */ sizeof(WKPageObject),
    /* tp_itemsize       */ 0,
    /* tp_dealloc        */ 0,
    /* tp_print          */ 0,
    /* tp_getattr        */ 0,
    /* tp_setattr        */ 0,
    /* tp_compare        */ 0,
    /* tp_repr           */ 0,
    /* tp_as_number      */ 0,
    /* tp_as_sequence    */ 0,
    /* tp_as_mapping     */ 0,
    /* tp_hash           */ 0,
    /* tp_call           */ 0,
    /* tp_str            */ 0,
    /* tp_getattro       */ 0,
    /* tp_setattro       */ 0,
    /* tp_as_buffer      */ 0,
    /* tp_flags          */ Py_TPFLAGS_DEFAULT,
    /* tp_doc            */ "WebKit Page.",
    /* tp_traverse       */ 0,
    /* tp_clear          */ 0,
    /* tp_richcompare    */ 0,
    /* tp_weaklistoffset */ 0,
    /* tp_iter           */ 0,
    /* tp_iternext       */ 0,
    /* tp_methods        */ WKPage_methods,
};


WKPageRef wkPageRef(PyObject* self)
{
    return reinterpret_cast<WKPageObject*>(self)->cptr;
}

PyObject* wrapPageRef(WKPageRef pageRef)
{
    WKPageObject* pageObj = PyObject_New(WKPageObject, &WKPageObjectType);
    pageObj->cptr = pageRef;
    return reinterpret_cast<PyObject*>(pageObj);
}
}
