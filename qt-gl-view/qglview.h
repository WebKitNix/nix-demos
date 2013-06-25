#include <QObject>
#include <QGLWidget>
#include <WebKit2/WKContext.h>
#include <WebKit2/WKPage.h>
#include <WebKit2/WKView.h>
#include <NIXEvents.h>

class QMouseEvent;

typedef struct {
    WKContextRef webContext;
    WKViewRef webView;
    WKPageRef webPage;
    WKViewClient webViewClient;
    WKPageLoaderClient webPageLoaderClient;
} WebKitWrapper;

class QGLView : public QGLWidget
{
    Q_OBJECT
public:
    QGLView(QWidget *parent = 0);

private:
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    NIXMouseEvent nixMouseEvent();
    void fillNIXEventMousePos(NIXMouseEvent &nixEvent, QMouseEvent *event);
    void sendMousePressOrReleaseEvent(QMouseEvent *event);

    void initWebKitWrapper();
    WebKitWrapper *m_webKitWrapper;
};
