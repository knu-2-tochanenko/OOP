#include <utility>

#include <utility>

//
// Created by Vlad on 25.05.2019.
//

#ifndef INC_2_LAB_IP_H
#define INC_2_LAB_IP_H

#include <iostream>
#include <string>
#include <iomanip>
#include "ipconverter.h"

using std::string;

class IP {
private:
    unsigned short int masks[16] = {
            65535,
            65534,
            65532,
            65528,
            65520,
            65504,
            65472,
            65408,
            65280,
            65024,
            64512,
            63488,
            61440,
            57344,
            49152,
            32768
    };

    unsigned short int address[8];
    unsigned short int ipMask;

    bool isV4() {
        for (int i = 0; i < 6; i++)
            if (address[i] != 0)
                return false;
        return true;
    }

public:

    IP() {
        for (unsigned short & octet : address)
            octet = 0;
        ipMask = 0;
    }

    IP(string str) {
        unsigned short *res = IpConverter::fromString(std::move(str));
        ipMask = 0;
    }

    IP(string str, unsigned short int ipMask) {
        unsigned short *res = IpConverter::fromString(std::move(str));
        this->ipMask = ipMask;
    }

    void mask(unsigned short int ipMask) {
        this->ipMask = ipMask % 128;
    }

    bool hasIP(IP ip) {
        int subMask = this->ipMask;
        int currentMask;
        for (int i = 7; subMask > 0 && i >= 0; i++) {
            currentMask = (subMask >= 16) ? 16 : subMask;
            subMask -= 16;
            if (currentMask == 16) {
                continue; // Skip current octet
            }
            if ((this->address[i] & masks[currentMask])
            != (ip.address[i] & masks[currentMask]))
                return false;
        }
        return true;
    }


    string toV4() {
        if (!isV4())
            return "IP is not IPv4";
        return IpConverter::toV4(address);
    }

    string toV6() {
        return IpConverter::toV6(address);
    }
};

#endif //INC_2_LAB_IP_H
