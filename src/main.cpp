#include <thread>
#include <sqlite3.h>

#include "logger/logger.h"
#include "db/db.h"
#include "ui/uilogic.h"

#include <iostream>

int main(int argc, const char *argv[]) {
    std::cout << "starting threads\n";
    std::thread DB_PushWorker(DB::pushWorker);
    std::thread KeyLogger_Thread(KeyLogger::logWorker);
    UILogic::setKeyFilter("%Y%m%d");
    

    Fl::run();

    // KeyLogger_Thread.join();
    // DB_PushWorker.join();
    
    DB::close();
    // sqlite3_close(db);
    // KeyLogger_Thread.detach();
    // DB_PushWorker.detach();
    return 0;
}