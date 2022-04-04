#include "db.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>

#include "ui/uilogic.h"

#include <chrono>
#include <thread>
#define IPC_BUFF_SQLQUERIES 4096

#define IPC_RS_HZ 500


DB* DB::DBinst = &inst();
DB& DB::inst() {
    if (DBinst == nullptr) {
        DBinst = new DB;
    }
    return *DBinst;
}

sqlite3* DB::DBconn() {
    return DB::inst().db;
}

void DB::DBconnFlush() {
    int rc = sqlite3_db_cacheflush(DB::DBconn());
    if( rc ){
        printf("Failed to flush databse to disk: %s\n", sqlite3_errmsg(DB::DBconn()));
        printf("Exiting and loosing changes!");
        exit(1);
    }
}


void DB::threadinginit() {
    ipc_sem_init(&queryBacklogAccess, "DB_queryBacklogAccess");
    if (ipc_sem_create(&queryBacklogAccess, 1)) {
        printf("Creating semaphore failed.\n");
        exit(-1);
    }
    // ipc_sem_decrement(&queryBacklogAccess);
    ipc_mem_init(&queryBacklog, "DB_queryBacklog", IPC_BUFF_SQLQUERIES);
    if (ipc_mem_create(&queryBacklog)) {
        printf("Creating memory failed.\n");
        exit(-1);
    }
    memset(queryBacklog.data, '\0', queryBacklog.size);
    // ipc_sem_increment(&queryBacklogAccess);
    printf("created shared mem\n");
}

void DB::threadingstop() {
    // ipc_sem_decrement(&queryBacklogAccess);
    ipc_mem_close(&queryBacklog);
    // ipc_sem_increment(&queryBacklogAccess);
    ipc_sem_close(&queryBacklogAccess);
}

void DB::threadingrestart() {
    threadingstop();
    threadinginit();
}


DB::DB() {
    int rc;
    rc = sqlite3_open("keys.db", &db);
    if( rc ){
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }


    threadinginit();
    threadingrestart();
    
    DBbusy = false;

    std::cout << "opened DB conn\n";
}

DB::~DB() {
    sqlite3_close(db);
    threadingstop();
    std::cout << "closed DB conn\n";
}


void DB::close() {
    delete DBinst;
}



int DB::Querynocallback(void *NotUsed, int argc, char **argv, char **azColName){
    return 0;
}
void DB::exec(char* query) {
    // std::cout << "exec: DBbusy=" << DBbusy <<"\n";

    int rc;
    char *zErrMsg = 0;

    DBbusy = true;
    rc = sqlite3_exec(db, query, Querynocallback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        printf("DBexec: SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    DBconnFlush();
    DBbusy = false;
    // std::cout << "exec: done: DBbusy=" << DBbusy <<"\n";
}





void DB::pushExecKeypress(const char* keyCode) {
    // DB::inst().queryBacklog.push_back(query);
    ipc_sem_decrement(&DB::inst().queryBacklogAccess);
    if (ipc_mem_open_existing(&DB::inst().queryBacklog)) {
        printf("pushExecKeypress: Opening existing memory failed. Dropping keys, trestarting threading management...\n");
        // DB::inst().prepSharedMem();
        // exit(-1);
        ipc_sem_increment(&DB::inst().queryBacklogAccess);
        DB::inst().threadingrestart();
    } else {

        char buf[IPC_BUFF_SQLQUERIES];
        memset(buf, '\0', IPC_BUFF_SQLQUERIES);
        memcpy(buf, reinterpret_cast<char*>(DB::inst().queryBacklog.data), IPC_BUFF_SQLQUERIES);
        bool startcpy=false;
        size_t cpyoff = 0;
        for (size_t i=0; i<IPC_BUFF_SQLQUERIES;i++) {
            if (buf[i] == '\0') {
                startcpy=true;
            }
            if (startcpy) {
                buf[i] = keyCode[cpyoff++];
                if (keyCode[cpyoff-1] == '\0') {
                    buf[i]='|';
                    buf[i+1]='\0';
                    break;
                }
            }
        }
        
        for (size_t i=0; i<IPC_BUFF_SQLQUERIES;i++) {
            DB::inst().queryBacklog.data[i] = reinterpret_cast<unsigned char &>(buf[i]);
        }
        // std::cout << "buf:" << buf << "\n";
        // DB::inst().queryBacklog.data = reinterpret_cast<unsigned char*>(buf);
        ipc_sem_increment(&DB::inst().queryBacklogAccess);
    }
}
void DB::pushWorker() {
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(IPC_RS_HZ));
        // std::cout << "QUERY BACKLOG:\n";
        // if (ipc_mem_open_existing(&DB::inst().queryBacklog)) {
        //     printf("PushWorker: Opening existing memory failed.\n");
        //     exit(-1);
        // }
        
        // for (auto& query : DB::inst().queryBacklog) {
            // std::cout << "\t" << query << "\n";
            // if (!DB::inst().DBbusy) {
            //     DB::inst().exec(query);
            // }
        // }
        
        // BacklogStr[strlen(BacklogStr)-1] = '\0'; // remove last | delimiter
        // std::cout << "\n data is: " << BacklogStr << "\n";
        ipc_sem_decrement(&DB::inst().queryBacklogAccess);
        if (!DB::inst().DBbusy && !ipc_mem_open_existing(&DB::inst().queryBacklog)) {
            char* BacklogStr =  reinterpret_cast<char*>(DB::inst().queryBacklog.data);
            std::cout << "DBpushWorker: Executing " << std::count(BacklogStr, BacklogStr+IPC_BUFF_SQLQUERIES, '|') << " queries.\n";

            char* q_array = strtok(BacklogStr, "|");
            while (q_array) {
                char dest[200]; //53+3+27+63+3+23+2 ~=200
                memset(dest, '\0', 200);
                strcat( dest, "INSERT OR IGNORE INTO keys VALUES (strftime('%Y%m%d%H-");
                strcat( dest, q_array);
                strcat( dest, "', 'now', 'localtime'), 0);UPDATE keys SET keyi = keyi + 1 WHERE tsid = strftime('%Y%m%d%H-");
                strcat( dest, q_array);
                strcat( dest, "', 'now', 'localtime');");
                strcat( dest, "\0");
                DB::inst().exec(dest);
                // std::cout << "SQL: "<<dest<<"\n";
                q_array = strtok(NULL, "|");
                if (q_array == nullptr) break;
            }
            memset(DB::inst().queryBacklog.data, '\0', IPC_BUFF_SQLQUERIES);
            // DB::inst().exec(BacklogStr);
        }
        ipc_sem_increment(&DB::inst().queryBacklogAccess);

        UILogic::colourKeyboard();
    }
}










int DB::Querycallback(void *NotUsed, int argc, char **argv, char **azColName) {
    DB::inst().keysVal = argv[0] ? strtol(argv[0],NULL,10) : 0;
    DB::inst().DBbusy = false;
    // std::cout << "getKeys: done: DBbusy=" << DB::inst().DBbusy <<"\n";
    return 0;
}
int DB::getKeys(const char* op, const char* filter, const char* keyc) {
    // std::cout << "getKeys: DBbusy=" << DB::inst().DBbusy <<"\n";
    while (DB::inst().DBbusy) {}
    std::string dest = "SELECT ";
    dest+=op;
    dest+="(keyi) FROM keys WHERE tsid LIKE '";
    dest+=filter;
    dest+="%";
    dest+=keyc;
    dest+="';";
    // std::cout << "SQL QUERY: " << dest;

    int rc;
    char *zErrMsg = 0;
    sqlite3 *db = DB::DBconn();
    DB::inst().DBbusy = true;
    rc = sqlite3_exec(db, const_cast<char*>(dest.c_str()), DB::Querycallback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        printf("getKeys: SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    while (DB::inst().DBbusy) {}
    DB::inst().DBbusy = false;
    return DB::inst().keysVal;
}