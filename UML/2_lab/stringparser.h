//
// Created by Vlad on 27.05.2019.
//

#ifndef INC_2_LAB_STRINGPARSER_H
#define INC_2_LAB_STRINGPARSER_H

#include <iostream>
#include <vector>

using std::vector;
using std::string;

class StringParser {
public:
    static vector<string> separate(string str, char det) {
        vector<string> res;
        int len = str.length();
        string sub = "";
        for (int i = 0; i < len; i++) {
            if (str[i] == det) {
                res.push_back(sub);
                sub = "";
            }
            else
                sub += str[i];
        }
        return res;
    }
};

#endif //INC_2_LAB_STRINGPARSER_H
