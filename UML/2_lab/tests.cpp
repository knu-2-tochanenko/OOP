//
// Created by Vlad on 25.05.2019.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "graphlist.h"
#include "graphmatrix.h"

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