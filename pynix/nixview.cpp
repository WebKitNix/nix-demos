#include <Python.h>
#include <WebKit2/WKPage.h>
#include <WebKit2/WKString.h>
#include <WebKit2/WKType.h>
#include <NIXView.h>
#include "nixmouseevent.h"

extern "C" {

typedef struct {
    PyObject_HEAD
    WKViewRef cptr;
    WKViewClientV0 viewClient;
    NIXViewClientV0 nixViewClient;
    PyObject* viewNeedsDisplayCallback;
} NIXViewObject;

extern WKContextRef wkContextRef(PyObject*);
extern PyObject* wrapPageRef(WKPageRef);

static void viewNeedsDisplayCallback(WKViewRef nixView, WKRect, const void* clientInfo)
{
    NIXViewObject* cppSelf = reinterpret_cast<NIXViewObject*>(const_cast<void*>(clientInfo));
    PyObject_CallFunctionObjArgs(cppSelf->viewNeedsDisplayCallback, reinterpret_cast<PyObject*>(cppSelf), NULL);
}

static int NIXView_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    PyObject* pyContext = 0;
    int ok = PyArg_ParseTuple(args, "O", &pyContext);
    if (!ok)
        return -1;

    NIXViewObject* cppSelf = reinterpret_cast<NIXViewObject*>(self);
    WKContextRef contextRef = wkContextRef(pyContext);
    cppSelf->cptr = WKViewCreate(contextRef, NULL);

    memset(&cppSelf->nixViewClient, 0, sizeof(NIXViewClientV0));
    cppSelf->nixViewClient.base.clientInfo = self;
    cppSelf->nixViewClient.base.version = 0;

    memset(&cppSelf->viewClient, 0, sizeof(WKViewClientV0));
    cppSelf->viewClient.base.version = 0;
    cppSelf->viewClient.base.clientInfo = self;
    cppSelf->viewClient.viewNeedsDisplay = viewNeedsDisplayCallback;

    return 1;
}

static void NIXView_Dealloc(PyObject* self)
{
    NIXViewObject* cppSelf = reinterpret_cast<NIXViewObject*>(self);
    WKRelease(cppSelf->cptr);
}

static PyObject* NIXView_getPage(PyObject* self)
{
    NIXViewObject* cppSelf = reinterpret_cast<NIXViewObject*>(self);
    return wrapPageRef(WKViewGetPage(cppSelf->cptr));
}

static PyObject* NIXView_initialize(PyObject* self)
{
    NIXViewObject* cppSelf = reinterpret_cast<NIXViewObject*>(self);

    WKViewInitialize(cppSelf->cptr);

    Py_RETURN_NONE;
}

static PyObject* NIXView_setSize(PyObject* self, PyObject* pyArgs)
{
    int width = 0;
    int height = 0;
    int ok = PyArg_ParseTuple(pyArgs, "ii", &width, &height);
    if (!ok)
        return 0;

    NIXViewObject* cppSelf = reinterpret_cast<NIXViewObject*>(self);

    WKViewSetSize(cppSelf->cptr, WKSizeMake(width, height));

    Py_RETURN_NONE;
}

static PyObject* NIXView_paintToCurrentGLContext(PyObject* self)
{
    NIXViewObject* cppSelf = reinterpret_cast<NIXViewObject*>(self);

    WKViewPaintToCurrentGLContext(cppSelf->cptr);

    Py_RETURN_NONE;
}

static PyObject* NIXView_setViewNeedsDisplayCallback(PyObject* self, PyObject* pyArg)
{
    if (!PyCallable_Check(pyArg))
        return 0;

    NIXViewObject* cppSelf = reinterpret_cast<NIXViewObject*>(self);
    cppSelf->viewNeedsDisplayCallback = pyArg;
    WKViewSetViewClient(cppSelf->cptr, &cppSelf->viewClient.base);

    Py_RETURN_NONE;
}

static PyObject* NIXView_sendMouseEvent(PyObject* self, PyObject* pyArgs)
{
    PyObject *pyMouseEvent = NULL;
    int ok = PyArg_ParseTuple(pyArgs, "O", &pyMouseEvent);
    if (!ok || !pyMouseEvent)
        return NULL;

    NIXViewObject* cppSelf = reinterpret_cast<NIXViewObject*>(self);
    NIXMouseEventObject* mouseEvent = reinterpret_cast<NIXMouseEventObject*>(pyMouseEvent);

    if (!cppSelf || !mouseEvent)
        return NULL;

    NIXViewSendMouseEvent(cppSelf->cptr, &mouseEvent->event);

    Py_RETURN_NONE;
}

static PyMethodDef NIXView_methods[] = {
    {"getPage", (PyCFunction)NIXView_getPage, METH_NOARGS},
    {"initialize", (PyCFunction)NIXView_initialize, METH_NOARGS},
    {"setSize", (PyCFunction)NIXView_setSize, METH_VARARGS},
    {"paintToCurrentGLContext", (PyCFunction)NIXView_paintToCurrentGLContext, METH_NOARGS},
    {"setViewNeedsDisplayCallback", (PyCFunction)NIXView_setViewNeedsDisplayCallback, METH_O},
    {"sendMouseEvent", (PyCFunction)NIXView_sendMouseEvent, METH_VARARGS},
    {0} // Sentinel
};

PyTypeObject NIXViewObjectType = {
    PyObject_HEAD_INIT(&PyType_Type)
    /* ob_size           */ 0,
    /* tp_name           */ "nix.View",
    /* tp_basicsize      */ sizeof(NIXViewObject),
    /* tp_itemsize       */ 0,
    /* tp_dealloc        */ NIXView_Dealloc,
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
    /* tp_doc            */ "NIX web view.",
    /* tp_traverse       */ 0,
    /* tp_clear          */ 0,
    /* tp_richcompare    */ 0,
    /* tp_weaklistoffset */ 0,
    /* tp_iter           */ 0,
    /* tp_iternext       */ 0,
    /* tp_methods        */ NIXView_methods,
    /* tp_members        */ 0,
    /* tp_getset         */ 0,
    /* tp_base           */ 0,
    /* tp_dict           */ 0,
    /* tp_descr_get      */ 0,
    /* tp_descr_set      */ 0,
    /* tp_dictoffset     */ 0,
    /* tp_init           */ NIXView_Init,
    /* tp_alloc          */ 0,
    /* tp_new            */ PyType_GenericNew,
};

}
