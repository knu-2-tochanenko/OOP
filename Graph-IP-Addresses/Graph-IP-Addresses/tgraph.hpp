#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

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
		vector<vector<TNodeType>> listOfEdges;

		/*	matrixOfEdges will be generated only if user wants to
		*	DANGER! Slow zone!
		*	I COULD made a matrix of 1-bit variables, but the lowest possible
		*	variable type, which don't have garbage in leftover bits,
		*	is unsigned int
		//*/
		unsigned int **matrixOfEdges;
		unsigned int numberOfElements;
		//	Number of the element in this vector is it's number in the Matrix
		vector<TNodeType> elementToNumberList;

		/*
		*	Returns true if element was added successfully
		*	Returns false if element exist
		//*/
		bool addElementList(TNodeType);
		bool addElementMatrix(TNodeType);

		/*
		*	Returns true if element was erased successfully
		*	Returns false if element doen't exist
		//*/
		bool eraseElementList(TNodeType);
		bool eraseElementMatrix(TNodeType);

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
