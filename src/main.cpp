/*  libvault
        main.cpp
*/

#define CLAY_IMPLEMENTATION

#include "common.hpp"


void Error_handler(Clay_ErrorData error_data){

        std::cout << error_data.errorText.chars << std::endl;

}

int main (void)
{ 
    Clay_Raylib_Initialize(WIDTH, HEIGHT, "libvault",FLAG_WINDOW_RESIZABLE);
   
    uint64_t RequiredMemory = Clay_MinMemorySize();
    
    Clay_Arena claymemory = Clay_CreateArenaWithCapacityAndMemory (RequiredMemory , malloc(RequiredMemory));

    Clay_Initialize(claymemory , {WIDTH , HEIGHT}, (Clay_ErrorHandler){Error_handler});
    
    Font fonts[1];
    fonts[0] = LoadFontEx("../assets/font_test.ttf", 48,0,400);

    SetTargetFPS(60);

    while(!WindowShouldClose()){

        // Update

        Clay_SetLayoutDimensions((Clay_Dimensions){.width = GetScreenWidth() , .height = GetScreenHeight()});

        Clay_BeginLayout();

            CLAY(
                CLAY_ID("Background Dislpay"), (Clay_ElementDeclaration){
                    .layout = {
                        .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                        
                        .padding = CLAY_PADDING_ALL(8),
                        .childGap = 16,
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    },
                    .backgroundColor = (Clay_Color){43,41,51,255},
                    
                }
            ){
                //Child elements go here
            
                CLAY(CLAY_ID("Header")
                    ,(Clay_ElementDeclaration){
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_FIXED(60),
                                .height = CLAY_SIZING_FIXED(60)
                            },

                            

                        },
                        .backgroundColor = purple,
                        .cornerRadius = CLAY_CORNER_RADIUS(8)
                    }

                ){}
                
            }
        
        
        Clay_RenderCommandArray renderCommands = Clay_EndLayout(GetFrameTime());



        // Drawing
        BeginDrawing();

        ClearBackground(BLACK);

        Clay_Raylib_Render(renderCommands, fonts);

        EndDrawing();





    }

    CloseWindow();

    return 0;
}
