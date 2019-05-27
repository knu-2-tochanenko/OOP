//
// Created by Vlad on 25.05.2019.
//

#ifndef INC_2_LAB_GRAPHMATRIX_H
#define INC_2_LAB_GRAPHMATRIX_H

#include "graph.h"

template <typename T>
class GraphMatrix : public Graph<T> {
private:
    short int **matrix;

    bool bfs(int ind, int fathers[], int length[], bool used[]) override {
        std::queue<int> nodes;
        int listSize = Graph<T>::elementsToIndex.size();
        for (int i = 0; i < listSize; i++) {
            fathers[i] = 0;
            length[i] = INT_MAX;
            used[i] = !Graph<T>::isUsed[i];
        }

        used[ind] = true;
        fathers[ind] = -1;
        nodes.push(ind);
        length[ind] = 0;

        while (!nodes.empty()) {
            int curVertex = nodes.front();
            nodes.pop();

            //	Scan all connected nodes
            for (int curNode = 0; curNode < listSize; curNode++) {
                //	If vertex is not visited
                if (matrix[curVertex][curNode] != 0)
                    if (!used[curNode]) {
                        used[curNode] = true;
                        nodes.push(curNode);
                        length[curNode] = length[curVertex] + 1;
                        fathers[curNode] = curVertex;
                    }
            }
        }
        return true;
    }

    void expandMatrix() {
        int elements = Graph<T>::elementsToIndex.size();
        int newElements = elements + 1;

        short int **newMatrix;
        newMatrix = new short int*[newElements];
        for (int i = 0; i < elements; i++) {
            newMatrix[i] = new short int[newElements];
            for (int j = 0; j < elements; j++)
                newMatrix[i][j] = matrix[i][j];
            newMatrix[i][elements] = 0;
        }
        newMatrix[elements] = new short int[newElements];
        for (int i = 0; i < newElements; i++)
            newMatrix[elements][i] = 0;
        matrix = newMatrix;
    }

    int distanceByIndex(int first, int second) override {
        int listSize = Graph<T>::elementsToIndex.size();
        int *fathers = new int[listSize];
        int *length = new int[listSize];
        bool *used = new bool[listSize];

        if (first * second < 0)
            return -1; //	There is no nodes with firstNode or secondNode values

        bfs(first, fathers, length, used);

        if (length[second] == INT_MAX)
            return -1;
        else
            return length[second];
    }

    bool removeByIndex(short int ind) override {
        // If index is invalid
        int elements = Graph<T>::elementsToIndex.size();
        if (ind > elements)
            return false;

        // Remove connections with ind node
        // Disconnect node from other nodes
        for (int i = 0; i < elements; i++) {
            matrix[ind][i] = matrix[i][ind] = 0;
        }

        Graph<T>::isUsed[ind] = false;
        return true;
    }

    bool connectByIndex(short int from, short int to) override {
        // If indexes are invalid
        if ((from >= Graph<T>::elementsToIndex.size()) || (to >= Graph<T>::elementsToIndex.size()))
            return false;
        // Connect if doesn't connected
        matrix[from][to] = matrix[to][from] = 1;
        return true;
    }

public:

    bool add(T value) override {
        // Add new value to elementToIndex vector if it doesn't exist yet
        if (!Graph<T>::has(value)) {
            // Resize matrix
            expandMatrix();
            Graph<T>::elementsToIndex.push_back(value);
            Graph<T>::isUsed.push_back(true);

            return true;
        }
        Graph<T>::isUsed[Graph<T>::index(value)] = true;
        return false;
    }
};

#endif //INC_2_LAB_GRAPHMATRIX_H
