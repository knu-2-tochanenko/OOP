//
// Created by Vlad on 25.05.2019.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "graphlist.h"

TEST_CASE( "Distance between nodes", "[distance]" ) {
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
}