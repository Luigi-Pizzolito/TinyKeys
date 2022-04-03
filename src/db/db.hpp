#pragma once

#include <stdio.h>
#include <sqlite3.h>

namespace DB {
    extern "C" {
        void exec(char* query);
        void init();
    }
}