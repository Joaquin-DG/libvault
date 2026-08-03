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
