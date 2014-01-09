#ifndef NIXMOUSEEVENT_H
#define NIXMOUSEEVENT_H

#include <Python.h>
#include <WebKit2/WKType.h>
#include <NIXEvents.h>

extern "C" {

typedef struct {
    PyObject_HEAD
    NIXMouseEvent event;
} NIXMouseEventObject;

extern PyTypeObject NIXMouseEventObjectType;

}

#endif // NIXMOUSEEVENT_H
