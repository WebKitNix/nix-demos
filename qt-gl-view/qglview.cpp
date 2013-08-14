#include <QMouseEvent>
#include <QKeyEvent>
#include <QDateTime>
#include <GL/gl.h>
#include <stdlib.h>
#include <WebKit2/WKURL.h>
#include <WebKit2/WKType.h>
#include <NIXView.h>
#include "qglview.h"

QGLView::QGLView(QWidget *parent)
    : QGLWidget(parent)
{
    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
    setAttribute(Qt::WA_InputMethodEnabled, true);
}

QGLView::~QGLView()
{
    releaseWebKitWrapper();
}

void QGLView::releaseWebKitWrapper()
{
    WKPageClose(m_webKitWrapper->webPage);
    WKRelease(m_webKitWrapper->webContext);
    WKRelease(m_webKitWrapper->webView);
}

WebKitWrapper *QGLView::webKitWrapper() const
{
    return m_webKitWrapper;
}

void QGLView::viewNeedsDisplay(WKViewRef view, WKRect rect, const void *clientInfo)
{
    QGLView *qglView = (QGLView*) clientInfo;
    qglView->updateGL();
}

WKPageRef QGLView::createNewPage(WKPageRef page, WKURLRequestRef urlRequest, WKDictionaryRef features,
            WKEventModifiers modifiers, WKEventMouseButton mouseButton, const void *clientInfo)
{
    QGLView *qglView = (QGLView*) clientInfo;
    QGLView *newWindow = new QGLView(qglView);
    newWindow->initWebKitWrapper(page);

    WKRetain(newWindow->webKitWrapper()->webPage);

    newWindow->show();

    return newWindow->webKitWrapper()->webPage;
}

void QGLView::initWebKitWrapper(WKPageRef parentPage)
{
    m_webKitWrapper = new WebKitWrapper();
    m_webKitWrapper->webContext = parentPage ? WKPageGetContext(parentPage) : WKContextCreate();
    m_webKitWrapper->webView = WKViewCreate(m_webKitWrapper->webContext, parentPage ? WKPageGetPageGroup(parentPage) : NULL);
    m_webKitWrapper->webPage = WKViewGetPage(m_webKitWrapper->webView);
    NIXViewSetAutoScaleToFitContents(m_webKitWrapper->webView, false);

    memset(&m_webKitWrapper->webViewClient, 0, sizeof(WKViewClient));
    m_webKitWrapper->webViewClient.version = kWKViewClientCurrentVersion;
    m_webKitWrapper->webViewClient.clientInfo = this;
    m_webKitWrapper->webViewClient.viewNeedsDisplay = QGLView::viewNeedsDisplay;

    memset(&m_webKitWrapper->pageUIClient, 0, sizeof(WKPageUIClient));
    m_webKitWrapper->pageUIClient.version = kWKPageUIClientCurrentVersion;
    m_webKitWrapper->pageUIClient.createNewPage = QGLView::createNewPage;
    WKPageSetPageUIClient(m_webKitWrapper->webPage, &m_webKitWrapper->pageUIClient);

    WKViewSetViewClient(m_webKitWrapper->webView, &m_webKitWrapper->webViewClient);
    WKViewInitialize(m_webKitWrapper->webView);
}

void QGLView::loadUrl(const QString &url)
{
    QByteArray urlArray;
    urlArray = url.toUtf8();
    WKURLRef urlRef = WKURLCreateWithUTF8CString(urlArray.constData());
    WKPageLoadURL(m_webKitWrapper->webPage, urlRef);
    WKRelease(urlRef);
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

NIXKeyEvent QGLView::nixKeyEvent()
{
    NIXKeyEvent nixEvent;
    memset(&nixEvent, 0, sizeof(NIXKeyEvent));
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

void QGLView::inputMethodEvent(QInputMethodEvent *event)
{
    NIXKeyEvent nixEvent = nixKeyEvent();
    QByteArray utf8Data = event->commitString().toUtf8();
    nixEvent.text = utf8Data.constData();
    nixEvent.type = kNIXInputEventTypeKeyDown;
    NIXViewSendKeyEvent(m_webKitWrapper->webView, &nixEvent);
    event->accept();
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

void QGLView::sendKeyPressOrReleaseEvent(QKeyEvent *event)
{
    NIXKeyEvent nixEvent = nixKeyEvent();
    nixEvent.type = event->type() == QEvent::KeyPress ? kNIXInputEventTypeKeyDown : kNIXInputEventTypeKeyUp;
    nixEvent.key = (NIXKeyEventKey) event->key();
    if (event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z)
        nixEvent.shouldUseUpperCase = (event->modifiers() & Qt::ShiftModifier) && !(event->nativeModifiers() & 2) ||
                                      !(event->modifiers() & Qt::ShiftModifier) && (event->nativeModifiers() & 2);

    nixEvent.isKeypad = event->modifiers() & Qt::KeypadModifier;
    NIXViewSendKeyEvent(m_webKitWrapper->webView, &nixEvent);
}

void QGLView::keyPressEvent(QKeyEvent *event)
{
    sendKeyPressOrReleaseEvent(event);
}

void QGLView::keyReleaseEvent(QKeyEvent *event)
{
    sendKeyPressOrReleaseEvent(event);
}
