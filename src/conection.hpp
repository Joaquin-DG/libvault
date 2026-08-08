#pragma once

#include <sqlite3.h>
#include <string>
#include "book.hpp"

extern const char* sqlInsert;
extern const char* sqlDelete;
extern const char* sqlModifyName;
extern const char* sqlModifyAuthor;
extern const char* sqlModifyComment;
extern const char* sqlModifyPath;
extern const char* sqlModifyRate;

bool fsqlInsert(sqlite3* db, std::string name, std::string author, std::string comment ,std::string path , int rate);
bool fsqlInsert(sqlite3* db);
bool fsqlDelete(sqlite3* db, std::string id);
bool fsqlUpdateName(sqlite3* db, std::string id, std::string name);
bool fsqlUpdateAuthor(sqlite3* db, std::string id, std::string author);
bool fsqlUpdateComment(sqlite3* db, std::string id, std::string comment);
bool fsqlUpdatePath(sqlite3* db, std::string id, std::string path);
bool fsqlUpdateRate(sqlite3* db, std::string id, int rate);

library ReadFromDatabase(sqlite3* db);

