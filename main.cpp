
#include <glib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <WebKit2/WKContext.h>
#include <WebKit2/WKPage.h>
#include <WebKit2/WKString.h>
#include <WebKit2/WKType.h>
#include <WebKit2/WKURL.h>
#include <NIXView.h>
#include <iostream>
#include <string.h>

using namespace std;

static struct {
	GMainLoop* mainLoop;
	WKContextRef context;
	NIXView webView;
	WKPageRef page;
	NIXViewClient viewClient;
	WKPageLoaderClient loaderClient;
} browser;

static void viewNeedsDisplay(NIXView webView, WKRect, const void*)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    NIXViewPaintToCurrentGLContext(webView);
    glutSwapBuffers();
}

void browser_init(const char *url)
{
	  browser.mainLoop = g_main_loop_new(0, false);

    browser.context = WKContextCreate();
    browser.webView = NIXViewCreate(browser.context, NULL);
    browser.page = NIXViewGetPage(browser.webView);

    
    memset(&browser.viewClient, 0, sizeof(NIXViewClient));
    browser.viewClient.version = kNIXViewClientCurrentVersion;
    browser.viewClient.viewNeedsDisplay = viewNeedsDisplay;

    NIXViewSetViewClient(browser.webView, &browser.viewClient);

    NIXViewInitialize(browser.webView);

    memset(&browser.loaderClient, 0, sizeof(browser.loaderClient));
    browser.loaderClient.version = kWKPageLoaderClientCurrentVersion;
    // browser.loaderClient.didReceiveTitleForFrame = didReceiveTitleForFrame;
    WKPageSetPageLoaderClient(browser.page, &browser.loaderClient);

    NIXViewSetSize(browser.webView, WKSizeMake(500, 500));
    WKPageLoadURL(browser.page, WKURLCreateWithUTF8CString(url));
}

void browser_loop(void)
{
	GMainContext *mainContext = g_main_loop_get_context(browser.mainLoop);
	g_main_context_iteration(mainContext, true);
}

void browser_quit(void)
{
	NIXViewRelease(browser.webView);
    WKRelease(browser.context);
	g_main_loop_unref(browser.mainLoop);
}

int main(int argc, char **argv)
{
	browser_init("http://google.com");

	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  	glutInitWindowSize(500, 500);
  	glutInitWindowPosition(300, 200);
  	glutCreateWindow("Hello World!");

	glutIdleFunc(browser_loop);
  	glutMainLoop();

  	browser_quit();

	return 0;
}