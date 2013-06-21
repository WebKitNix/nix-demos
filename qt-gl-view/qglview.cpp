#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <WebKit2/WKURL.h>
#include "qglview.h"

void webKitViewNeedsDisplay(WKViewRef view, WKRect rect, const void *clientInfo)
{
    QGLView *qglView = (QGLView*) clientInfo;
    qglView->updateGL();
}

QGLView::QGLView(QWidget *parent)
    : QGLWidget(parent)
{
    initWebKitWrapper();
}

void QGLView::initWebKitWrapper()
{
    m_webKitWrapper = new WebKitWrapper();
    m_webKitWrapper->webContext = WKContextCreate();
    m_webKitWrapper->webView = WKViewCreate(m_webKitWrapper->webContext, NULL);
    m_webKitWrapper->webPage = WKViewGetPage(m_webKitWrapper->webView);
    memset(&m_webKitWrapper->webViewClient, 0, sizeof(WKViewClient));
    m_webKitWrapper->webViewClient.version = kWKViewClientCurrentVersion;
    m_webKitWrapper->webViewClient.clientInfo = this;
    m_webKitWrapper->webViewClient.viewNeedsDisplay = webKitViewNeedsDisplay;

    WKViewSetViewClient(m_webKitWrapper->webView, &m_webKitWrapper->webViewClient);
    WKViewInitialize(m_webKitWrapper->webView);
    WKPageLoadURL(m_webKitWrapper->webPage, WKURLCreateWithUTF8CString("http://www.google.com"));
}

void QGLView::paintGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    WKViewPaintToCurrentGLContext(m_webKitWrapper->webView);
}

void QGLView::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
    WKViewSetSize(m_webKitWrapper->webView, WKSizeMake(width, height));
}
