#include <QObject>
#include <QGLWidget>
#include <WebKit2/WKContext.h>
#include <WebKit2/WKPage.h>
#include <WebKit2/WKView.h>

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

    void initWebKitWrapper();
    WebKitWrapper *m_webKitWrapper;
};
