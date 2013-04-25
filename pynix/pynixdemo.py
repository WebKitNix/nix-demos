#!/usr/bin/env python
# -*- coding: utf-8 -*-

import nix

import sys
import glib
from OpenGL.GLUT import *
from OpenGL.GL import *

class Browser(object):
    WIDTH  = 800
    HEIGHT = 600
    XPOS   = 100
    YPOS   = 100

    def __init__(self):
        self.mainLoop = glib.MainLoop(None, False)
        self.context = nix.Context()
        self.nixView = nix.View(self.context)

    def run(self):
        def viewNeedsDisplay(nixView):
            glViewport(0, 0, Browser.WIDTH, Browser.HEIGHT)
            glClearColor(0.4, 0.4, 0.4, 1.0)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
            nixView.paintToCurrentGLContext()
            glutSwapBuffers()

        self.nixView.setViewNeedsDisplayCallback(viewNeedsDisplay)

        self.nixView.initialize()

        self.nixView.setSize(Browser.WIDTH, Browser.HEIGHT)
        page = self.nixView.getPage()
        page.loadURL('http://nix.openbossa.org/')

        glutInit(sys.argv)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
        glutInitWindowSize(Browser.WIDTH, Browser.HEIGHT)
        glutInitWindowPosition(Browser.XPOS, Browser.YPOS)
        glutCreateWindow('PyNIX')

        def nixViewLoop():
            mainContext = self.mainLoop.get_context()
            mainContext.iteration(True)
        glutIdleFunc(nixViewLoop)

        glutMainLoop()

if __name__ == '__main__':
    b = Browser()
    b.run()

