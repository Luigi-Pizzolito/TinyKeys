// #define IPC_MALLOC
// #define IPC_FREE

#include <stdlib.h>
#define IPC_IMPLEMENTATION
#include "db/ipc.h"
#include "uilogic.h"
#include <iostream>
#include "db/db.h"
#include <string>
#include "Gradient.h"

namespace UILogic {
    void init() {
        Fl_Double_Window* window = make_main_window();

        nav_today->setonly();
    
        gs_today->static_value("1,000");    
        gs_yesterday->static_value("2,000");
        gs_week->static_value("100,000");
        gs_alltime->static_value("10,000,000");

        window->end();
        window->show();
    }



    const char* keyOrder[73] = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\\", "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "[tab]", "[del]", "[esc]", "[f1]", "[f2]", "[f3]", "[f4]", "[f5]", "[f6]", "[f7]", "[f8]", "[f9]", "[f10]", "[f11]", "[f12]", "[caps]", "[return]", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "\"", "[left-shift]", "[right-shift]", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "[left-ctrl]", "[left-option]", "[left-cmd]", "[right-ctrl]", "[right-cmd]", "[fn]", " "};

    void colourKeyboard() {
        // DB::init();
        std::string keyFilter = "2022040415";


        int keyMax = DB::getKeys("MAX", keyFilter.c_str(), "");
        keyMax = (keyMax>0)?keyMax:1;

        Gradient::Gradient<Gradient::GradientColor> heat;
        heat.addColorStop(0.0, Gradient::GradientColor(182.0f, 179.0f, 221.0f, 1.0f));        
        heat.addColorStop(0.2, Gradient::GradientColor(160.0f, 166.0f, 252.0f, 1.0f));
        heat.addColorStop(0.4, Gradient::GradientColor(159.0f, 201.0f, 251.0f, 1.0f));
        heat.addColorStop(0.6, Gradient::GradientColor(158.0f, 245.0f, 194.0f, 1.0f));
        heat.addColorStop(0.8, Gradient::GradientColor(237.0f, 226.0f, 170.0f, 1.0f));
        heat.addColorStop(1.0, Gradient::GradientColor(238.0f, 168.0f, 172.0f, 1.0f));

        // std::cout << "Key MAX is " << keyMax << "\n";
        for (size_t i=0; i<keyboard->children(); i++) {
            Fl_Widget* key = keyboard->child(i);

            int keyC = DB::getKeys("SUM", keyFilter.c_str(), keyOrder[i]);
            float keyP = (float)keyC / (float)keyMax;
            // std::cout << keyOrder[i] << "=" << keyC << "\n";

            key->copy_tooltip(std::to_string(keyC).c_str());
            float keyclrP = (-(keyP-1.0f)*(keyP-1.0f))+1.0f; // nicer colours by bringing out low values using y=-(x-1)^2+1 curve
            Gradient::GradientColor keyclr = heat.getColorAt(keyclrP);
            key->color(fl_rgb_color(keyclr.r,keyclr.g,keyclr.b));
            key->redraw();
        }
    }

}