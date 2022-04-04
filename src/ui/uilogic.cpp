// #define IPC_MALLOC
// #define IPC_FREE

#include <stdlib.h>
// #define IPC_IMPLEMENTATION
// #include "db/ipc.h"
#include "uilogic.h"
#include <iostream>
#include "db/db.h"
#include "Gradient.h"
#include <ctime>

#define SEM 1
#define WEEK 0
#define N_RST -1

UILogic* UILogic::UILinst = &inst();
UILogic& UILogic::inst() {
    if (UILinst == nullptr) {
        UILinst = new UILogic;
    }
    return *UILinst;
}

UILogic::UILogic() {
    Fl_Double_Window* window = make_main_window();

    nav_today->setonly();
    keyFilter = "%Y%m%d";
    stampWeek = false;
    stampSem = false;

    gs_today->value("");    
    gs_yesterday->value("");
    gs_week->value("");
    gs_alltime->value("");

    window->end();
    window->show();

}


std::string UILogic::getKeyFilter(const char* format, int dayoff) {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    rawtime+=86400*dayoff;
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),format,timeinfo);
    std::string str(buffer);

    return str;
}

void UILogic::setKeyFilter(const char* format, int dayoff) {
    setKeyFilter(N_RST);
    UILogic::inst().keyFilter=UILogic::getKeyFilter(format, dayoff);
}

void UILogic::setKeyFilter(int w) {
    if (w==0) {
        UILogic::inst().stampWeek = true;
    } else if (w==1) {
        UILogic::inst().stampSem = true;
    } else {
        UILogic::inst().stampWeek = false;
        UILogic::inst().stampSem = false;
    }
}


void UILogic::colourKeyboard() {

    int keyMax;
    if (!UILogic::inst().stampWeek && !UILogic::inst().stampSem) {
        keyMax = DB::getKeys("MAX", UILogic::inst().keyFilter.c_str(), "");
    } else if (UILogic::inst().stampWeek) {
        keyMax = 0;
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m%d").c_str()    , "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m%d", -1).c_str(), "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m%d", -2).c_str(), "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m%d", -3).c_str(), "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m%d", -4).c_str(), "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m%d", -5).c_str(), "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m%d", -6).c_str(), "");
    } else if (UILogic::inst().stampSem) {
        keyMax = 0;
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m").c_str()    , "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m", -1*30).c_str(), "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m", -2*30).c_str(), "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m", -3*30).c_str(), "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m", -4*30).c_str(), "");
        keyMax+=DB::getKeys("MAX", UILogic::getKeyFilter("%Y%m", -5*30).c_str(), "");
    }
    keyMax = (keyMax>0)?keyMax:1;

    Gradient::Gradient<Gradient::GradientColor> heat;
    heat.addColorStop(0.0, Gradient::GradientColor(182.0f, 179.0f, 221.0f, 1.0f));        
    heat.addColorStop(0.2, Gradient::GradientColor(160.0f, 166.0f, 252.0f, 1.0f));
    heat.addColorStop(0.4, Gradient::GradientColor(159.0f, 201.0f, 251.0f, 1.0f));
    heat.addColorStop(0.6, Gradient::GradientColor(158.0f, 245.0f, 194.0f, 1.0f));
    heat.addColorStop(0.8, Gradient::GradientColor(237.0f, 226.0f, 170.0f, 1.0f));
    heat.addColorStop(1.0, Gradient::GradientColor(238.0f, 168.0f, 172.0f, 1.0f));

    // std::cout << "Key MAX is " << keyMax << "\n";
    const char* keyOrder[73] = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "-]", "\\", "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "[tab]", "[del]", "[esc]", "[f1]", "[f2]", "[f3]", "[f4]", "[f5]", "[f6]", "[f7]", "[f8]", "[f9]", "[f10]", "[f11]", "[f12]", "[caps]", "[return]", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "\"", "[left-shift]", "[right-shift]", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "[left-ctrl]", "[left-option]", "[left-cmd]", "[right-ctrl]", "[right-cmd]", "[fn]", " "};
    for (size_t i=0; i<keyboard->children(); i++) {
        Fl_Widget* key = keyboard->child(i);


        int keyC;
        if (!UILogic::inst().stampWeek && !UILogic::inst().stampSem) {
            keyC = DB::getKeys("SUM", UILogic::inst().keyFilter.c_str(), keyOrder[i]);
        } else if (UILogic::inst().stampWeek) {
            keyC = 0;
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d").c_str()    , keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -1).c_str(), keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -2).c_str(), keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -3).c_str(), keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -4).c_str(), keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -5).c_str(), keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -6).c_str(), keyOrder[i]);
        } else if (UILogic::inst().stampSem) {
            keyC = 0;
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m").c_str()       , keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m", -1*30).c_str(), keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m", -2*30).c_str(), keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m", -3*30).c_str(), keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m", -4*30).c_str(), keyOrder[i]);
            keyC+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m", -5*30).c_str(), keyOrder[i]);
        }




        // keyC = (keyC)?keyC:0;
        float keyP = (float)keyC / (float)keyMax;
        // std::cout << keyOrder[i] << "=" << keyC << "\n";

        key->copy_tooltip(std::to_string(keyC).c_str());
        float keyclrP = (-(keyP-1.0f)*(keyP-1.0f))+1.0f; // nicer colours by bringing out low values using y=-(x-1)^2+1 curve
        Gradient::GradientColor keyclr = heat.getColorAt(keyclrP);
        key->color(fl_rgb_color(keyclr.r,keyclr.g,keyclr.b));
        key->redraw();
    }


    gs_today->value(std::to_string(DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d").c_str(), "")).c_str());    
    gs_yesterday->value(std::to_string(DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -1).c_str(), "")).c_str());
    int k_week = 0;
    k_week+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d").c_str()    , "");
    k_week+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -1).c_str(), "");
    k_week+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -2).c_str(), "");
    k_week+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -3).c_str(), "");
    k_week+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -4).c_str(), "");
    k_week+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -5).c_str(), "");
    k_week+=DB::getKeys("SUM", UILogic::getKeyFilter("%Y%m%d", -6).c_str(), "");
    gs_week->value(std::to_string(k_week).c_str());
    gs_alltime->value(std::to_string(DB::getKeys("SUM", "", "")).c_str());


}

// namespace UILogic {
//     void init() {
        
//     }



    

//     void colourKeyboard() {
//         // DB::init();
        

// }