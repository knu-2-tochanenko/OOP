//
// Created by Vlad on 25.05.2019.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "graphlist.h"
#include "graphmatrix.h"
#include "hexconverter.h"
#include "stringparser.h"
#include "ipconverter.h"
#include "ip.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

TEST_CASE("Hex converter", "[hex][converter]") {
    SECTION("Int to HEX") {
        REQUIRE(HexConverter::toHex(15) == "F");
        REQUIRE(HexConverter::toHex(0) == "0");
        REQUIRE(HexConverter::toHex(8) == "8");
        REQUIRE(HexConverter::toHex(2564) == "A04");
    }
    SECTION("HEX to Int") {
        REQUIRE(HexConverter::toInt("F") == 15);
        REQUIRE(HexConverter::toInt("0") == 0);
        REQUIRE(HexConverter::toInt("8") == 8);
        REQUIRE(HexConverter::toInt("A04") == 2564);
    }
}

TEST_CASE ("String parser", "[parser]") {
    vector<string> first;
    SECTION("127.0.0.1") {
        first.emplace_back("127");
        first.emplace_back("0");
        first.emplace_back("0");
        first.emplace_back("1");
        REQUIRE(StringParser::separate("127.0.0.1", '.') == first);
    }
    SECTION("255.255.255.255") {
        first.emplace_back("255");
        first.emplace_back("255");
        first.emplace_back("255");
        first.emplace_back("255");
        REQUIRE(StringParser::separate("255.255.255.255", '.') == first);
    }
    SECTION("c86c:e301:7241:815:27c8:fae0:5049:10c9") {
        first.emplace_back("c86c");
        first.emplace_back("e301");
        first.emplace_back("7241");
        first.emplace_back("815");
        first.emplace_back("27c8");
        first.emplace_back("fae0");
        first.emplace_back("5049");
        first.emplace_back("10c9");
        REQUIRE(StringParser::separate("c86c:e301:7241:815:27c8:fae0:5049:10c9", ':') == first);
    }
    SECTION("0:0:0:0:0:0:0:0") {
        first.emplace_back("0");
        first.emplace_back("0");
        first.emplace_back("0");
        first.emplace_back("0");
        first.emplace_back("0");
        first.emplace_back("0");
        first.emplace_back("0");
        first.emplace_back("0");
        REQUIRE(StringParser::separate("0:0:0:0:0:0:0:0", ':') == first);
    }
}

TEST_CASE("Ip", "[ip]") {
    IP ipV4("127.0.0.1");
    IP ipV6("0:0:0:0:0:ffff:7f00:1");
    SECTION("Write V4 and V6 from V4") {
        REQUIRE(ipV4.toV4() == "127.0.0.1");
        REQUIRE(ipV4.toV6() == "0:0:0:0:0:ffff:7f00:1");
    }
    SECTION("Write V4 and V6 from V6") {
        REQUIRE(ipV6.toV4() == "127.0.0.1");
        REQUIRE(ipV6.toV6() == "0:0:0:0:0:ffff:7f00:1");
    }
    SECTION("Test for subnet") {
        IP ipV4_2("127.0.0.120");
        ipV4.mask(8);
        REQUIRE(ipV4.hasIP(ipV4_2));
        ipV4.mask(1);
        REQUIRE(!ipV4.hasIP(ipV4_2));
    }
}

TEST_CASE("Distance between nodes", "[distance]") {
    GraphList<char> gl;
    gl.add('a');
    gl.add('b');
    gl.add('c');
    gl.add('d');
    gl.add('e');
    gl.add('f');
    gl.add('g');
    gl.connect('a', 'b');
    gl.connect('b', 'c');
    gl.connect('c', 'd');
    gl.connect('a', 'e');
    gl.connect('e', 'd');
    gl.connect('f', 'g');

    REQUIRE(gl.distance('a', 'd') == 2);
    REQUIRE(gl.distance('g', 'f') == 1);
    REQUIRE(!gl.isConnected());

    GraphMatrix<char> gm;
    gm.add('a');
    gm.add('b');
    gm.add('c');
    gm.add('d');
    gm.add('e');
    gm.add('f');
    gm.add('g');
    gm.connect('a', 'b');
    gm.connect('b', 'c');
    gm.connect('c', 'd');
    gm.connect('a', 'e');
    gm.connect('e', 'd');
    gm.connect('f', 'g');

    REQUIRE(gm.distance('a', 'd') == 2);
    REQUIRE(gm.distance('g', 'f') == 1);
    REQUIRE(!gm.isConnected());
}