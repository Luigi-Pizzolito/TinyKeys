#include "db.h"
#include <stdlib.h>
#include <string>
#include <iostream>

namespace DB {
    // bool dbR = true;

    static int callback(void *NotUsed, int argc, char **argv, char **azColName){
        // dbR=true;
        return 0;
    }
    void exec(char* query) {
        // while (!dbR) {}
        // dbR = false;

        sqlite3 *db;
        int rc;
        rc = sqlite3_open("keys.db", &db);
        if( rc ){
            printf("Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }

        char *zErrMsg = 0;

        rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
        if( rc!=SQLITE_OK ){
            printf("SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }

        sqlite3_close(db);
    }

    bool keysReady = false;
    int keysVal;
    static int callback2(void *NotUsed, int argc, char **argv, char **azColName) {
        // for(int i=0; i<argc; i++){
        //     printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        // }
        // printf("\n");
        keysReady = true;
        // dbR=true;
        keysVal = argv[0] ? strtol(argv[0],NULL,10) : 0;
        return 0;
    }
    int getKeys(const char* op, const char* filter, const char* keyc) {
        // while (!dbR) {}
        // dbR=false;
        std::string dest = "SELECT ";
        dest+=op;
        dest+="(keyi) FROM keys WHERE tsid LIKE '";
        dest+=filter;
        dest+="%";
        dest+=keyc;
        dest+="';";
        // std::cout << "SQL QUERY: " << dest;

        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;

        rc = sqlite3_open("keys.db", &db);
        if( rc ){
            printf("Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }

        rc = sqlite3_exec(db, const_cast<char*>(dest.c_str()), callback2, 0, &zErrMsg);
        if( rc!=SQLITE_OK ){
            printf("SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }

        sqlite3_close(db);

        while (!keysReady) {}
        keysReady = false;
        // dbR=true;
        return keysVal;
    }


}