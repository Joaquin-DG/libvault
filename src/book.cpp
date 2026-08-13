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


int book::GetImg(){ return img_; }
int book::GetRate(){ return rate_; }

book::book(){
    id_ = generarId();
    name_ = "unknown";
    author_ = "unknown";
    comment_ = "unknown";
    img_ = 0;
    rate_ = 0;
}

book::book(std::string id){
    id_ = id;
    name_ = "unknown";
    author_ = "unknown";
    comment_ = "unknown";
    img_ = 0;
    rate_ = 0;
}

book::book(std::string id, std::string name, std::string author, std::string comment ,int img, int rate){
    id_ = id;
    name_ = name;
    author_ = author;
    comment_ = comment;
    img_ = img;
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

std::string GetCommandOutput(const std::string& command) {
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "ERROR: popen failed!";
    
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

std::vector<std::string> GetArrayImages(const std::string& command_output){
    std::vector<std::string> list;
    std::istringstream iss(command_output);

    std::string line;
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        list.push_back(line);
    }

    return list;
}
