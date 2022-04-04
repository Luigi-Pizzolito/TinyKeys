#include <thread>
#include <sqlite3.h>

#include "logger/logger.h"
#include "ui/uilogic.h"
#include "db/db.h"

#include <iostream>

int main(int argc, const char *argv[]) {
    std::cout << "starting threads\n";
    std::thread DB_PushWorker(DB::pushWorker);
    std::thread KeyLogger_Thread(KeyLogger::logWorker);

    UILogic::init();

    Fl::run();

    // KeyLogger_Thread.join();
    // DB_PushWorker.join();
    
    DB::close();
    // sqlite3_close(db);
    // KeyLogger_Thread.detach();
    return 0;
}