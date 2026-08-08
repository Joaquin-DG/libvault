#pragma once
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <iostream>
#include <iomanip>

class book {

    private:
    
    std::string id_;
    std::string name_;
    std::string author_;
    std::string comment_;
    std::string path_;
    unsigned char rate_;
    
    public:

    std::string GetId();
    const std::string& GetIdRef() const;
    std::string GetName();
    std::string GetAuthor();
    std::string GetComment();
    std::string GetImgPath();
    int GetRate();

    inline void SetId(std::string id) { id_ = id; }
    inline void SetName(std::string name){ name_ = name;}
    inline void SetAuthor(std::string author){author_ = author;}
    inline void SetComment(std::string comment){comment_ = comment;}
    inline void SetImgPath(std::string path){path_ = path;}
    inline void SetRate(int rate){rate_ = rate;}

    book();
    book(std::string id);
    book(std::string id, std::string name, std::string author, std::string comment , std::string path_ , int rate);

};

typedef std::vector<book> library ;

std::string generarId();

bool find_selected_book(std::vector<std::string> books, std::string id);