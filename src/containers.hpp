#include <clay.h>
#include <iostream>

/*

    ALL PAGES:
        MAIN PAGE:
            Header, Left Column and Main Panel
        


*/

inline Clay_Sizing layout_expand = {
    .width = CLAY_SIZING_GROW(),
    .height = CLAY_SIZING_GROW()
};

inline Clay_Sizing layout_book = {
    .width = CLAY_SIZING_FIXED(130),
    .height = CLAY_SIZING_FIXED(200)
};

inline Clay_Sizing layout_book_row = {
    .width = CLAY_SIZING_GROW(),
    .height = CLAY_SIZING_FIXED(200)
};

inline Clay_TextElementConfig bookTextConfig = {
    .userData = nullptr,
    .textColor = {0, 0, 0, 255},
    .fontId = 0,
    .fontSize = 20,
    .letterSpacing = 3
};

inline Clay_TextElementConfig bookNameTextConfig = {
    .userData = nullptr,
    .textColor = {0, 0, 0, 255},
    .fontId = 0,
    .fontSize = 26,
    .letterSpacing = 3
};

void Error_handler(Clay_ErrorData error_data){

        std::cout << error_data.errorText.chars << std::endl;

}