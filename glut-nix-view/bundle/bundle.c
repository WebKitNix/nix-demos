#include <WebKit2/WKBundle.h>
#include <WebKit2/WKBundleFrame.h>
#include <WebKit2/WKBundlePage.h>
#include <WebKit2/WKString.h>
#include <WebKit2/WKStringPrivate.h>
#include <WebKit2/WKType.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define BUNDLE_EXPORT __attribute__ ((visibility("default")))

struct bundle {
    WKBundleRef bundleRef;
    WKBundleClient client;
    JSObjectRef window;
    JSGlobalContextRef context;
};

static struct bundle globalBundle;

static JSValueRef _debug(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    if (argumentCount == 0)
        return JSValueMakeNull(ctx);

    JSStringRef str = JSValueToStringCopy(ctx, arguments[0], 0);
    size_t size = JSStringGetMaximumUTF8CStringSize(str);
    char buffer[size];
    JSStringGetUTF8CString(str, buffer, size);
    printf("%s\n", buffer);
    JSStringRelease(str);

    return JSValueMakeNull(ctx);
}

static void register_debug(struct bundle* bundle)
{
    assert(bundle->context);

    JSStringRef funcName = JSStringCreateWithUTF8CString("_debug");
    JSObjectRef jsFunc = JSObjectMakeFunctionWithCallback(bundle->context, funcName, _debug);
    JSObjectSetProperty(bundle->context, bundle->window, funcName, jsFunc, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete, 0);
    JSStringRelease(funcName);
}

static void didClearWindowForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleScriptWorldRef world, const void *clientInfo)
{
    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContextForWorld(frame, world);

    struct bundle* bundle = ((struct bundle*) clientInfo);
    bundle->window = JSContextGetGlobalObject(context);
    bundle->context = context;

    register_debug(bundle);
}

static void didCreatePage(WKBundleRef bundleRef, WKBundlePageRef page, const void* clientInfo)
{
    WKBundlePageLoaderClient loaderClient;
    memset(&loaderClient, 0, sizeof(WKBundlePageLoaderClient));
    loaderClient.version = kWKBundlePageLoaderClientCurrentVersion;
    loaderClient.clientInfo = clientInfo;
    loaderClient.didClearWindowObjectForFrame = didClearWindowForFrame;

    WKBundlePageSetPageLoaderClient(page, &loaderClient);

    WKBundlePageUIClient uiClient;
    memset(&uiClient, 0, sizeof(WKBundlePageUIClient));
    uiClient.version = kWKBundlePageUIClientCurrentVersion;
    WKBundlePageSetUIClient(page, &uiClient);
}

BUNDLE_EXPORT void WKBundleInitialize(WKBundleRef bundleRef, WKTypeRef initializationUserData)
{
    globalBundle.bundleRef = bundleRef;
    memset(&globalBundle.client, 0, sizeof(WKBundleClient));
    globalBundle.client.version = kWKBundleClientCurrentVersion;
    globalBundle.client.clientInfo = &globalBundle;
    globalBundle.client.didCreatePage = didCreatePage;
    WKBundleSetClient(bundleRef, &globalBundle.client);
}
