#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace tvv {
	template <typename TNodeType> class Graph {
	private:

		// listOfEdges will be generated no matter what
		vector<vector<TNodeType>> listOfEdges;

		/*	matrixOfEdges will be generated only if user wants to
		*	DANGER! Slow zone!
		*	I COULD made a matrix of 1-bit variables, but the lowest possible
		*	variable type, which don't have garbage in leftover bits,
		*	is unsigned int
		//*/
		usigned int **matrixOfEdges;

		/*--- Methods for listOfEdges ---*/
		bool addElementList(TNodeType);
		bool eraseElementList(TNodeType);

		bool checkForConnectivity();
		bool findMinLengthList(TNodeType, TNodeType);

		/*--- Methods for matrixOfEdges ---*/
		bool generateMatrixOfedges();

		bool addElementMatrix(TNodeType);
		bool eraseElementMatrix(TNodeType);

		bool checkForConnectivityMatrix();
		bool findMinLengthMatrix(TNodeType, TNodeType);

	public:
		Graph() {

		}

		bool addElement(TNodeType);
		bool eraseElement(TNodeType);

		bool checkForConnectivity();
		bool findMinLength(TNodeType, TNodeType);

		/*	Function which lets user decide if it is necessary to work on matrix
			True	- program will use matrixOfEdges
			False	- program will use listOfEdges
		//*/
		bool workWithMatrix(bool);

	};
}
