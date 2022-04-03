#include "db.hpp"
#include <stdlib.h>

namespace DB {
    static int callback(void *NotUsed, int argc, char **argv, char **azColName){return 0;}
    void exec(char* query) {
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        rc = sqlite3_open("keys.db", &db);
        if( rc ){
            printf("Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }

        rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK ){
            printf("SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }

        sqlite3_close(db);
    }

    void init() {
        
    }
}