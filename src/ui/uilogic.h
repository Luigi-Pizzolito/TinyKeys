#pragma once
#include <string>
// namespace UILogic {
//     void init();

//     void colourKeyboard();
// };
#include <map>


#include <FL/Fl.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>

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
        std::map<std::string,std::string> kPmap;
        Fl_Double_Window* more_window;

        static std::string getKeyFilter(const char* format, int dayoff=0);
        static void setKeyFilter(const char* format, int dayoff=0);
        static void setKeyFilter(int w);
};


#include "cui.hxx"
#include "ui/ui.h"