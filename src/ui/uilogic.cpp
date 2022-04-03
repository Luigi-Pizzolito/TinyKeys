#include "uilogic.h"
#include <iostream>
#include "db/db.h"
#include <string>

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
        DB::init();
        std::string keyFilter = "2022040401";
        int keyMax = DB::getKeys("MAX", keyFilter.c_str(), "");
        keyMax = (keyMax>0)?keyMax:1;
        std::cout << "Key MAX is " << keyMax << "\n";
        for (size_t i=0; i<keyboard->children(); i++) {
            Fl_Widget* key = keyboard->child(i);

            int keyC = DB::getKeys("SUM", keyFilter.c_str(), keyOrder[i]);
            float keyP = (float)keyC / (float)keyMax;
            // std::cout << keyOrder[i] << "=" << keyC << "\n";

            key->copy_tooltip(std::to_string(keyC).c_str());
            key->color(fl_rgb_color(162, 60, 20));
            key->redraw();
        }
    }

}