#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>

namespace tvv {
	template <typename TNodeType> class Graph {
	private:
		/*
		*	Variable which is used to tell which
		*	structure we are working with.
		*	true - List (default value)
		*	false - matrix
		//*/
		bool workType;

		/*
		*	listOfEdges will be generated no matter what
		*	The first element in each sub vector is the name of the node
		//*/
		std::vector<std::vector<TNodeType>> listOfEdges;

		/*	matrixOfEdges will be generated only if user wants to
		*	DANGER! Slow zone!
		*	I COULD made a matrix of 1-bit variables, but the lowest possible
		*	variable type, which don't have garbage in leftover bits,
		*	is unsigned int
		//*/
		unsigned int **matrixOfEdges;
		unsigned int numberOfElements;
		//	Number of the element in this vector is it's number in the Matrix
		std::vector<TNodeType> elementToNumberList;

		/*
		*	Returns true if element was added successfully
		*	Returns false if element exist
		//*/
		bool addElementList(TNodeType elementToAdd) {
			unsigned int listSize = listOfEdges.size();
			//	If the list is empty
			if (listSize == 0) {
				listOfEdges.push_back(std::vector<TNodeType>(elementToAdd));
				return true;
			}

			//	Search for the existing node
			for (int vectorIterator = 0; vectorIterator < listSize; vectorIterator++) {
				if (listOfEdges[vectorIterator][0] == elementToAdd)
					return false;
			}
			listOfEdges.push_back(std::vector<TNodeType>(elementToAdd));
			return true;
		}
		bool addElementMatrix(TNodeType elementToAdd) {
			//	If the Matrix is empty
			if (numberOfElements == 0) {
				matrixOfEdges = new int[1];
				matrixOfEdges[0] = new int[1];
				matrixOfEdges[0][0] = 0;
				elementToNumberList.push_back(elementToAdd);
				numberOfElements++;
				return true;
			}

			//	Search for the existing node
			for (int vectorIterator = 0; vectorIterator < numberOfElements; vectorIterator++) {
				if (elementToNumberList[vectorIterator] == elementToAdd)
					return false;
			}

			//	If the element is new
			matrixChangeNumberOfEdges(+1);
			elementToNumberList.push_back(elementToAdd);
			numberOfElements++;
			return true;
		}

		/*
		*	Returns true if element was erased successfully
		*	Returns false if element doesn't exist
		//*/
		bool eraseElementList(TNodeType elementToErase) {
			unsigned int listSize = listOfEdges.size();
			//	If the list is empty
			if (listSize == 0) {
				return false;
			}

			//	Search for the existing node
			for (int vectorIterator = 0; vectorIterator < listSize; vectorIterator++) {
				if (listOfEdges[vectorIterator][0] == elementToErase) {
					int numberOfConnectedNodes = listOfEdges[vectorIterator].size();
					//	Going through all connected 
					
					//	Finding all connected nodes
					for (int summaryIterator = 0; summaryIterator < listSize; summaryIterator++) {
						//	TODO Add checking for exact node to delete
							int numberOfEdges = listOfEdges[summaryIterator].size();
							for (int curNodesIterator = 1; curNodesIterator < numberOfEdges; numberOfEdges++) {
								if ([listOfEdges, numberOfConnectedNodes, summaryIterator]() {
									//	Go through of all connected to elementToErace nodes
									for (int connectsIterator = 1; connectsIterator < numberOfConnectedNodes; connectsIterator++) {
										if (listOfEdges[summaryIterator][curNodesIterator] == listOfEdges[vectorIterator][connectsIterator])
											return true;
										return false;
									}
								}) listOfEdges[summaryIterator].erase(listOfEdges[summaryIterator].begin() + curNodesIterator);
							}
						
					}
					return true;
				}
			}
			
			return false;
		}
		bool eraseElementMatrix(TNodeType elementToErase) {
			//	If the Matrix is empty
			if (numberOfElements == 0)
				return false;

			//	Search for the existing node
			for (int vectorIterator = 0; vectorIterator < numberOfElements; vectorIterator++) {
				if (elementToNumberList[vectorIterator] == elementToErase) {
					// Delete connection between number and selected element
					elementToNumberList.erase(elementToNumberList.begin() + vectorIterator);
					//	Delete %vectorIterator% row and column
					for (int columnIterator = 0; columnIterator < numberOfElements; columnIterator++)
						for (int rowIterator = vectorIterator; rowIterator < numberOfElements - 1; rowIterator++)
							matrixOfEdges[columnIterator][rowIterator] = matrixOfEdges[columnIterator][rowIterator + 1];
					for (int rowIterator = 0; rowIterator < numberOfElements; rowIterator++)
						for (int columnIterator = vectorIterator; columnIterator < numberOfElements - 1; columnIterator++)
							matrixOfEdges[columnIterator][rowIterator] = matrixOfEdges[columnIterator + 1][rowIterator];
					matrixChangeNumberOfEdges(-1);
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
			unsigned int listSize = listOfEdges.size();
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
			int *fathers = new int[numberOfElements];
			int *length = new int[numberOfElements];
			bool *used = new bool[numberOfElements];

			breadthFirstSearchForMatrix(0, fathers, length, used);

			for (int i = 0; i < numberOfElements; i++)
				if (!used)
					return false;
			return true;
		}

		//	Finds a number for specific TNodeType element in list
		int findNumberInList(TNodeType vertex) {
			unsigned int listSize = listOfEdges.size();
			for (int i = 0; i < listSize; i++)
				if (listOfEdges[i][0] == vertex)
					return i;
			return -1;
		}

		/*
		*	Breadth-First Search for different formats.
		*	Makes three different useful arrays
		//*/
		bool breadthFirstSearchForList(int vertex, int fathers[], int length[], bool used[]) {
			std::queue<int> nodes;
			unsigned int listSize = listOfEdges.size();
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

				//	Get list of connected nodes
				typename std::vector<TNodeType>::iterator nodeIterator;

				for (nodeIterator = listOfEdges[vertex].begin() + 1; nodeIterator < listOfEdges[vertex].back(); nodeIterator++) {
					int findNumber = findNumberInList(nodeIterator);
					if (findNumber == -1)
						return false; //	No such element
					if (used[findNumber] == false) {
						used[findNumber] = true;
						nodes.push(findNumber);
						length[findNumber] = length[cur_vertex] + 1;
						fathers[findNumber] = cur_vertex;
					}
				}
			}
			return true;
		}
		bool breadthFirstSearchForMatrix(int vertex, int fathers[], int length[], bool used[]) {
			std::queue<int> nodes;
			for (int i = 0; i < numberOfElements; i++) {
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

				for (int curNode = 0; curNode < numberOfElements; curNode++) {
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
			unsigned int listSize = listOfEdges.size();
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
			int *fathers = new int[numberOfElements];
			int *length = new int[numberOfElements];
			bool *used = new bool[numberOfElements];

			breadthFirstSearchForMatrix(firstNode, fathers, length, used);

			if (length[secondNode] == INT64_MAX)
				return -1;
			else
				return length[secondNode];
		}

		/*
		*	Connects two elements
		*	Returns false if it is the same element
		*	ACHTUNG! YOU MAY CHANGE THIS FUNCTION TO ALLOW LOOPS
		//*/
		bool connectElementToAnotherElementList(TNodeType firstNode, TNodeType secondNode) {
			//	Not allow making loops
			//	Delete this if you want to allow loops
			if (firstNode == secondNode)
				return false;
			//	End block

			unsigned int listSize = listOfEdges.size();
			bool isFirst = false, isSecond = false;
			for (int i = 0; i < listSize; i++) {
				if (listOfEdges[i][0] == firstNode) {
					listOfEdges[i].push_back(secondNode);
					isFirst = true;
				}
				if (listOfEdges[i][0] == secondNode) {
					listOfEdges[i].puch_back(firstNode);
					isSecond = true;
				}
				if (isFirst && isSecond)
					return true;
			}
			//	Element was connected to... non-existing node
			if (!isFirst) {
				addElementList(firstNode);
				listOfEdges[listOfEdges.size()].push_back(firstNode);
				listOfEdges[listOfEdges.size()].push_back(secondNode);
				return true;
			}
			if (!isSecond) {
				addElementList(secondNode);
				listOfEdges[listOfEdges.size()].push_back(secondNode);
				listOfEdges[listOfEdges.size()].push_back(firstNode);
			}
			return true;
		}
		bool connectElementToAnotherElementMatrix(TNodeType firstNode, TNodeType secondNode) {
			int firstNumber = -1, secondNumber = -1;
			for (int i = 0; i < numberOfElements; i++)
				if (elementToNumberList[i] == firstNode) {
					firstNumber = i;
					break;
				}
			for (int i = 0; i < numberOfElements; i++)
				if (elementToNumberList[i] == secondNode) {
					secondNumber = i;
					break;
				}

			if (firstNumber == -1) {
				findElementByNameMatrix(firstNode);
				firstNumber = numberOfElements;
			}
			if (secondNumber == -1) {
				findElementByNameMatrix(secondNode);
				secondNumber = numberOfElements;
			}

			matrixOfEdges[firstNumber][secondNumber] = matrixOfEdges[secondNumber][firstNumber] = 1;
		}

		/*
		*	Returns a pointer to an element
		*	If it has no such element, returns NULL
		//*/
		TNodeType* findElementByNameList(TNodeType);
		TNodeType* findElementByNameMatrix(TNodeType);

		/*
		*	Generates random graph
		//*/
		bool generateRandomlist();
		bool generateRandomMatrix();

		/*
		*	Functions which are used to export one type
		*	of saving elements to another
		//*/
		bool generateListFromMatrix();
		bool generateMatrixFromList();

		/*
		*	Remakes the whole matrix and adds / subtracts % parameter %
		*	rows & columns
		*	Make take some time to use.
		*	Also changes the value of numberOfEdges variable
		//*/
		// TODO Make this unnecessary if the user wants it. May save some time & memory
		bool matrixChangeNumberOfEdges(int numberToChange) {
			if (numberToChange == 0)
				return true;
			if (numberOfElements == numberToChange) {
				//	Delete a whole matrix
				for (int i = 0; i < numberOfElements; i++)
					delete matrixOfEdges[i];
				delete[] matrixOfEdges;

				numberOfElements = 0;
			}
			else if (numberToChange < 0)
				if (numberOfElements - numberToChange < 0)
					return false;
				else {
					unsigned int **newMatrix;
					int newNumberOfElements = numberOfElements - numberToChange;

					newMatrix = new unsigned int*[newNumberOfElements];
					for (int i = 0; i < newNumberOfElements; i++) {
						newMatrix[i] = new unsigned int[newNumberOfElements];
						for (int j = 0; j < newNumberOfElements; j++)
							newMatrix[i][j] = matrixOfEdges[i][j];
					}
					//	Delete old matrix
					for (int i = 0; i < numberOfElements; i++)
						delete matrixOfEdges[i];
					delete[] matrixOfEdges;
					matrixOfEdges = newMatrix;

					numberOfElements = newNumberOfElements;
				}
			else {
				unsigned int **newMatrix;
				int newNumberOfElements = numberOfElements + numberToChange;

				newMatrix = new unsigned int*[newNumberOfElements];
				for (int i = 0; i < newNumberOfElements; i++) {
					newMatrix[i] = new unsigned int[newNumberOfElements];
					for (int j = 0; j < newNumberOfElements; j++)
						if ((j < numberOfElements) && (i < numberOfElements))
							newMatrix[i][j] = matrixOfEdges[i][j];
						else
							newMatrix[i][j] = 0;
				}
				//	Delete old matrix
				for (int i = 0; i < numberOfElements; i++)
					delete matrixOfEdges[i];
				delete[] matrixOfEdges;
				matrixOfEdges = newMatrix;

				numberOfElements = newNumberOfElements;
			}
			return true;
		}

	public:
		Graph() {
			workType = true;
		}

		//	Back-end stuff will use Matrix
		bool workWithMatrix() {
			if (workType) {
				generateMatrixFromList();
				workType = false;
				return true;
			}
			return false;
		}
		//	Back-end stuff will use List
		bool workWithList() {
			if (!workType) {
				generateListFromMatrix();
				workType = true;
				return true;
			}
			return false;
		}

	};
}
