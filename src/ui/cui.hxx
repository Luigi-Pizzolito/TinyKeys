#include <FL/Fl.H>
#include <FL/Fl_Table.H>
#define MAX_ROWS 73
#define MAX_COLS 2

#include <iostream>

// Derive a class from Fl_Table
class MyTable : public Fl_Table
{

    const char *headers[2] = {"Key", "Amount"};
    int cw[2] = {80, 120};

    // Draw the row/col headings
    //    Make this a dark thin upbox with the text inside.
    //
    void DrawHeader(const char *s, int X, int Y, int W, int H)
    {
        fl_push_clip(X, Y, W, H);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
        fl_color(FL_BLACK);
        fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
        fl_pop_clip();
    }
    // Draw the cell data
    //    Dark gray text on white background with subtle border
    //
    void DrawData(const char *s, int X, int Y, int W, int H)
    {
        fl_push_clip(X, Y, W, H);
        // Draw cell bg
        fl_color(FL_WHITE);
        fl_rectf(X, Y, W, H);
        // Draw cell data
        fl_color(FL_GRAY0);
        fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
        // Draw box border
        fl_color(color());
        fl_rect(X, Y, W, H);
        fl_pop_clip();
    }
    // Handle drawing table's cells
    //     Fl_Table calls this function to draw each visible cell in the table.
    //     It's up to us to use FLTK's drawing functions to draw the cells the way we want.
    //
    void draw_cell(TableContext context, int ROW = 0, int COL = 0, int X = 0, int Y = 0, int W = 0, int H = 0)
    {
        const char *keyOrder[73] = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "-]", "\\", "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "[tab]", "[del]", "[esc]", "[f1]", "[f2]", "[f3]", "[f4]", "[f5]", "[f6]", "[f7]", "[f8]", "[f9]", "[f10]", "[f11]", "[f12]", "[caps]", "[return]", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "\"", "[left-shift]", "[right-shift]", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "[left-ctrl]", "[left-option]", "[left-cmd]", "[right-ctrl]", "[right-cmd]", "[fn]", " "};
        const char *keyOrderDisp[73] = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\\", "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "[tab]", "[del]", "[esc]", "[f1]", "[f2]", "[f3]", "[f4]", "[f5]", "[f6]", "[f7]", "[f8]", "[f9]", "[f10]", "[f11]", "[f12]", "[caps]", "[return]", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "\"", "[left-shift]", "[right-shift]", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "[left-ctrl]", "[left-option]", "[left-cmd]", "[right-ctrl]", "[right-cmd]", "[fn]", "[space]"};
        static char s[40];
        const char *cc;
        if (ROW < UILogic::inst().kPmap.size()) {
            cc = (COL == 0) ? keyOrderDisp[ROW] : UILogic::inst().kPmap[keyOrder[ROW]].c_str();
        } else {
            cc = "";
        }

        switch (context)
        {
        case CONTEXT_STARTPAGE:        // before page is drawn..
            fl_font(FL_HELVETICA, 14); // set the font for our drawing operations
            return;
        case CONTEXT_COL_HEADER: // Draw column headers
            // sprintf(s,"%c",'A'+COL);                // "A", "B", "C", etc.
            sprintf(s, "%s", headers[COL]);
            DrawHeader(s, X, Y, W, H);
            return;
        case CONTEXT_ROW_HEADER:      // Draw row headers
            sprintf(s, "%03d:", ROW); // "001:", "002:", etc
            DrawHeader(s, X, Y, W, H);
            return;
        case CONTEXT_CELL: // Draw data in cells
            sprintf(s, "%s", cc);
            DrawData(s, X, Y, W, H);
            return;
        default:
            return;
        }
    }

public:
    // Constructor
    //     Make our data array, and initialize the table options.
    //
    
    std::string data[MAX_ROWS][MAX_COLS]; // data array for cells

    void updateData()
    {

        //   std::cout << "kMAP sieze is: " << UILogic::inst().kPmap.size() << "\n";
        //   for (int i = 0; i < UILogic::inst().kPmap.size(); i++) {
        //       std::cout << std::string(keyOrderDisp[i]);
        //       std::cout << UILogic::inst().kPmap[keyOrder[i]];
        //   }
        //   for ( int r=0; r<MAX_ROWS; r++ ) {
        //     data[r][0] = std::string("key");
        //     data[r][1] = std::string("num");
        //   }
    }

    MyTable(int X, int Y, int W, int H, const char *L = 0) : Fl_Table(X, Y, W, H, L)
    {
        // Fill data array
        for (int r = 0; r < MAX_ROWS; r++)
        {
            data[r][0] = std::string("key");
            data[r][1] = std::string("num");
        }

        // size_t r=0;
        // for ( auto const& it : UILogic::inst().kPmap) {
        //     data[r][0] = it.first;
        //     data[r][1] = it.second;
        //     r++;
        // }
        // Rows
        rows(MAX_ROWS);     // how many rows
        row_header(0);      // disenable row headers (along left)
        row_height_all(15); // default height of rows
        row_resize(0);      // disable row resizing
        // Cols
        cols(MAX_COLS); // how many columns
        col_header(1);  // enable column headers (along top)
        // col_width_all(80);          // default width of columns
        col_width(0, cw[0]);
        col_width(1, cw[1]);
        col_resize(0); // disenable column resizing
        end();         // end the Fl_Table group
    }
    ~MyTable() {}
};