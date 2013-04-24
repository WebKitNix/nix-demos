#ifndef Bundle_h
#define Bundle_h

#include <WebKit2/WKBundle.h>
#include <vector>

class Bundle
{
public:
    Bundle(WKBundleRef);

    void registerAPI();
    static JSValueRef jsGetCpus(JSContextRef ctx, JSObjectRef func, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef*);
    static JSValueRef jsGetLoad(JSContextRef ctx, JSObjectRef func, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef*);

private:
    WKBundleRef m_bundle;
    JSGlobalContextRef m_jsContext;
    JSObjectRef m_windowObj;

    // Bundle client
    static void didCreatePage(WKBundleRef, WKBundlePageRef page, const void* clientInfo);

    // Loader client
    static void didClearWindowForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleScriptWorldRef world, const void* clientInfo);

};

#endif
