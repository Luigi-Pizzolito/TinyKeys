#pragma once
#include <string>
// namespace UILogic {
//     void init();

//     void colourKeyboard();
// };

class UILogic {
    static UILogic* UILinst;
    private:
        UILogic();
        std::string keyFilter;
        bool stampWeek;
        bool stampSem;
    public:
        static UILogic& inst();
        
        static void colourKeyboard();

        static std::string getKeyFilter(const char* format, int dayoff=0);
        static void setKeyFilter(const char* format, int dayoff=0);
        static void setKeyFilter(int w);
};

#include "ui/ui.h"