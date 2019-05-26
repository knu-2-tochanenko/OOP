//
// Created by Vlad on 25.05.2019.
//

#ifndef INC_2_LAB_IPCONVERTER_H
#define INC_2_LAB_IPCONVERTER_H

#include <iostream>
#include <string>
#include <vector>
#include "hexconverter.h"
#include "stringparser.h"

using std::string;
using std::vector;

class IpConverter {
public:
    static string toV4(unsigned short int address[]) {
        for (int i = 0; i < 6; i++) {
            if (address[i] != 0)
                return "";
        }
        string str = "";
        str += std::to_string(address[6] >> 8);
        str += '.';
        str += std::to_string((unsigned short int) (address[6] << 8) >> 8);
        str += '.';
        str += std::to_string(address[7] >> 8);
        str += '.';
        str += std::to_string((unsigned short int) (address[7] << 8) >> 8);
        return str;
    }

    static string toV6(unsigned short int address[]) {
        string str = "";
        for (int i = 0; i < 7; i++) {
            str += HexConverter::toHex(address[i]);
            str += ':';
        }
        str += HexConverter::toHex(address[7]);
        return str;
    }

    static unsigned short int* fromString(string ip) {
        // Check for IPv4
        unsigned short int *newAddress;
        newAddress = new unsigned short int[8];
        for (int i = 0; i < 8; i++)
            newAddress = 0;
        if (ip.find('.') != std::string::npos) {
            vector<string> res = StringParser::separate(ip, '.');
            for (int i = 0; i < res.size(); i++)
                newAddress[4 + i] = std::stoi(res[i]);
        }
        else if (ip.find(':') != std::string::npos) {
            vector<string> res = StringParser::separate(ip, ':');
            for (int i = 0; i < res.size(); i++)
                newAddress[i] = HexConverter::toInt(res[i]);
        }
        return newAddress;
    }
};

#endif //INC_2_LAB_IPCONVERTER_H
