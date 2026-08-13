#pragma once
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>

class book {

    private:
    
    std::string id_;
    std::string name_;
    std::string author_;
    std::string comment_;
    unsigned char img_;
    unsigned char rate_;
    
    public:

    const std::string& GetId() const;
    const std::string& GetName() const;
    const std::string& GetAuthor() const;
    const std::string& GetComment() const;
    int GetImg();
    int GetRate();

    inline void SetId(std::string id) { id_ = id; }
    inline void SetName(std::string name){ name_ = name;}
    inline void SetAuthor(std::string author){author_ = author;}
    inline void SetComment(std::string comment){comment_ = comment;}
    inline void SetImg(int img){img_ = img;}
    inline void SetRate(int rate){rate_ = rate;}

    book();
    book(std::string id);
    book(std::string id, std::string name, std::string author, std::string comment , int img_ , int rate);

};

typedef std::vector<book> library ;

std::string generarId();

bool find_selected_book(std::vector<std::string> books, std::string id);

std::string GetCommandOutput(const std::string& command);

std::vector<std::string> GetArrayImages(const std::string& command_output);