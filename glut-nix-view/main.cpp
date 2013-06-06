#include <glib.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <WebKit2/WKContext.h>
#include <WebKit2/WKPage.h>
#include <WebKit2/WKString.h>
#include <WebKit2/WKType.h>
#include <WebKit2/WKURL.h>
#include <NIXView.h>
#include <NIXEvents.h>
#include <stdio.h>
#include <string.h>

#define WIDTH   800
#define HEIGHT  600
#define XPOS    300
#define YPOS    200

static struct {
    GMainLoop* mainLoop;
    WKContextRef context;
    WKViewRef webView;
    WKPageRef page;
    WKViewClient viewClient;
    WKPageLoaderClient loaderClient;
    int window;
} browser;

double currentTime()
{
    GTimeVal now;
    g_get_current_time(&now);
    return static_cast<double>(now.tv_sec) + static_cast<double>(now.tv_usec / 1000000.0);
}

static void updateDisplay()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    WKViewPaintToCurrentGLContext(browser.webView);
    glutSwapBuffers();
}

static void viewNeedsDisplay(WKViewRef, WKRect, const void*)
{
    updateDisplay();
}

static void didReceiveTitleForFrame(WKPageRef page, WKStringRef title, WKFrameRef frame, WKTypeRef, const void*)
{
    if (page != browser.page)
        return;

    char buffer[256];
    size_t size = WKStringGetUTF8CString(title, buffer, sizeof(buffer) - 1);
    buffer[size] = 0;
    printf("Title: '%s'\n", buffer);
    glutSetWindowTitle(buffer);
}

static void browser_init(const char* url)
{
    browser.mainLoop = g_main_loop_new(0, false);

    WKStringRef wkStr = WKStringCreateWithUTF8CString(INJECTEDBUNDLE_DIR "libBundle.so");
    browser.context = WKContextCreateWithInjectedBundlePath(wkStr);
    WKRelease(wkStr);
    browser.webView = WKViewCreate(browser.context, NULL);
    browser.page = WKViewGetPage(browser.webView);

    memset(&browser.viewClient, 0, sizeof(WKViewClient));
    browser.viewClient.version = kWKViewClientCurrentVersion;
    browser.viewClient.viewNeedsDisplay = viewNeedsDisplay;

    WKPageSetUseFixedLayout(browser.page, true);

    WKViewSetViewClient(browser.webView, &browser.viewClient);

    WKViewInitialize(browser.webView);

    memset(&browser.loaderClient, 0, sizeof(browser.loaderClient));
    browser.loaderClient.version = kWKPageLoaderClientCurrentVersion;
    browser.loaderClient.didReceiveTitleForFrame = didReceiveTitleForFrame;
    WKPageSetPageLoaderClient(browser.page, &browser.loaderClient);

    WKViewSetSize(browser.webView, WKSizeMake(WIDTH, HEIGHT));
    WKPageLoadURL(browser.page, WKURLCreateWithUTF8CString(url));
}

static void browser_loop()
{
    GMainContext* mainContext = g_main_loop_get_context(browser.mainLoop);
    g_main_context_iteration(mainContext, false);
}

static void mouse_position(NIXMouseEvent *event, int x, int y)
{
    WKPoint contentsPoint = WKViewUserViewportToContents(browser.webView, WKPointMake(x, y));
    event->x = contentsPoint.x;
    event->y = contentsPoint.y;
    event->globalX = x;
    event->globalY = y;
}

static void browser_mouse_move(int x, int y)
{
    struct NIXMouseEvent event;
    memset(&event, 0, sizeof(NIXMouseEvent));
    event.type = kNIXInputEventTypeMouseMove;
    event.button = kWKEventMouseButtonNoButton;
    mouse_position(&event, x, y);
    event.timestamp = currentTime();
    NIXViewSendMouseEvent(browser.webView, &event);
}

static void browser_mouse(int button, int state, int x, int y)
{
    struct NIXMouseEvent event;
    memset(&event, 0, sizeof(NIXMouseEvent));
    event.button = kWKEventMouseButtonLeftButton;
    event.type = (state == GLUT_DOWN) ? kNIXInputEventTypeMouseDown : kNIXInputEventTypeMouseUp;
    event.clickCount = 1;
    mouse_position(&event, x, y);
    event.timestamp = currentTime();
    NIXViewSendMouseEvent(browser.webView, &event);
}

static void browser_resize(int width, int height)
{
    if (!browser.webView)
        return;

    glViewport(0, 0, width, height);
    WKViewSetSize(browser.webView, WKSizeMake(width, height));
    updateDisplay();
}

static void browser_close()
{
    glutLeaveMainLoop();
}

static void browser_quit()
{
    WKRelease(browser.webView);
    WKRelease(browser.context);
    g_main_loop_unref(browser.mainLoop);
}

int main(int argc, char** argv)
{
    const char* url = argc == 2 ? argv[1] : "http://nix.openbossa.org";
    browser_init(url);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(XPOS, YPOS);
    glutCloseFunc(browser_close);
    browser.window = glutCreateWindow("Loading...");

    glutIdleFunc(browser_loop);
    glutMouseFunc(browser_mouse);
    glutMotionFunc(browser_mouse_move);
    glutPassiveMotionFunc(browser_mouse_move);
    glutReshapeFunc(browser_resize);
    glutMainLoop();

    browser_quit();

    return 0;
}
