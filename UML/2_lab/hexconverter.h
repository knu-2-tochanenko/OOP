//
// Created by Vlad on 25.05.2019.
//

#ifndef INC_2_LAB_HEXCONVERTER_H
#define INC_2_LAB_HEXCONVERTER_H

#include <iostream>
#include <string>
#include <iomanip>

class HexConverter {
public:
    template <typename T>
    static std::string toHex(T i) {
        std::stringstream stream;
        stream << std::setfill ('0')
               << std::hex << i;
        return stream.str();
    }

    static int toInt (std::string const str) {
        int res;
        std::stringstream ss;
        ss << std::hex << str;
        ss >> res;
        return res;
    }
};

#endif //INC_2_LAB_HEXCONVERTER_H
