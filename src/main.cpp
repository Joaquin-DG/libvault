/*  libvault
        main.cpp
*/

#define CLAY_IMPLEMENTATION

#include "common.hpp"
#include "clay_renderer_raylib.c"

int main (int argc , char *argv[])
{ 
    Clay_Raylib_Initialize(WIDTH, HEIGHT, "libvault",FLAG_WINDOW_RESIZABLE);
   
    uint64_t RequiredMemory = Clay_MinMemorySize();
    
    Clay_Arena claymemory = Clay_CreateArenaWithCapacityAndMemory (RequiredMemory , malloc(RequiredMemory));

    Clay_Initialize(claymemory , {WIDTH , HEIGHT}, (Clay_ErrorHandler){Error_handler});
    
    Font fonts[1];
    fonts[0] = GetFontDefault();

    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    // Leer la base de datos, por ahora solo va a leer una, despues habra menu interactivo

    sqlite3 *db;

    if (sqlite3_open("/opt/libvault/db/library.db", &db) != SQLITE_OK) {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return 1;
    }
    
    // Read all database and store it in library

    library lib = ReadFromDatabase(db);

    Clay_TextElementConfig bookTextConfig = {
        .userData = nullptr,
        .textColor = black,
        .fontId = 0,
        .fontSize = 24,
        .letterSpacing = 0
    };

    SetTargetFPS(60);
    SetWindowMinSize(420,320);
    
    while (!WindowShouldClose()) {
        // Update
        Clay_SetLayoutDimensions((Clay_Dimensions){.width = GetScreenWidth(), .height = GetScreenHeight()});

        Vector2 mouse_position_raylib = GetMousePosition();
        Clay_Vector2 mouse_position = {(float)mouse_position_raylib.x, (float)mouse_position_raylib.y};
        Clay_SetPointerState(mouse_position, false);

        Clay_BeginLayout();

        CLAY(CLAY_ID("Background Dislpay"), {
            .layout = {
                .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                .padding = CLAY_PADDING_ALL(8),
                .childGap = 25,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            .backgroundColor = vanilla
        }) {
            // Child elements go here

            CLAY(CLAY_ID("Header"), {
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_GROW(),
                        .height = CLAY_SIZING_FIXED(60)
                    },
                },
                .backgroundColor = white,
                .cornerRadius = CLAY_CORNER_RADIUS(8)
            }) {}

            CLAY(CLAY_ID("Lower Content"), {
                .layout = {
                    .sizing = {layout_expand},
                    .childGap = 25
                }
            }) {
                CLAY(CLAY_ID("Options Column"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIXED(220),
                            .height = CLAY_SIZING_GROW()
                        }
                    },
                    .backgroundColor = white,
                    .cornerRadius = CLAY_CORNER_RADIUS(8)
                }) {} // Options Column

                CLAY(CLAY_ID("Main Panel"), {
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
                }) {
                    // Childs of Main Panel

                    // Here comes the for loop with the library vector
                    
                    int size = lib.size();
                    int row_count = (size + 2) / 3;

                    for (int row = 0; row < row_count; ++row) {
                        CLAY(CLAY_IDI("Book_row_", row), {
                            .layout = {
                                .sizing = layout_book_row,
                                .childGap = 25,
                                .layoutDirection = CLAY_LEFT_TO_RIGHT
                            }
                        }) {
                            for (int column = 0; column < 3; ++column) {
                                int book_index = row * 3 + column;

                                if (book_index < size) {
                                    CLAY(CLAY_IDI("Book_", book_index), {
                                        .layout = {
                                            .sizing = layout_expand,
                                            .childGap = 25,
                                            .layoutDirection = CLAY_LEFT_TO_RIGHT
                                        }
                                    }) {
                                        CLAY(CLAY_IDI("Book_cover_", book_index), {
                                            .layout = {
                                                .sizing = layout_book
                                            },
                                            .backgroundColor = black
                                        }) {}
                                        CLAY(CLAY_IDI("Book_text_", book_index), {
                                            .layout = {
                                                .sizing = layout_expand
                                                ,
                                                .padding = CLAY_PADDING_ALL(8),
                                                .childAlignment = {
                                                    .x = CLAY_ALIGN_X_LEFT,
                                                    .y = CLAY_ALIGN_Y_TOP
                                                }
                                            }
                                        }) {
                                            std::string bookId = lib[book_index].GetId();
                                            Clay_String bookIdText = {
                                                .isStaticallyAllocated = false,
                                                .length = (int32_t)bookId.size(),
                                                .chars = bookId.c_str()
                                            };
                                            CLAY_TEXT(bookIdText, bookTextConfig);
                                        }
                                    }
                                } else {
                                    CLAY(CLAY_IDI("Book_empty_", book_index), {
                                        .layout = {
                                            .sizing = layout_expand
                                        }
                                    }) {}
                                }
                            }
                        } // book_row
                    } // book rows ( for loop )
                } // Main Panel
            } // Lower content
        } // Background Display

        Clay_RenderCommandArray renderCommands = Clay_EndLayout(GetFrameTime());


        //Update non-clay starts here


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