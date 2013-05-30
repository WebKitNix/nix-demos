#include <glib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <WebKit2/WKContext.h>
#include <WebKit2/WKPage.h>
#include <WebKit2/WKString.h>
#include <WebKit2/WKType.h>
#include <WebKit2/WKURL.h>
#include <NIXView.h>
#include <stdio.h>
#include <string.h>

#define WIDTH   480
#define HEIGHT  360
#define XPOS    0
#define YPOS    0

static struct {
    GMainLoop* mainLoop;
    WKContextRef context;
    WKViewRef webView;
    WKPageRef page;
    WKViewClient viewClient;
    WKPageLoaderClient loaderClient;
} browser;

static void viewNeedsDisplay(WKViewRef webView, WKRect, const void*)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    WKViewPaintToCurrentGLContext(webView);
    glutSwapBuffers();
}

static void didReceiveTitleForFrame(WKPageRef, WKStringRef title, WKFrameRef, WKTypeRef, const void*)
{
    char buffer[256];
    size_t size = WKStringGetUTF8CString(title, buffer, sizeof(buffer) - 1);
    buffer[size] = 0;
    printf("Title: '%s'\n", buffer);
}

void browser_init(const char* url)
{
    browser.mainLoop = g_main_loop_new(0, false);

    browser.context = WKContextCreateWithInjectedBundlePath(WKStringCreateWithUTF8CString(CPUMONITOR_INJECTEDBUNDLE_DIR "libCPUMonitorInjectedBundle.so"));
    browser.webView = WKViewCreate(browser.context, NULL);
    browser.page = WKViewGetPage(browser.webView);

    memset(&browser.viewClient, 0, sizeof(WKViewClient));
    browser.viewClient.version = kWKViewClientCurrentVersion;
    browser.viewClient.viewNeedsDisplay = viewNeedsDisplay;
    WKViewSetViewClient(browser.webView, &browser.viewClient);

    WKViewInitialize(browser.webView);

    memset(&browser.loaderClient, 0, sizeof(browser.loaderClient));
    browser.loaderClient.version = kWKPageLoaderClientCurrentVersion;
    browser.loaderClient.didReceiveTitleForFrame = didReceiveTitleForFrame;
    WKPageSetPageLoaderClient(browser.page, &browser.loaderClient);

    WKViewSetSize(browser.webView, WKSizeMake(WIDTH, HEIGHT));
    WKPageLoadURL(browser.page, WKURLCreateWithUTF8CString(url));
}

void browser_loop()
{
    GMainContext* mainContext = g_main_loop_get_context(browser.mainLoop);
    g_main_context_iteration(mainContext, true);
}

void browser_quit()
{
    WKRelease(browser.webView);
    WKRelease(browser.context);
    g_main_loop_unref(browser.mainLoop);
}

int main(int argc, char** argv)
{
    const char* url = argc == 2 ? argv[1] : CPUMONITOR_UI_FILE;
    fprintf(stderr, "%s\n", url);
    browser_init(url);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(XPOS, YPOS);
    glutCreateWindow("Hello World!");

    glutIdleFunc(browser_loop);
    glutMainLoop();

    browser_quit();

    return 0;
}
