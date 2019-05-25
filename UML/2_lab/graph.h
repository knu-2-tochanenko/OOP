//
// Created by Vladislav Tochanenko on 25.05.2019.
//

#ifndef INC_2_LAB_GRAPH_H
#define INC_2_LAB_GRAPH_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using std::vector;
using std::queue;

template <typename T>
class Graph {
protected:
    // List of index - value
    vector<T> elementsToIndex;
    vector<bool> isUsed;

    virtual bool bfs(int ind, int fathers[], int length[], bool used[]) = 0;

public:
    // Checks if graph is connected
    virtual bool isConnected() = 0;

    // Finds distance between two nodes
    virtual int distance(T first, T second) = 0;

    // Finds distance between two nodes
    virtual int distanceByIndex(int first, int second) = 0;

    // Removes element by value
    // Returns true if element was removed successfully
    virtual bool remove(T value) = 0;

    // Removes element by index
    // Returns true if element was removed successfully
    virtual bool removeByIndex(short int ind) = 0;

    // Connects two nodes
    // Returns true if two nodes were connected successfully
    virtual bool connect(short int from, short int to) = 0;

    // Adds new element to graph
    virtual bool add(T value) = 0;

    // Returns true if there is given value
    bool has(T value) {
        return std::find(elementsToIndex.begin(), elementsToIndex.end(), value) != elementsToIndex.end();
    }

    // Returns index of value. Use with has function
    short int index(T value) {
        typename vector<T>::iterator it = std::find(elementsToIndex.begin(), elementsToIndex.end(), value);
        short int ind = -1;
        if (it != elementsToIndex.end())
            ind = std::distance(elementsToIndex.begin(), it);
        return ind;
    }
};

#endif //INC_2_LAB_GRAPH_H
