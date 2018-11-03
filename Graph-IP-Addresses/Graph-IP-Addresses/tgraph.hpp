#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <ctime>
#include <random>

namespace tvv {
	/*
	*	Class Graph is used to store elements of various types
	//*/
	template <typename TNodeType> class Graph {
	private:
		/*
		*	Variable which is used to tell which
		*	structure we are working with.
		*	true - List (default value)
		*	false - matrix
		//*/
		bool ifWorkWithList;
		/*
		*	listOfEdges will be generated no matter what
		*	The first element in each sub vector is the name of the node
		//*/
		std::vector<std::vector<short int>> listOfEdges;

		/*	matrixOfEdges will be generated only if user wants to
		*	DANGER! Slow zone!
		*	I COULD made a matrix of 1-bit variables, but the lowest possible
		*	variable type, which doesn't have garbage in leftover bits,
		*	is short int
		//*/
		short int **matrixOfEdges;
		//	Number of the element in this vector is it's number in the Matrix & List
		std::vector<TNodeType> elementToNumberList;

		/*
		*	Returns true if element was added successfully
		*	Returns false if element exist
		//*/
		bool addElementList(TNodeType elementToAdd) {
			short int listSize = listOfEdges.size();
			//	If the list is empty
			if (listSize == 0) {
				listOfEdges.push_back(std::vector<short int>());
				elementToNumberList.push_back(elementToAdd);
				return true;
			}

			//	Search for the existing node
			for (int vectorIterator = 0; vectorIterator < listSize; vectorIterator++) {
				if (elementToNumberList[vectorIterator] == elementToAdd)
					return false;
			}
			listOfEdges.push_back(std::vector<short int>());
			elementToNumberList.push_back(elementToAdd);
			return true;
		}
		bool addElementMatrix(TNodeType elementToAdd) {
			int listSize = elementToNumberList.size();
			//	If the Matrix is empty
			if (listSize == 0) {
				matrixOfEdges = new short int*[1];
				matrixOfEdges[0] = new short int[1];
				matrixOfEdges[0][0] = 0;
				elementToNumberList.push_back(elementToAdd);
				return true;
			}

			//	Search for the existing node
			for (int vectorIterator = 0; vectorIterator < listSize; vectorIterator++) {
				if (elementToNumberList[vectorIterator] == elementToAdd)
					return false;
			}

			//	If the element is new
			matrixChangeNumberOfEdges(+1, listSize);
			elementToNumberList.push_back(elementToAdd);
			return true;
		}

		/*
		*	Returns true if element was erased successfully
		*	Returns false if element doesn't exist
		//*/
		bool eraseElementList(TNodeType elementToErase) {
			int listSize = elementToNumberList.size();
			if (listSize == 0)
				return false;

			int elementToEraseNumber = -1;
			for (int vectorIterator = 0; vectorIterator < listSize; vectorIterator++)
				if (elementToNumberList[vectorIterator] == elementToErase) {
					elementToEraseNumber = vectorIterator;
					break;
				}
			//	If there is no element with elementToErase value
			if (elementToEraseNumber == -1)
				return false;

			//	Remove all connectivity with other nodes
			int numberOfConnectedNodes = listOfEdges[elementToEraseNumber].size();
			for (int vectorIterator = 0; vectorIterator < numberOfConnectedNodes; vectorIterator++) {
				for (int connectedIterator = 0;
					connectedIterator < listOfEdges[listOfEdges[elementToEraseNumber][vectorIterator]].size();
					connectedIterator++) {
					if (listOfEdges[listOfEdges[elementToEraseNumber][vectorIterator]][connectedIterator] == elementToEraseNumber)
						listOfEdges[listOfEdges[elementToEraseNumber][vectorIterator]].erase(listOfEdges[listOfEdges[elementToEraseNumber][vectorIterator]].begin() + connectedIterator);
				}
			}

			listOfEdges.erase(listOfEdges.begin() + elementToEraseNumber);
			elementToNumberList.erase(elementToNumberList.begin() + elementToEraseNumber);
			return true;
		}
		bool eraseElementMatrix(TNodeType elementToErase) {
			//	If the Matrix is empty
			int listSize = elementToNumberList.size();
			if (listSize == 0)
				return false;

			//	Search for the existing node
			for (int vectorIterator = 0; vectorIterator < listSize; vectorIterator++) {
				if (elementToNumberList[vectorIterator] == elementToErase) {
					// Delete connection between number and selected element
					elementToNumberList.erase(elementToNumberList.begin() + vectorIterator);
					//	Delete %vectorIterator% row and column
					for (int columnIterator = 0; columnIterator < listSize; columnIterator++)
						for (int rowIterator = vectorIterator; rowIterator < listSize - 1; rowIterator++)
							matrixOfEdges[columnIterator][rowIterator] = matrixOfEdges[columnIterator][rowIterator + 1];
					for (int rowIterator = 0; rowIterator < listSize; rowIterator++)
						for (int columnIterator = vectorIterator; columnIterator < listSize - 1; columnIterator++)
							matrixOfEdges[columnIterator][rowIterator] = matrixOfEdges[columnIterator + 1][rowIterator];
					matrixChangeNumberOfEdges(-1, listSize);
				}
				return false;
			}

			return false;
		}

		/*
		*	Returns true if all elements are in one connected component
		*	false - if not
		//*/
		bool checkForConnectivityList() {
			int listSize = elementToNumberList.size();
			int *fathers = new int[listSize];
			int *length = new int[listSize];
			bool *used = new bool[listSize];

			breadthFirstSearchForList(0, fathers, length, used);

			for (int i = 0; i < listSize; i++)
				if (!used)
					return false;
			return true;
		}
		bool checkForConnectivityMatrix() {
			int listSize = elementToNumberList.size();
			int *fathers = new int[listSize];
			int *length = new int[listSize];
			bool *used = new bool[listSize];

			breadthFirstSearchForMatrix(0, fathers, length, used);

			for (int i = 0; i < listSize; i++)
				if (!used)
					return false;
			return true;
		}

		//	Finds a number for specific TNodeType element in list
		int findNumberInList(TNodeType vertex) {
			int listSize = elementToNumberList.size();
			for (int i = 0; i < listSize; i++)
				if (elementToNumberList[i] == vertex)
					return i;
			return -1;
		}

		/*
		*	Breadth-First Search for different formats.
		*	Makes three different useful arrays
		//*/
		bool breadthFirstSearchForList(int vertex, int fathers[], int length[], bool used[]) {
			std::queue<int> nodes;
			int listSize = elementToNumberList.size();
			for (int i = 0; i < listSize; i++) {
				fathers[i] = 0;
				length[i] = INT64_MAX;
				used[i] = false;
			}

			used[vertex] = true;
			fathers[vertex] = -1;
			nodes.push(vertex);

			while (nodes.empty() != true) {
				int cur_vertex = nodes.front();
				nodes.pop();

				//	Scan all connected nodes
				int numberOfNodes = listOfEdges[cur_vertex].size();
				for (int vectorIterator = 0; vectorIterator < numberOfNodes; vectorIterator++) {
					int connectedNumber = listOfEdges[cur_vertex][vectorIterator];
					//	If vertex is not visited
					if (used[connectedNumber] == false) {
						used[connectedNumber] = true;
						nodes.push(connectedNumber);
						length[connectedNumber] = length[cur_vertex] + 1;
						fathers[connectedNumber] = cur_vertex;
					}
				}
			}
			return true;
		}
		bool breadthFirstSearchForMatrix(int vertex, int fathers[], int length[], bool used[]) {
			std::queue<int> nodes;
			int listSize = elementToNumberList.size();
			for (int i = 0; i < listSize; i++) {
				fathers[i] = 0;
				length[i] = INT64_MAX;
				used[i] = false;
			}

			used[vertex] = true;
			fathers[vertex] = -1;
			nodes.push(vertex);

			while (nodes.empty() != true) {
				int curVertex = nodes.front();
				nodes.pop();

				for (int curNode = 0; curNode < listSize; curNode++) {
					//	If vertex is not visited
					if (matrixOfEdges[curVertex][curNode] != 0)
						if (used[curNode] == false) {
							used[curNode] = true;
							nodes.push(curNode);
							length[curNode] = length[curVertex] + 1;
							fathers[curNode] = curVertex;
						}
				}
			}
			return true;
		}

		/*
		*	Returns the minimal length between two elements
		*	Returns -1 if there is no such way
		//*/
		int findMinLengthList(TNodeType firstNode, TNodeType secondNode) {
			int listSize = elementToNumberList.size();
			int *fathers = new int[listSize];
			int *length = new int[listSize];
			bool *used = new bool[listSize];

			int firstNumber = findNumberInList(firstNode);
			int secondNumber = findNumberInList(secondNode);
			if (firstNumber * secondNumber < 0)
				return -1; //	There is no nodes with firstNode or secondNode values

			breadthFirstSearchForList(firstNumber, fathers, length, used);

			if (length[secondNumber] == INT64_MAX)
				return -1;
			else
				return length[secondNumber];
			
		}
		int findMinLengthMatrix(TNodeType firstNode, TNodeType secondNode) {
			int listSize = elementToNumberList.size();
			int *fathers = new int[listSize];
			int *length = new int[listSize];
			bool *used = new bool[listSize];

			int firstNumber = findNumberInList(firstNode);
			int secondNumber = findNumberInList(secondNode);

			breadthFirstSearchForMatrix(firstNumber, fathers, length, used);

			if (length[secondNumber] == INT64_MAX)
				return -1;
			else
				return length[secondNumber];
		}

		/*
		*	Connects two elements
		*	Returns false if it is the same element
		*	ACHTUNG! YOU MAY CHANGE THIS FUNCTION TO ALLOW LOOPS
		//*/
		bool connectElementToAnotherElementList(TNodeType firstNode, TNodeType secondNode) {
			//	Not allow making loops
			//	Delete to 'END" point if you want to allow loops
			if (firstNode == secondNode)
				return false;
			//	END

			int firstNodeNumber = findNumberInList(firstNode);
			int secondNodeNumber = findNumberInList(secondNode);
			
			//	If one or both of these nodes are new
			if (firstNodeNumber == -1) {
				addElementList(firstNode);
				firstNodeNumber = elementToNumberList.size();
			}
			if (secondNodeNumber == -1) {
				addElementList(secondNode);
				secondNodeNumber = elementToNumberList.size();
			}
			
			//	Check if there is already a connection between these nodes
			bool firstToSecond = false, secondToFirst = false;
			int numberOfNodes = listOfEdges[firstNodeNumber].size();
			for (int vectorIterator = 0; vectorIterator < numberOfNodes; vectorIterator++)
				if (listOfEdges[firstNodeNumber][vectorIterator] == secondNodeNumber) {
					firstToSecond = true;
					break;
				}
			numberOfNodes = listOfEdges[secondNodeNumber].size();
			for (int vectorIterator = 0; vectorIterator < numberOfNodes; vectorIterator++)
				if (listOfEdges[secondNodeNumber][vectorIterator] == firstNodeNumber) {
					secondToFirst = true;
					break;
				}

			//	If there is no connection
			if (!firstToSecond)
				listOfEdges[firstNodeNumber].push_back(secondNodeNumber);
			if (!secondToFirst)
				listOfEdges[secondNodeNumber].push_back(firstNodeNumber);
			return true;
		}
		bool connectElementToAnotherElementMatrix(TNodeType firstNode, TNodeType secondNode) {
			int firstNumber = -1, secondNumber = -1;
			int listSize = elementToNumberList.size();
			for (int i = 0; i < listSize; i++)
				if (elementToNumberList[i] == firstNode) {
					firstNumber = i;
					break;
				}
			for (int i = 0; i < listSize; i++)
				if (elementToNumberList[i] == secondNode) {
					secondNumber = i;
					break;
				}

			if (firstNumber == -1) {
				addElementMatrix(firstNode);
				firstNumber = elementToNumberList.size();
			}
			if (secondNumber == -1) {
				addElementMatrix(secondNode);
				secondNumber = elementToNumberList.size();
			}

			matrixOfEdges[firstNumber][secondNumber] = matrixOfEdges[secondNumber][firstNumber] = 1;
			return true;
		}

		/*
		*	Functions which are used to export one type
		*	of saving elements to another.
		*	WARNING! This function deletes unused structure
		//*/
		bool generateListFromMatrix() {
			int listSize = elementToNumberList.size();

			//	Create new list
			listOfEdges.reserve(listSize);

			for (int i = 0; i < listSize; i++)
				for (int j = i; j < listSize; j++)
					if (matrixOfEdges[i][j] == 1) {
						listOfEdges[i].push_back(j);
						listOfEdges[j].push_back(i);
					}


			//	Delete old matrix
			for (int i = 0; i < listSize; i++)
				delete matrixOfEdges[i];
			delete[] matrixOfEdges;
		}
		bool generateMatrixFromList() {
			int listSize = elementToNumberList.size();

			//	Create new matrix
			matrixOfEdges = new short int*[listSize];
			for (int i = 0; i < listSize; i++)
				matrixOfEdges[i] = new short int[listSize];
			for (int i = 0; i < listSize; i++)
				for (int j = 0; j < listSize; j++)
					matrixOfEdges[i][j] = 0;

			int numberOfNodes = 0;
			for (int vectorIterator = 0; vectorIterator < listSize; vectorIterator++) {
				numberOfNodes = listOfEdges[vectorIterator].size();
				for (int nodesIterator = 0; nodesIterator < numberOfNodes; nodesIterator++) {
					matrixOfEdges[vectorIterator][ listOfEdges[vectorIterator][nodesIterator] ] = 1;
					matrixOfEdges[ listOfEdges[vectorIterator][nodesIterator] ][vectorIterator] = 1;
				}
			}

			//	Delete old list
			for (int vectorIterator = 0; vectorIterator < listSize; vectorIterator++)
				listOfEdges[vectorIterator].clear();
			listOfEdges.clear();
		}

		/*
		*	Remakes the whole matrix and adds / subtracts % parameter %
		*	rows & columns
		*	Make take some time to use.
		*	Also changes the value of numberOfEdges variable
		//*/
		bool matrixChangeNumberOfEdges(int numberToChange, int numberOfEdges) {
			if (numberToChange == 0)
				return true;
			if (numberOfEdges + numberToChange == 0) {
				//	Delete a whole matrix
				for (int i = 0; i < numberOfEdges; i++)
					delete matrixOfEdges[i];
				delete[] matrixOfEdges;
			}
			else if (numberToChange < 0)
				if (numberOfEdges - numberToChange < 0)
					return false;
				else {
					short int **newMatrix;
					int newNumberOfElements = numberOfEdges - numberToChange;

					newMatrix = new short int*[newNumberOfElements];
					for (int i = 0; i < newNumberOfElements; i++) {
						newMatrix[i] = new short int[newNumberOfElements];
						for (int j = 0; j < newNumberOfElements; j++)
							newMatrix[i][j] = matrixOfEdges[i][j];
					}
					//	Delete old matrix
					for (int i = 0; i < numberOfEdges; i++)
						delete matrixOfEdges[i];
					delete[] matrixOfEdges;
					matrixOfEdges = newMatrix;
				}
			else {
				short int **newMatrix;
				int newNumberOfElements = numberOfEdges + numberToChange;

				newMatrix = new short int*[newNumberOfElements];
				for (int i = 0; i < newNumberOfElements; i++) {
					newMatrix[i] = new short int[newNumberOfElements];
					for (int j = 0; j < newNumberOfElements; j++)
						if ((j < numberOfEdges) && (i < numberOfEdges))
							newMatrix[i][j] = matrixOfEdges[i][j];
						else
							newMatrix[i][j] = 0;
				}
				//	Delete old matrix
				for (int i = 0; i < numberOfEdges; i++)
					delete matrixOfEdges[i];
				delete[] matrixOfEdges;
				matrixOfEdges = newMatrix;
			}
			return true;
		}

	public:
		Graph() {
			ifWorkWithList = true;
		}

		//	Public functions to work with elements
		bool addElement(TNodeType elementToAdd) {
			if (ifWorkWithList)
				return addElementList(elementToAdd);
			else
				return addElementMatrix(elementToAdd);
		}
		bool eraseElement(TNodeType elementToErase) {
			if (ifWorkWithList)
				return eraseElementList(elementToErase);
			else
				return eraseElementMatrix(elementToErase);
		}
		bool checkForConnectivity() {
			if (ifWorkWithList)
				return checkForConnectivityList();
			else
				return checkForConnectivityMatrix();
		}
		int findMinLength(TNodeType firstNode, TNodeType secondNode) {
			if (ifWorkWithList)
				return findMinLengthList(firstNode, secondNode);
			else
				return findMinLengthMatrix(firstNode, secondNode);
		}
		bool connectElementToAnotherElement(TNodeType firstNode, TNodeType secondNode) {
			if (ifWorkWithList)
				return connectElementToAnotherElementList(firstNode, secondNode);
			else
				return connectElementToAnotherElementMatrix(firstNode, secondNode);
		}

		/*
		*	Randomly generates connections.
		*	Generates up to maxNumberOfConnections connections
		//*/
		bool randomlyConnect(int maxNumberOfConnections) {
			if (maxNumberOfConnections <= 0)
				return false;	//	If user is stupid
			srand(time(NULL));

			int firstNode, secondNode,
				listSize = elementToNumberList.size();
			for (int i = 0; i < maxNumberOfConnections; i++) {
				firstNode = rand() % listSize;
				secondNode = rand() % listSize;
				connectElementToAnotherElement(elementToNumberList[firstNode], elementToNumberList[secondNode]);
			}
			return true;
		}

		//	Back-end stuff will use Matrix
		bool workWithMatrix() {
			if (ifWorkWithList) {
				generateMatrixFromList();
				ifWorkWithList = false;
				return true;
			}
			return false;
		}
		//	Back-end stuff will use List
		bool workWithList() {
			if (!ifWorkWithList) {
				generateListFromMatrix();
				ifWorkWithList = true;
				return true;
			}
			return false;
		}

		bool getInformationString() {
			if (ifWorkWithList) {
				std::cout << "Working in List mode\n";
				int listSize = elementToNumberList.size();
				for (int i = 0; i < listSize; i++) {
					std::cout << i << "\t:\t" << "\n";
				}

				int nodesSize = 0;
				for (int i = 0; i < listSize; i++) {
					nodesSize = listOfEdges[i].size();
					std::cout << i << ": ";
					for (int j = 0; j < nodesSize; j++)
						std::cout << listOfEdges[i][j] << ", ";
					std::cout << "\n";
				}

			}
			else {
				std::cout << "Working in Matrix mode\n";
				int listSize = elementToNumberList.size();
				for (int i = 0; i < listSize; i++) {
					std::cout << i << "\t:\t" << "\n";
				}

				int nodesSize = 0;
				for (int i = 0; i < listSize; i++) {
					std::cout << i << ": ";
					for (int j = 0; j < listSize; j++) {
						if (matrixOfEdges[i][j] == 1)
							std::cout << j << ", ";
						std::cout << "\n";
					}
				}
			}
			return true;
		}

	};
}
