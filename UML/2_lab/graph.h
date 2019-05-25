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
    virtual bool connect(int from, int to) = 0;

    // Adds new element to graph
    virtual bool add(T value) = 0;

    // Returns true if there is given value
    bool has(T value) override {
        return std::find(elementsToIndex.begin(), elementsToIndex.end(), value) != elementsToIndex.end();
    }

    // Returns index of value. Use with has function
    short int index(T value) override {
        typename vector<T>::iterator it = std::find(elementsToIndex.begin(), elementsToIndex.end(), value);
        short int ind = -1;
        if (it != elementsToIndex.end())
            ind = std::distance(elementsToIndex.begin(), it);
        return ind;
    }
};

template <typename T>
class GraphList : public Graph<T> {
private:
    enum Color { white, gray, black };
    vector<vector<short int>> list;

    bool pushIfDoesntContain(vector<short int> &vec, short int value) {
        if (std::find(vec.begin(), vec.end(), value) == vec.end()) {
            vec.push_back(value);
            return true;
        }
        return false;
    }

    bool bfs(int ind, int fathers[], int length[], bool used[]) override {

    }

public:
    bool isConnected() override {
        
    }

    int distance(T first, T second) override {

    }

    int distanceByIndex(int first, int second) override {

    }

    bool remove(T value) override {
        return removeByIndex(index(value));
    }

    bool removeByIndex(short int ind) override {
        // If index is invalid
        if (ind > list.size())
            return false;

        // Remove connections with ind node
        for (vector<short int> vec : list) {
            vec.erase(std::remove_if(vec.begin(), vec.end(), [&ind](short int found){ return found == ind; }));
        }

        // Disconnect node from other nodes
        list[ind].clear();
        return true;
    }

    bool connect(short int from, short int to) override {
        // If indexes are invalid
        if ((from >= Graph<T>::elementsToIndex.size()) || (to >= Graph<T>::elementsToIndex.size()))
            return false;
        // Connect if doesn't connected
        pushIfDoesntContain(list[from], to);
        pushIfDoesntContain(list[to], from);
        return true;
    }

    bool add(T value) override {
        // Add new value to elementToIndex vector if it doesn't exist yet
        if (has(value)) {
            Graph<T>::elementsToIndex.push_back(value);
            list.push_back(new vector<short int>);
            return true;
        }
        return false;
    }
};

/*
template <typename T>
class GraphMatrix : public Graph<T> {
private:
    short int **matrix;
    int elements;
public:
};
 */

#endif //INC_2_LAB_GRAPH_H
