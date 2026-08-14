/*  libvault
        main.cpp

        READ TODO
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
    std::vector<std::string> selected_book;
    std::string obj_book_id;
    int open_image_menu_index = -1;
    int selected_image_for_change = 0;

    std::string image_dir = "/home/jdg/Pictures/ebook_cover";
    std::string command_output = GetCommandOutput((std::string("ls -- ") + "\"" + image_dir + "\"").c_str());

    std::vector<std::string> path_images = GetArrayImages(command_output);

    std::vector<Image> images;
    std::vector<Texture2D> coverTextures;

    for (const auto &i : path_images) {
        if (i.empty()) continue;

        std::string full_path = image_dir + "/" + i;
        Image img = LoadImage(full_path.c_str());

        if (img.data == nullptr || img.width <= 0 || img.height <= 0) {
            UnloadImage(img);
            continue;
        }

        images.push_back(img);
    }

    for (const auto &img : images) {
        if (img.data == nullptr || img.width <= 0 || img.height <= 0) continue;

        Texture2D tex = LoadTextureFromImage(img);
        if (tex.id != 0) {
            coverTextures.push_back(tex);
        }
    }

    bool allow_img_input = false;

    bool hasPendingDelete = false;
    bool hasPendingInsert = false;
    bool hasPendingUpdateImg = false;

    std::string img_input_path;

    SetTargetFPS(60);
    SetWindowMinSize(420,320);
    
    while (!WindowShouldClose()) {
        // Update
        Clay_SetLayoutDimensions((Clay_Dimensions){.width = GetScreenWidth(), .height = GetScreenHeight()});

        Vector2 mouse_position_raylib = GetMousePosition();
        Clay_Vector2 mouse_position = {(float)mouse_position_raylib.x, (float)mouse_position_raylib.y};
        Clay_SetPointerState(mouse_position, IsMouseButtonDown(MOUSE_BUTTON_LEFT));

        float mouseWheelY = GetMouseWheelMove();
        Clay_UpdateScrollContainers(true, {0.0f, mouseWheelY * 10.0f}, GetFrameTime());

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
                    .sizing = layout_expand,
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
                    CLAY(CLAY_ID("Add Book Button"), {
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_GROW(),
                                .height = CLAY_SIZING_FIXED(40)
                            },
                            .padding = CLAY_PADDING_ALL(0),
                            .childAlignment = {
                                .x = CLAY_ALIGN_X_CENTER,
                                .y = CLAY_ALIGN_Y_CENTER
                            }
                        },
                        .backgroundColor = camel,
                        .cornerRadius = CLAY_CORNER_RADIUS(8)
                    }) {
                        if(Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            hasPendingInsert = true;
                        }
                        Clay_String AddBookButton = {
                            .isStaticallyAllocated = false,
                            .length = (int32_t)10,
                            .chars = "Add Book"
                        };
                        CLAY_TEXT(AddBookButton, medium);
                    }
                    CLAY(CLAY_ID("Delete Book Button"), {
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_GROW(),
                                .height = CLAY_SIZING_FIXED(40)
                            },
                            .padding = CLAY_PADDING_ALL(0),
                            .childAlignment = {
                                .x = CLAY_ALIGN_X_CENTER,
                                .y = CLAY_ALIGN_Y_CENTER
                            }
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
                        CLAY_TEXT(DeleteBookButton, medium);
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
                        .layoutDirection = CLAY_TOP_TO_BOTTOM
                    },
                    .backgroundColor = vanilla,
                    .cornerRadius = CLAY_CORNER_RADIUS(8),
                    .clip = {
                        .vertical = true,
                        .childOffset = Clay_GetScrollOffset()
                    }
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
                                        }) {
                                            Texture2D* bookTexture = nullptr;
                                            int currentCoverIndex = lib[book_index].GetImg();
                                            if (currentCoverIndex >= 0 && currentCoverIndex < coverTextures.size()) {
                                                bookTexture = &coverTextures[currentCoverIndex];
                                            }

                                            CLAY(CLAY_IDI("Book_image_",book_index),{
                                                .layout = {
                                                    .sizing = layout_expand
                                                },
                                                .image = bookTexture
                                            }){
                                                CLAY(CLAY_IDI("Book_selector_",book_index),{
                                                    .layout = {
                                                        .sizing = {
                                                            .width = 20,
                                                            .height = 20
                                                        }
                                                    },
                                                    .backgroundColor = vanilla
                                                }){
                                                    if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                                                        open_image_menu_index = (open_image_menu_index == book_index) ? -1 : book_index;
                                                        obj_book_id = lib[book_index].GetId();
                                                    }

                                                    if (open_image_menu_index == book_index) {
                                                        CLAY(CLAY_IDI("Book_selection_menu_", book_index), {
                                                            .layout = {
                                                                .sizing = layout_book,
                                                                .padding = CLAY_PADDING_ALL(8),
                                                                .childGap = 10,
                                                                .layoutDirection = CLAY_TOP_TO_BOTTOM
                                                            },
                                                            .floating = {
                                                                .offset = {0,0},
                                                                .expand = {32, 100},
                                                                .zIndex = 0,
                                                                .attachPoints = {
                                                                    .element = CLAY_ATTACH_POINT_LEFT_TOP,
                                                                    .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM
                                                                },
                                                                .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE
                                                            },
                                                            .clip = {
                                                                .vertical = true,
                                                                .childOffset = Clay_GetScrollOffset()
                                                            }
                                                        }) {
                                                            CLAY_TEXT(CLAY_STRING("Scrool & Change with right click"),small);
                                                            CLAY_TEXT(CLAY_STRING("Or input global path to img"),small);
                                                            CLAY(CLAY_ID("input_img"),{
                                                                .layout = {
                                                                    .sizing = {
                                                                        .width = allow_img_input ? CLAY_SIZING_FIXED(500) : CLAY_SIZING_FIXED(30),
                                                                        .height = CLAY_SIZING_FIXED(30)
                                                                    },
                                                                    .childAlignment = {
                                                                        .x = CLAY_ALIGN_X_LEFT,
                                                                        .y = CLAY_ALIGN_Y_CENTER
                                                                    }
                                                                },
                                                                .backgroundColor = allow_img_input ? white : black,
                                                                .cornerRadius = CLAY_CORNER_RADIUS(8),
                                                                .floating = {
                                                                    .offset = { 0,0 },
                                                                    .expand = { 0,0 },
                                                                    .zIndex = 200,
                                                                    .attachPoints = {
                                                                        .element = CLAY_ATTACH_POINT_LEFT_BOTTOM,
                                                                        .parent = CLAY_ATTACH_POINT_LEFT_TOP
                                                                    },
                                                                    .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE,
                                                                    .attachTo = CLAY_ATTACH_TO_PARENT,
                                                                }
                                                            }){
                                                                if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                                                                    allow_img_input = !allow_img_input;
                                                                }

                                                                if (allow_img_input) {
                                                                    Clay_String img_path = {
                                                                        .isStaticallyAllocated = false,
                                                                        .length = (int32_t)img_input_path.size(),
                                                                        .chars = img_input_path.c_str()
                                                                    };
                                                                    CLAY_TEXT(img_path, small);
                                                                }
                                                            }

                                                            for (int i = 0; i < coverTextures.size(); ++i) {
                                                                Texture2D* choiceTexture = &coverTextures[i];

                                                                CLAY(CLAY_IDI("Possible_book_selector", i), {
                                                                    .layout = {
                                                                        .sizing = layout_book
                                                                    },
                                                                    .image = choiceTexture
                                                                }) {
                                                                    if ( Clay_Hovered() ) {
                                                                        selected_image_for_change = i;
                                                                    }

                                                                    if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                                                                        hasPendingUpdateImg = true;
                                                                        open_image_menu_index = -1;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
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
                                                CLAY_TEXT(bookNameText, large);
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
                                                CLAY_TEXT(bookAuthorText, medium);
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
                                                CLAY_TEXT(bookCommentText, medium);
                                            }

                                            CLAY(CLAY_IDI("Book_text_rate_",book_index)){
                                                const int bookRate = lib[book_index].GetRate();
                                                const std::string& bookRateLabel = std::to_string(bookRate) + " / 10";
                                                Clay_String bookRateText = {
                                                    .isStaticallyAllocated = false,
                                                    .length = (int32_t)bookRateLabel.size(),
                                                    .chars = bookRateLabel.c_str()
                                                };
                                                CLAY_TEXT(bookRateText, medium);
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
        if ( hasPendingUpdateImg ){
            fsqlUpdateImg(db,obj_book_id,selected_image_for_change);
            lib = ReadFromDatabase(db);
            obj_book_id.clear();
            selected_image_for_change = 0;
            hasPendingUpdateImg = false;
        }

        if ( allow_img_input ){
            int key = GetCharPressed();
            while(key > 0){
                if ((key >= 32) && (key <= 125))
                {
                    img_input_path.push_back((char)key);
                }
                key = GetCharPressed();
            }
            if ( IsKeyPressed ( KEY_BACKSPACE ) ){
                img_input_path.pop_back();
            }
            if ( IsKeyPressed ( KEY_BACKSPACE ) && IsKeyDown(KEY_LEFT_CONTROL) ){
                img_input_path.clear();
            }
            if( IsKeyPressed ( KEY_ENTER ) ){
                Texture2D new_img = LoadTexture(img_input_path.c_str());
                if(new_img.id != 0){ 
                    coverTextures.push_back(new_img);
                    FileCopy(img_input_path.c_str(), image_dir.c_str());
                    img_input_path.clear();
                    allow_img_input = false; 
                }else{
                    img_input_path = "Error loading img";
                }
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands, fonts);
        EndDrawing();

    } // Main loop

    for (size_t i = 0; i < coverTextures.size(); ++i) {
        UnloadTexture(coverTextures[i]);
    }

    for( auto &i : images )
        UnloadImage(i);

    CloseWindow();
    sqlite3_close(db);

    return 0;
}    