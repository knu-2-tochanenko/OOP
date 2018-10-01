#pragma once

#include <iostream>
#include <vector>
#include <string>

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

		/*
		*	Functions which are used to export one type
		*	of saving elements to another
		//*/
		bool generateListOfEdges();
		bool generateMatrixOfedges();

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
				delete matrixOfEdges[];

				numberOfElements = 0;
			}
			else if (numberToChange < 0)
				if (numberOfElements - numberToChange < 0)
					return false;
				else {
					unsigned int **newMatrix;
					int newNumberOfElements = numberOfElements - numberToChange;

					newMatrix = new *unsigned int[newNumberOfElements];
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

				newMatrix = new *unsigned int[newNumberOfElements];
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
				generateMatrixOfedges();
				workType = false;
				return true;
			}
			return false;
		}
		//	Back-end stuff will use List
		bool workWithList() {
			if (!workType) {
				generateListOfEdges();
				workType = true;
				return true;
			}
			return false;
		}

	};
}
