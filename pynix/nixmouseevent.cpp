
#include "nixmouseevent.h"

extern "C" {

static int NIXMouseEvent_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    int x = 0;
    int y = 0;
    int globalX = 0;
    int globalY = 0;
    int ok = PyArg_ParseTuple(args, "iiii", &x, &y, &globalX, &globalY);
    if (!ok)
        return 0;

    NIXMouseEventObject *s = reinterpret_cast<NIXMouseEventObject*>(self);

    s->event.type = kNIXInputEventTypeMouseDown;
    s->event.button = kWKEventMouseButtonLeftButton;
    s->event.x = x;
    s->event.y = y;
    s->event.globalX = globalX;
    s->event.globalY = globalY;
    time_t t;
    s->event.timestamp = ((double) time(&t)) / 1000.0;
    s->event.clickCount = 1;
    s->event.modifiers = 0;

    return 1;
}

PyTypeObject NIXMouseEventObjectType = {
    PyObject_HEAD_INIT(&PyType_Type)
    /* ob_size           */ 0,
    /* tp_name           */ "nix.MouseEvent",
    /* tp_basicsize      */ sizeof(NIXMouseEventObject),
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
    /* tp_doc            */ "NIX mouse event.",
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
    /* tp_init           */ NIXMouseEvent_Init,
    /* tp_alloc          */ 0,
    /* tp_new            */ PyType_GenericNew,
};

}