#include <QMouseEvent>
#include <QDateTime>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <WebKit2/WKURL.h>
#include <NIXView.h>
#include "qglview.h"

void webKitViewNeedsDisplay(WKViewRef view, WKRect rect, const void *clientInfo)
{
    QGLView *qglView = (QGLView*) clientInfo;
    qglView->updateGL();
}

QGLView::QGLView(QWidget *parent)
    : QGLWidget(parent)
{
    setMouseTracking(true);
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

NIXMouseEvent QGLView::nixMouseEvent()
{
    NIXMouseEvent nixEvent;
    memset(&nixEvent, 0, sizeof(NIXMouseEvent));
    nixEvent.timestamp = QDateTime::currentDateTime().toTime_t() / 1000.0;
    return nixEvent;
}

void QGLView::fillNIXEventMousePos(NIXMouseEvent &nixEvent, QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    WKPoint contentsPoint = WKViewUserViewportToContents(m_webKitWrapper->webView, WKPointMake(x, y));
    nixEvent.x = contentsPoint.x;
    nixEvent.y = contentsPoint.y;
    nixEvent.globalX = x;
    nixEvent.globalY = y;
}

void QGLView::sendMousePressOrReleaseEvent(QMouseEvent *event)
{
    NIXMouseEvent nixEvent = nixMouseEvent();
    if (event->button() == Qt::LeftButton)
        nixEvent.button = kWKEventMouseButtonLeftButton;
    else if (event->button() == Qt::RightButton)
        nixEvent.button = kWKEventMouseButtonRightButton;
    else if (event->button() == Qt::MiddleButton)
        nixEvent.button = kWKEventMouseButtonMiddleButton;
    else
        nixEvent.button = kWKEventMouseButtonNoButton;

    nixEvent.type = event->type() == QEvent::MouseButtonPress ? kNIXInputEventTypeMouseDown : kNIXInputEventTypeMouseUp;
    nixEvent.clickCount = 1;
    fillNIXEventMousePos(nixEvent, event);
    NIXViewSendMouseEvent(m_webKitWrapper->webView, &nixEvent);
}

void QGLView::mousePressEvent(QMouseEvent *event)
{
    sendMousePressOrReleaseEvent(event);
}

void QGLView::mouseReleaseEvent(QMouseEvent *event)
{
    sendMousePressOrReleaseEvent(event);
}

void QGLView::mouseMoveEvent(QMouseEvent *event)
{
    NIXMouseEvent nixEvent = nixMouseEvent();
    nixEvent.type = kNIXInputEventTypeMouseMove;
    nixEvent.button = kWKEventMouseButtonNoButton;
    fillNIXEventMousePos(nixEvent, event);
    NIXViewSendMouseEvent(m_webKitWrapper->webView, &nixEvent);
}
