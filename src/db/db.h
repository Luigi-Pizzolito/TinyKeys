#pragma once

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

// #include "ipc.h"
#define IPC_BUFF_SQLQUERIES 4096
class DB {
    static DB* DBinst;
    private:

        DB();
        ~DB();

        sqlite3 *db;
        static int Querynocallback(void *NotUsed, int argc, char **argv, char **azColName);
        bool DBbusy;
        int keysVal;
        static int Querycallback(void *NotUsed, int argc, char **argv, char **azColName);

        // ipc_sharedmemory queryBacklog;
        // ipc_sharedsemaphore queryBacklogAccess;
        // unsigned char shm_buf[IPC_BUFF_SQLQUERIES];
        std::vector<char*> queryBacklog;

        void threadinginit();
        void threadingstop();
        void threadingrestart();
        void exec(char* query);
    public:
        static DB& inst();
        static sqlite3* DBconn();
        static void DBconnFlush();

        static void pushExecKeypress(const char* keyCode);
        static void pushWorker();

        static int getKeys(const char* op, const char* filter, const char* keyc);

        static void close();
};