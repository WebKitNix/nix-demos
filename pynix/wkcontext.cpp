#include <Python.h>
#include <WebKit2/WKContext.h>
#include <WebKit2/WKString.h>
#include <WebKit2/WKType.h>
#include <NIXView.h>

extern "C" {
typedef struct {
    PyObject_HEAD
    WKContextRef cptr;
} WKContextObject;

WKContextRef wkContextRef(PyObject* self)
{
    return reinterpret_cast<WKContextObject*>(self)->cptr;
}

static int WKContext_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    const char* injectedBundlePath = 0;
    int ok = PyArg_ParseTuple(args, "|s", &injectedBundlePath);
    if (!ok)
        return -1;

    WKContextObject* cppSelf = reinterpret_cast<WKContextObject*>(self);
    if (injectedBundlePath)
        cppSelf->cptr = WKContextCreateWithInjectedBundlePath(WKStringCreateWithUTF8CString(injectedBundlePath));
    else
        cppSelf->cptr = WKContextCreate();

    return 1;
}

static void WKContext_Dealloc(PyObject* self)
{
    WKContextObject* cppSelf = reinterpret_cast<WKContextObject*>(self);
    WKRelease(cppSelf->cptr);
}

PyTypeObject WKContextObjectType = {
    PyObject_HEAD_INIT(&PyType_Type)
    /* ob_size           */ 0,
    /* tp_name           */ "nix.Context",
    /* tp_basicsize      */ sizeof(WKContextObject),
    /* tp_itemsize       */ 0,
    /* tp_dealloc        */ WKContext_Dealloc,
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
    /* tp_doc            */ "WebKit context.",
    /* tp_traverse       */ 0,
    /* tp_clear          */ 0,
    /* tp_richcompare    */ 0,
    /* tp_weaklistoffset */ 0,
    /* tp_iter           */ 0,
    /* tp_iternext       */ 0,
    /* tp_methods        */ 0,
    /* tp_members        */ 0,
    /* tp_getset         */ 0,
    /* tp_base           */ 0,
    /* tp_dict           */ 0,
    /* tp_descr_get      */ 0,
    /* tp_descr_set      */ 0,
    /* tp_dictoffset     */ 0,
    /* tp_init           */ WKContext_Init,
    /* tp_alloc          */ 0,
    /* tp_new            */ PyType_GenericNew,
};
}
