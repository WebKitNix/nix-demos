#include <QObject>
#include <QGLWidget>
#include <WebKit2/WKContext.h>
#include <WebKit2/WKPage.h>
#include <WebKit2/WKView.h>
#include <NIXEvents.h>

class QMouseEvent;
class QKeyEvent;

typedef struct {
    WKContextRef webContext;
    WKViewRef webView;
    WKPageRef webPage;
    WKViewClient webViewClient;
    WKPageUIClient pageUIClient;
    WKPageLoaderClient webPageLoaderClient;
} WebKitWrapper;

class QGLView : public QGLWidget
{
    Q_OBJECT
public:
    QGLView(QWidget *parent = 0);
    ~QGLView();

    WebKitWrapper *webKitWrapper() const;

    void initWebKitWrapper(WKPageRef parentPage = 0);
    void loadUrl(const QString &url);

protected:
    virtual void inputMethodEvent(QInputMethodEvent *event);

private:
    static void viewNeedsDisplay(WKViewRef view, WKRect rect, const void *clientInfo);
    static WKPageRef createNewPage(WKPageRef page, WKURLRequestRef urlRequest, WKDictionaryRef features,
            WKEventModifiers modifiers, WKEventMouseButton mouseButton, const void *clientInfo);
    void releaseWebKitWrapper();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    NIXMouseEvent nixMouseEvent();
    NIXKeyEvent nixKeyEvent();
    void fillNIXEventMousePos(NIXMouseEvent &nixEvent, QMouseEvent *event);
    void sendMousePressOrReleaseEvent(QMouseEvent *event);
    void sendKeyPressOrReleaseEvent(QKeyEvent *event);

    WebKitWrapper *m_webKitWrapper;
};
