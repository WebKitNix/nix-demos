
#include "Bundle.h"
#include <WebKit2/WKBundle.h>
#include <WebKit2/WKBundleFrame.h>
#include <WebKit2/WKBundlePage.h>
#include <WebKit2/WKBundleInitialize.h>
#include <WebKit2/WKNumber.h>
#include <WebKit2/WKString.h>
#include <WebKit2/WKStringPrivate.h>
#include <WebKit2/WKType.h>
#include <WebKit2/WKArray.h>

#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include <string>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
// I don't care about windows or gcc < 4.x right now.
#define UIBUNDLE_EXPORT __attribute__ ((visibility("default")))

static Bundle* gBundle = 0;

// WKBundlePageClient

extern "C" {
UIBUNDLE_EXPORT void WKBundleInitialize(WKBundleRef bundle, WKTypeRef initializationUserData)
{
    gBundle = new Bundle(bundle);
}
} // "extern C"

Bundle::Bundle(WKBundleRef bundle)
    : m_bundle(bundle)
    , m_jsContext(0)
    , m_windowObj(0)
{
    WKBundleClient client;
    std::memset(&client, 0, sizeof(WKBundleClient));

    client.version = kWKBundleClientCurrentVersion;
    client.clientInfo = this;
    client.didCreatePage = &Bundle::didCreatePage;

    WKBundleSetClient(bundle, &client);
}

void Bundle::didClearWindowForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleScriptWorldRef world, const void *clientInfo)
{
    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContextForWorld(frame, world);

    Bundle* bundle = ((Bundle*)clientInfo);
    bundle->m_jsContext = context;
    bundle->m_windowObj = JSContextGetGlobalObject(context);

    bundle->registerAPI();
}

void Bundle::didCreatePage(WKBundleRef, WKBundlePageRef page, const void* clientInfo)
{
    WKBundlePageLoaderClient loaderClient;
    std::memset(&loaderClient, 0, sizeof(WKBundlePageLoaderClient));
    loaderClient.version = kWKBundlePageLoaderClientCurrentVersion;
    loaderClient.clientInfo = clientInfo;
    loaderClient.didClearWindowObjectForFrame = &Bundle::didClearWindowForFrame;

    WKBundlePageSetPageLoaderClient(page, &loaderClient);

    WKBundlePageUIClient uiClient;
    std::memset(&uiClient, 0, sizeof(WKBundlePageUIClient));
    uiClient.version = kWKBundlePageUIClientCurrentVersion;
    WKBundlePageSetUIClient(page, &uiClient);
}

void Bundle::registerAPI()
{
    assert(m_jsContext);

    JSStringRef funcName = JSStringCreateWithUTF8CString("_getCpus");
    JSObjectRef jsFunc = JSObjectMakeFunctionWithCallback(m_jsContext, funcName, jsGetCpus);
    JSObjectSetProperty(m_jsContext, m_windowObj, funcName, jsFunc, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete, 0);
    JSStringRelease(funcName);

    funcName = JSStringCreateWithUTF8CString("_getLoad");
    jsFunc = JSObjectMakeFunctionWithCallback(m_jsContext, funcName, jsGetLoad);
    JSObjectSetProperty(m_jsContext, m_windowObj, funcName, jsFunc, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete, 0);
    JSStringRelease(funcName);
}

JSValueRef Bundle::jsGetCpus(JSContextRef ctx, JSObjectRef func, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef*) {
    return JSValueMakeNumber(ctx, sysconf( _SC_NPROCESSORS_ONLN ));
}

JSValueRef Bundle::jsGetLoad(JSContextRef ctx, JSObjectRef func, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef*) {

    static int totalVector[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    static int workVector[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    WKTypeRef param = WKUInt64Create(JSValueToNumber(ctx, arguments[0], 0));;
    int cpu = WKUInt64GetValue((WKUInt64Ref)param);

    FILE *cmdline = fopen("/proc/stat", "rb");
    char *arg = 0;
    size_t size = 0;
    int load = 0;

    while(getline(&arg, &size, cmdline) != -1)
    {
        load = 0;
        char buffer[10];
        sprintf(buffer, "cpu%d", cpu);
        // Find the line that corresponds to the current CPU
        if (strncmp(arg, buffer, 4) == 0) {
            char *tmp = strtok(arg, " ");
            tmp = strtok (NULL, " ");
            int total = 0;
            int work = 0;

            // Compute total usage and work usage to get average load
            while (tmp != NULL)
            {
                total += atoi(tmp);
                if (load < 3)
                    work = total;
                tmp = strtok (NULL, " ");
                load++;
            }
            load = ((work - workVector[cpu]) * 100.0) / (total - totalVector[cpu]);
            totalVector[cpu] = total;
            workVector[cpu] = work;
            break;
        }
    }
    free(arg);
    fclose(cmdline);
    return JSValueMakeNumber(ctx, load);
}
