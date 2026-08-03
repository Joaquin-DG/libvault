#pragma once
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>

class book {

    private:

    public:

        std::string id_;
        std::string title_;
        std::string author_;
        std::string comment_;
        unsigned char rate_;

};

typedef std::vector<book> library ;

std::string generarId();