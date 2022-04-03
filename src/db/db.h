#pragma once

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

namespace DB {
    extern "C" {
        void init();
        void exec(char* query);
        int getKeys(const char* op, const char* filter, const char* keyc);
    }
}