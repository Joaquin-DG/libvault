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
    
    Clay_TextElementConfig bookTextConfig = {
        .userData = nullptr,
        .textColor = black,
        .fontId = 0,
        .fontSize = 20,
        .letterSpacing = 3
    };

    
    Clay_TextElementConfig bookNameTextConfig = {
        .userData = nullptr,
        .textColor = black,
        .fontId = 0,
        .fontSize = 26,
        .letterSpacing = 3
    };

    // Read all database and store it in library

    library lib = ReadFromDatabase(db);
    std::vector<std::string> selected_book;

    std::string command_output = GetCommandOutput("ls /home/jdg/Pictures/ebook_cover");
    
    std::vector<std::string> images = GetArrayImages(command_output);
    
    bool hasPendingDelete = false;
    bool hasPendingInsert = false;

    std::string temp_name;
    std::string temp_author;
    std::string temp_comment;
    std::string temp_path;
    int temp_rate = 0;

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
            .backgroundColor = brown
        }) {
            // Child elements go here

            CLAY(CLAY_ID("Header"), {
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_GROW(),
                        .height = CLAY_SIZING_FIXED(60)
                    },
                },
                .backgroundColor = vanilla,
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
                            },
                        .padding = {8,8,16,8},
                        .childGap = 25,
                        .layoutDirection = CLAY_TOP_TO_BOTTOM
                    },
                    .backgroundColor = vanilla,
                    .cornerRadius = CLAY_CORNER_RADIUS(8)
                }) {
                    
                    char* addbook = "Add Book";
                    CLAY(CLAY_ID("Add Book Button"), {
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_GROW(),
                                .height = CLAY_SIZING_FIXED(40)
                            },
                            .padding = {45,0,10,0}
                        },
                        .backgroundColor = camel,
                        .cornerRadius = CLAY_CORNER_RADIUS(8)
                    }) {    // Insert logic
                        if(Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            hasPendingInsert = true;
                        }
                        Clay_String AddBookButton = {
                            .isStaticallyAllocated = false,
                            .length = (int32_t)strlen(addbook),
                            .chars = addbook
                        };
                        CLAY_TEXT(AddBookButton, bookTextConfig);
                    }
                    CLAY(CLAY_ID("Delete Book Button"), {
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_GROW(),
                                .height = CLAY_SIZING_FIXED(40)
                            },
                            .padding = {45,0,10,0}
                        },
                        .backgroundColor = camel,
                        .cornerRadius = CLAY_CORNER_RADIUS(8)
                    }) {
                        if(Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            hasPendingDelete = true;
                        }
                        Clay_String DeleteBookButton = {
                            .isStaticallyAllocated = false,
                            .length = (int32_t)16,
                            .chars = "Delete Book"
                        };
                        CLAY_TEXT(DeleteBookButton, bookTextConfig);
                    }

                } // Options Column

                CLAY(CLAY_ID("Main Panel"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_GROW(),
                            .height = CLAY_SIZING_GROW()
                        },
                        .padding = CLAY_PADDING_ALL(16),
                        .childGap = 25,
                        /*
                        .childAlignment = {
                            .x = CLAY_ALIGN_X_LEFT,
                            .y = CLAY_ALIGN_Y_TOP
                        },
                        */
                        .layoutDirection = CLAY_TOP_TO_BOTTOM
                    },
                    .backgroundColor = vanilla,
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
                                        },
                                        .backgroundColor = (find_selected_book(selected_book,lib[book_index].GetId())) ? camel : vanilla
                                    }) {
                                        if ( Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)){
                                            selected_book.push_back(lib[book_index].GetId());
                                        }
                                        if ( Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyUp(KEY_LEFT_CONTROL)){
                                            selected_book.clear();
                                            selected_book.push_back(lib[book_index].GetId());
                                        }

                                        CLAY(CLAY_IDI("Book_cover_", book_index), {
                                            .layout = {
                                                .sizing = layout_book
                                            },
                                            .backgroundColor = black
                                        }) {}
                                        CLAY(CLAY_IDI("Book_text_", book_index), {
                                            .layout = {
                                                .sizing = layout_expand,
                                                .padding = CLAY_PADDING_ALL(8),
                                                .childGap = 25,
                                                .childAlignment = {
                                                    .x = CLAY_ALIGN_X_LEFT,
                                                    .y = CLAY_ALIGN_Y_TOP
                                                },
                                                .layoutDirection = CLAY_TOP_TO_BOTTOM
                                            }
                                        }) {

                                            CLAY(CLAY_IDI("Book_text_name_", book_index)){

                                                const std::string& bookName = lib[book_index].GetName();
                                                const char* bookNameChars = bookName.empty()
                                                    ? "unknown"
                                                    : bookName.c_str();

                                                Clay_String bookNameText = {
                                                    .isStaticallyAllocated = false,
                                                    .length = (int32_t)(bookName.empty() ? 7 : bookName.size()),
                                                    .chars = bookNameChars
                                                };
                                                CLAY_TEXT(bookNameText, bookNameTextConfig);
                                            }
                                            
                                            CLAY(CLAY_IDI("Book_text_author_",book_index)){
                                                const std::string& bookAuthor = lib[book_index].GetAuthor();
                                                const char* bookAuthorChars = bookAuthor.empty()
                                                    ? "unknown"
                                                    : bookAuthor.c_str();

                                                Clay_String bookAuthorText = {
                                                    .isStaticallyAllocated = false,
                                                    .length = (int32_t)(bookAuthor.empty() ? 7 : bookAuthor.size()),
                                                    .chars = bookAuthorChars
                                                };
                                                CLAY_TEXT(bookAuthorText, bookTextConfig);
                                            }

                                            CLAY(CLAY_IDI("Book_text_comment_",book_index)){
                                                const std::string& bookComment = lib[book_index].GetComment();
                                                const char* bookCommentChars = bookComment.empty()
                                                    ? ""
                                                    : bookComment.c_str();

                                                Clay_String bookCommentText = {
                                                    .isStaticallyAllocated = false,
                                                    .length = (int32_t)(bookComment.empty() ? 7 : bookComment.size()),
                                                    .chars = bookCommentChars
                                                };
                                                CLAY_TEXT(bookCommentText, bookTextConfig);
                                            }

                                            CLAY(CLAY_IDI("Book_text_rate_",book_index)){
                                                const int bookRate = lib[book_index].GetRate();
                                                const std::string& bookRateLabel = std::to_string(bookRate) + " / 10";
                                                Clay_String bookRateText = {
                                                    .isStaticallyAllocated = false,
                                                    .length = (int32_t)bookRateLabel.size(),
                                                    .chars = bookRateLabel.c_str()
                                                };
                                                CLAY_TEXT(bookRateText, bookTextConfig);
                                            }
                                        } // Book_text
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

        
        if (hasPendingDelete) {
            
            for ( const auto &b : selected_book ){
                fsqlDelete(db, b);
            }            

            lib = ReadFromDatabase(db);
            hasPendingDelete = false;
            selected_book.clear();
        }
        if ( hasPendingInsert ){
            fsqlInsert(db);
            lib = ReadFromDatabase(db);
            hasPendingInsert = false; 
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands, fonts);
        EndDrawing();
    }

    CloseWindow();
    sqlite3_close(db);

    return 0;
}    