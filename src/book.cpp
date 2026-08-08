#include "book.hpp"

std::string generarId(){
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0,255);

    std::stringstream ss;
    for(int i = 0 ; i < 4 ; ++i){
        ss << std::hex <<std::setw(2) << std::setfill('0') << dis(gen);
    }

    return ss.str();
}

std::string book::GetId(){ return id_; }

const std::string& book::GetIdRef() const { return id_; }

std::string book::GetName(){ return name_; }

std::string book::GetAuthor(){ return author_; }

std::string book::GetComment(){ return comment_; }

int book::GetRate(){ return rate_; }

book::book(){
    id_ = generarId();
    name_ = "unknown";
    author_ = "unknown";
    comment_ = "unknown";
    rate_ = 0;
}

book::book(std::string id){
    id_ = id;
    name_ = "unknown";
    author_ = "unknown";
    comment_ = "unknown";
    rate_ = 0;
}

book::book(std::string id, std::string name, std::string author, std::string comment , int rate){
    id_ = id;
    name_ = name;
    author_ = author;
    comment_ = comment;
    rate_ = rate;
}

