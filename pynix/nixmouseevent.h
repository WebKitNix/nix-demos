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

enum NixMouseEvents {
	MOUSE_DOWN_EVENT = 0,
	MOUSE_UP_EVENT = 1,
	MOUSE_MOVE_EVENT = 2
};
typedef NixMouseEvents NixMouseEvents;

extern PyTypeObject NIXMouseEventObjectType;

}

#endif // NIXMOUSEEVENT_H
