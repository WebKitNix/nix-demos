#!/usr/bin/env python
# -*- coding: utf-8 -*-

import nix

import sys
import os
import glib
from collections import defaultdict
from OpenGL.GLUT import *
from OpenGL.GL import *

class Browser(object):
    XPOS   = 100
    YPOS   = 100
    PATH = os.path.dirname(os.path.abspath(__file__))

    def __init__(self, width=480, height=800):
        self.mainLoop = glib.MainLoop(None, False)
        self.context = nix.Context()
        self.nixView = nix.View(self.context)
        self.windowX = 0
        self.windowY = 0
        self.width = width
        self.height = height
        self.mouseState = defaultdict(int)

    def mouse(self, button, state, x, y):
        e = nix.MouseEvent(button, state, x, y, self.windowX + x, self.windowY + y)
        self.mouseState[button] = state
        print self, button, state, x, y
        self.nixView.sendMouseEvent(e)

    def mouseMove(self, x, y):
        for button in self.mouseState:
            if self.mouseState[button] == GLUT_DOWN:
                break

        e = nix.MouseEvent(button, nix.MOVE_EVENT, x, y, self.windowX + x, self.windowY + y)
        self.nixView.sendMouseEvent(e)

    def run(self):
        def viewNeedsDisplay(nixView):
            glViewport(0, 0, self.width, self.height)
            glClearColor(0.4, 0.4, 0.4, 1.0)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
            nixView.paintToCurrentGLContext()
            glutSwapBuffers()

        self.nixView.setViewNeedsDisplayCallback(viewNeedsDisplay)

        self.nixView.initialize()

        self.nixView.setSize(self.width, self.height)
        page = self.nixView.getPage()
        print('file://%s/index.html' % Browser.PATH)
        page.loadURL('file://%s/ui/index.html' % Browser.PATH)

        glutInit(sys.argv)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
        glutInitWindowSize(self.width, self.height)
        glutInitWindowPosition(Browser.XPOS, Browser.YPOS)
        glutCreateWindow('PyNIX')
        glutMouseFunc(self.mouse)
        glutMotionFunc(self.mouseMove)

        def nixViewLoop():
            mainContext = self.mainLoop.get_context()
            mainContext.iteration(False)
        glutIdleFunc(nixViewLoop)

        glutMainLoop()

if __name__ == '__main__':
    b = Browser()
    b.run()

