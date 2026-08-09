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

const std::string& book::GetId() const { return id_; }
const std::string& book::GetName() const { return name_; }
const std::string& book::GetAuthor() const { return author_; }
const std::string& book::GetComment() const { return comment_; }
const std::string& book::GetImgPath() const { return path_; }

int book::GetRate(){ return rate_; }

book::book(){
    id_ = generarId();
    name_ = "unknown";
    author_ = "unknown";
    comment_ = "unknown";
    path_ = "unknown";
    rate_ = 0;
}

book::book(std::string id){
    id_ = id;
    name_ = "unknown";
    author_ = "unknown";
    comment_ = "unknown";
    path_ = "unknown";
    rate_ = 0;
}

book::book(std::string id, std::string name, std::string author, std::string comment ,std::string path, int rate){
    id_ = id;
    name_ = name;
    author_ = author;
    comment_ = comment;
    path_ = path;
    rate_ = rate;
}

bool find_selected_book(std::vector<std::string> books, std::string id){
    for ( const auto &b : books ){
        if (id == b){
            return true;
        }
    }
    return false;
}