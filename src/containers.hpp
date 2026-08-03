#include <clay.h>

Clay_LayoutConfig config_layout {
    
};

Clay_Sizing layout_expand = {
    .width = CLAY_SIZING_GROW(),
    .height = CLAY_SIZING_GROW()
};

Clay_Sizing layout_book = {
    .width = CLAY_SIZING_FIXED(130),
    .height = CLAY_SIZING_FIXED(200)
};

Clay_Sizing layout_book_row = {
    .width = CLAY_SIZING_GROW(),
    .height = CLAY_SIZING_FIXED(200)
};

void Error_handler(Clay_ErrorData error_data){

        std::cout << error_data.errorText.chars << std::endl;

}