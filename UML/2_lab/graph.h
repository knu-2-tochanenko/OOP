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

    bool pushIfDoesntContain(vector<short int> &vec, short int value) {
        if (std::find(vec.begin(), vec.end(), value) == vec.end()) {
            vec.push_back(value);
            return true;
        }
        return false;
    }

    // Finds distance between two nodes
    virtual int distanceByIndex(int first, int second) = 0;

    // Removes element by index
    // Returns true if element was removed successfully
    virtual bool removeByIndex(short int ind) = 0;

    // Connects two nodes
    // Returns true if two nodes were connected successfully
    virtual bool connectByIndex(short int from, short int to) = 0;

    // Returns index of value. Use with has function
    short int index(T value) {
        typename vector<T>::iterator it = std::find(elementsToIndex.begin(), elementsToIndex.end(), value);
        short int ind = -1;
        if (it != elementsToIndex.end())
            ind = std::distance(elementsToIndex.begin(), it);
        return ind;
    }

public:
    // Checks if graph is connected
    bool isConnected() {
        int listSize = Graph<T>::elementsToIndex.size();
        int *fathers = new int[listSize];
        int *length = new int[listSize];
        bool *used = new bool[listSize];

        bfs(0, fathers, length, used);

        for (int i = 0; i < listSize; i++)
            if (!used[i])
                return false;
        return true;
    }

    // Finds distance between two nodes
    int distance(T first, T second) {
        return distanceByIndex(Graph<T>::index(first), Graph<T>::index(second));
    }

    // Removes element by value
    // Returns true if element was removed successfully
    bool remove(T value) {
        return removeByIndex(Graph<T>::index(value));
    }

    // Connects two nodes
    // Returns true if two nodes were connected successfully
    bool connect(T from, T to) {
        return connectByIndex(Graph<T>::index(from), Graph<T>::index(to));
    }

    // Adds new element to graph
    virtual bool add(T value) = 0;

    // Returns true if there is given value
    bool has(T value) {
        return std::find(elementsToIndex.begin(), elementsToIndex.end(), value) != elementsToIndex.end();
    }
};

#endif //INC_2_LAB_GRAPH_H
