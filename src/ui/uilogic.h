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

#include <FL/Fl.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include "cui.hxx"
#include "ui/ui.h"