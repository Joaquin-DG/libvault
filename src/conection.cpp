#include "conection.hpp"

const char* sqlRead = "SELECT id, name, author, comment, rate FROM book;";
const char* sqlInsert = "INSERT into book (id, name, author, comment, rate) VALUES (?, ?, ?, ?, ?);";
const char* sqlDelete = "DELETE FROM book WHERE id = ?;";
const char* sqlUpdateName = "UPDATE book SET name = ? WHERE id = ?;";
const char* sqlUpdateAuthor = "UPDATE book SET author = ? WHERE id = ?;";
const char* sqlUpdateComment = "UPDATE book SET comment = ? WHERE id = ?;";
const char* sqlUpdateRate = "UPDATE book SET rate = ? WHERE id = ?;";


bool fsqlInsert(sqlite3* db,std::string name, std::string author, std::string comment , int rate){
    
    std::string id = generarId();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, author.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, comment.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int64(stmt, 5, rate);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << sqlite3_errmsg(db) << "\n";
            return false;
        }
        
        sqlite3_finalize(stmt);
    } else {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return false;
    }

    return true;
}

bool fsqlInsert(sqlite3* db){
    
    std::string id = generarId();
    std::string name = "";
    std::string author = "";
    std::string comment = "";
    int rate = 0;
    
    sqlite3_stmt* stmt;


    if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, author.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, comment.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int64(stmt, 5, rate);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << sqlite3_errmsg(db) << "\n";
            return false;
        }
        
        sqlite3_finalize(stmt);
    } else {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return false;
    }

    return true;
}


bool fsqlUpdateName(sqlite3* db, std::string id, std::string name) { 
        
    sqlite3_stmt* stmt;
        
    if ( sqlite3_prepare_v2(db, sqlUpdateName, -1 , &stmt , nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt , 1, name.c_str() , -1 , SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt , 2, id.c_str() , -1 , SQLITE_TRANSIENT);
        
        if ( sqlite3_step (stmt) != SQLITE_DONE ) {
            std::cerr << sqlite3_errmsg(db) << "\n";
            return false;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return false;
    }

    return true;
}

bool fsqlUpdateAuthor(sqlite3* db, std::string id, std::string author){

    sqlite3_stmt* stmt;
        
    if ( sqlite3_prepare_v2(db, sqlUpdateAuthor, -1 , &stmt , nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt , 1, author.c_str() , -1 , SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt , 2, id.c_str() , -1 , SQLITE_TRANSIENT);
        
        if ( sqlite3_step (stmt) != SQLITE_DONE ) {
            std::cerr << sqlite3_errmsg(db) << "\n";
            return false;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return false;
    }

    return true;
}

bool fsqlUpdateComment(sqlite3* db, std::string id, std::string comment){

    sqlite3_stmt* stmt;
        
    if ( sqlite3_prepare_v2(db, sqlUpdateComment, -1 , &stmt , nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt , 1, comment.c_str() , -1 , SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt , 2, id.c_str() , -1 , SQLITE_TRANSIENT);
        
        if ( sqlite3_step (stmt) != SQLITE_DONE ) {
            std::cerr << sqlite3_errmsg(db) << "\n";
            return false;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return false;
    }

    return true;
}


bool fsqlUpdateRate(sqlite3* db, std::string id, int rate){

    sqlite3_stmt* stmt;
        
    if ( sqlite3_prepare_v2(db, sqlUpdateRate, -1 , &stmt , nullptr) == SQLITE_OK) {
        
        sqlite3_bind_int(stmt , 1, rate);
        sqlite3_bind_text(stmt , 2, id.c_str() , -1 , SQLITE_TRANSIENT);
        
        if ( sqlite3_step (stmt) != SQLITE_DONE ) {
            std::cerr << sqlite3_errmsg(db) << "\n";
            return false;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return false;
    }

    return true;
}

bool fsqlDelete(sqlite3* db, std::string id){
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        std::cerr << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }

    int stepResult = sqlite3_step(stmt);
    if (stepResult != SQLITE_DONE) {
        std::cerr << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

library ReadFromDatabase(sqlite3* db){
    library lib;
    
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sqlRead, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return lib;
    }

    while (true) {
        int stepResult = sqlite3_step(stmt);

        if (stepResult == SQLITE_ROW) {
            const unsigned char* idText = sqlite3_column_text(stmt, 0);
            const unsigned char* nameText = sqlite3_column_text(stmt, 1);
            const unsigned char* authorText = sqlite3_column_text(stmt, 2);
            const unsigned char* commentText = sqlite3_column_text(stmt, 3);
            int rate = sqlite3_column_int(stmt, 4);

            std::string id = idText ? reinterpret_cast<const char*>(idText) : "";
            std::string name = nameText ? reinterpret_cast<const char*>(nameText) : "";
            std::string author = authorText ? reinterpret_cast<const char*>(authorText) : "";
            std::string comment = commentText ? reinterpret_cast<const char*>(commentText) : "";

            book newBook(id, name, author, comment, rate);
            lib.push_back(newBook);
        } else if (stepResult == SQLITE_DONE) {
            break;
        } else {
            std::cerr << sqlite3_errmsg(db) << "\n";
            lib.clear();
            sqlite3_finalize(stmt);
            return lib;
        }
    }

    sqlite3_finalize(stmt);
    return lib;
}
