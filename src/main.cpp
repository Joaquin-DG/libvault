/*  libvault
        main.cpp
*/

#define CLAY_IMPLEMENTATION

#include "common.hpp"


int main (int argc , char *argv[])
{ 
    Clay_Raylib_Initialize(WIDTH, HEIGHT, "libvault",FLAG_WINDOW_RESIZABLE);
   
    uint64_t RequiredMemory = Clay_MinMemorySize();
    
    Clay_Arena claymemory = Clay_CreateArenaWithCapacityAndMemory (RequiredMemory , malloc(RequiredMemory));

    Clay_Initialize(claymemory , {WIDTH , HEIGHT}, (Clay_ErrorHandler){Error_handler});
    
    Font fonts[1];
    fonts[0] = LoadFontEx("../assets/font_test.ttf", 48,0,400);

    // Leer la base de datos, por ahora solo va a leer una, despues habra menu interactivo

    sqlite3 *db;

    if(sqlite3_open("../library.db",&db) != SQLITE_OK){
        return 1;
    }

    std::string id = generarId();
    std::string name = "Rey Arturo";

    const char* sqlInsert = "INSERT into book (id, name) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare16_v2(db,sqlInsert, -1 ,&stmt ,nullptr) == SQLITE_OK){
        sqlite3_bind_text(stmt , 1 , id.c_str(), -1 , SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt , 2 , name.c_str(), -1 , SQLITE_TRANSIENT);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    library l;


    SetTargetFPS(60);
    SetWindowMinSize(420,320);

    while(!WindowShouldClose()){

        // Update

        Clay_SetLayoutDimensions((Clay_Dimensions){.width = GetScreenWidth() , .height = GetScreenHeight()});

        Clay_BeginLayout();

            CLAY(

                CLAY_ID("Background Dislpay"), (Clay_ElementDeclaration){
                    .layout = {
                        .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                        
                        .padding = CLAY_PADDING_ALL(8),
                        .childGap = 25,
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    },
                    .backgroundColor = vanilla
                }
            ){
                //Child elements go here

                CLAY(CLAY_ID("Header")
                    ,(Clay_ElementDeclaration){
                        .layout = { 
                            .sizing = {
                                .width = CLAY_SIZING_GROW(),
                                .height = CLAY_SIZING_FIXED(60)
                            },

                        },
                        .backgroundColor = white,
                        .cornerRadius = CLAY_CORNER_RADIUS(8)
                    }

                ){
                 
                    CLAY(CLAY_ID("Library_header")


                    ){}


                }

                CLAY(CLAY_ID("Lower Content"),
                    (Clay_ElementDeclaration){
                        .layout = {
                            .sizing = { layout_expand },
                            .childGap = 25
                        }
                    }
                ){

                    
                    CLAY(CLAY_ID("Options Column")
                        ,(Clay_ElementDeclaration){
                            .layout = {
                                .sizing = {
                                    .width = CLAY_SIZING_FIXED(220),
                                    .height = CLAY_SIZING_GROW()
                                }
                            },
                            .backgroundColor = white,
                            .cornerRadius = CLAY_CORNER_RADIUS(8)
                        }

                    ){} // Options Column


                    CLAY(CLAY_ID("Main Panel")
                        ,(Clay_ElementDeclaration){
                            .layout = {
                                .sizing = {
                                    .width = CLAY_SIZING_GROW(),
                                    .height = CLAY_SIZING_GROW()
                                },
                                .padding = CLAY_PADDING_ALL(16),
                                .childGap = 25,
                                .childAlignment = {
                                    .x = CLAY_ALIGN_X_LEFT,
                                    .y = CLAY_ALIGN_Y_TOP
                                },
                                .layoutDirection = CLAY_TOP_TO_BOTTOM
                            },
                            .backgroundColor = white,
                            .cornerRadius = CLAY_CORNER_RADIUS(8)
                        }

                    ){
                        //Childs of Main Panel

                        //Here comes the for loop with the library vector
                        CLAY(CLAY_ID("Book_row_test"),
                            (Clay_ElementDeclaration){
                                .layout = {
                                    .sizing = layout_book_row,
                                    .childGap = 25,
                                    .layoutDirection = CLAY_LEFT_TO_RIGHT
                                },
                            }

                        ){
                            CLAY(CLAY_ID("book_cover"),
                                (Clay_ElementDeclaration){
                                    .layout = {
                                        .sizing = layout_book
                                    },
                                    .backgroundColor = black
                                } 
                            ){}

                            CLAY(CLAY_ID("Book_text"),
                                (Clay_ElementDeclaration){
                                    .layout = {
                                        .sizing = layout_expand
                                    },
                                    .backgroundColor = black
                                }
                            ){}
                        } // book_row
                    
                    } //Main Panel

                }   // Lower content
                
                
            } // Background Display 
        
        
        Clay_RenderCommandArray renderCommands = Clay_EndLayout(GetFrameTime());



        // Drawing
        BeginDrawing();

        ClearBackground(BLACK);

        Clay_Raylib_Render(renderCommands, fonts);

        EndDrawing();

    }

    UnloadFont(fonts[0]);

    CloseWindow();

    sqlite3_close(db);

    return 0;
}
