#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace tvv {
	template <typename TNodeType> class Graph {
	private:
		/*
		*	Wariable which is used to tell which
		*	structure we are working with.
		*	true - List (default value)
		*	false - matrix
		//*/
		bool workType;

		/*
		*	listOfEdges will be generated no matter what
		*	The first element in each subvector is the name of the node
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
			matrixAddSpaceForElements(1);
			elementToNumberList.push_back(elementToAdd);
			numberOfElements++;
			return true;
		}

		/*
		*	Returns true if element was erased successfully
		*	Returns false if element doen't exist
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
				if (elementToNumberList[vectorIterator] == elementToErase)
					return false;
			}

			return true;
		}

		/*
		*	Returns true if all elements are in one connected compnent
		*	false - if not
		//*/
		bool checkForConnectivityList();
		bool checkForConnectivityMatrix();

		/*
		*	Returns the minimal length between two elements
		*	Returns -1 if there is no such way
		//*/
		int findMinLengthList(TNodeType, TNodeType);
		int findMinLengthMatrix(TNodeType, TNodeType);

		/*
		*	Connects two elements
		*	Returns false if it is the same element
		*	ACHTUNG! YOU MAY CHANGE THIS FUNCTION TO ALLOW LOOPS
		//*/
		bool connectElementToAnotherElementList(TNodeType, TNodeType);
		bool connectElementToAnotherElementMatrix(TNodeType, TNodeType);

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

		
		bool generateMatrixOfedges();

		/*
		*	Remakes the whole matrix and add %parameter%
		*	more rows & columns
		*	Make take some time to use
		//*/
		// TODO Make this inccessary if the user wants it. May save some time
		bool matrixAddSpaceForElements();

		

	public:
		Graph();

		//	Back-end stuff will use Matrix
		bool workWithMatrix();
		//	Back-end stuff will use List
		bool workWithList();

	};
}
