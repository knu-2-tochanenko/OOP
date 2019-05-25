//
// Created by Vladislav Tochanenko on 25.05.2019.
//

#ifndef INC_2_LAB_GRAPH_H
#define INC_2_LAB_GRAPH_H

#include <iostream>
#include <vector>

using std::vector;

template <typename T>
class Graph {
private:
    // List of index - value
    vector<T> elemetsToIndex;
public:
    // Checks if graph is connected
    virtual bool isConnected() = 0;

    // Finds distance between two nodes
    virtual int distance(int first, int second) = 0;

    // Removes all elements by value
    // Returns true if elements were removed successfully
    virtual bool removeByValue(T value) = 0;

    // Removes element by index
    // Returns true if element was removed successfully
    virtual bool removeByIndex(short int index) = 0;

    // Connects two nodes
    // Returns true if two nodes were connected successfully
    virtual bool connect(int from, int to) = 0;

    // Adds new element to graph
    virtual void add(T value) = 0;
};

template <typename T>
class GraphList : public Graph<T> {
private:
    vector<vector<short int>> list;
public:
    bool isConnected() override {

    }

    int distance(int first, int second) override {

    }

    bool removeByValue(T value) override {

    }

    bool removeByIndex(short int index) override {

    }

    bool connect(int from, int to) override {

    }

    void add(T value) override {

    }
};

template <typename T>
class GraphMatrix : public Graph<T> {
private:
    short int **matrix;
public:
    bool isConnected() override {

    }

    int distance(int first, int second) override {

    }

    bool removeByValue(T value) override {

    }

    bool removeByIndex(short int index) override {

    }

    bool connect(int from, int to) override {

    }

    void add(T value) override {

    }
};

#endif //INC_2_LAB_GRAPH_H
