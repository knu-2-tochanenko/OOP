#include "tgraph.hpp"

template<typename TNodeType>
inline bool tvv::Graph<TNodeType>::addElementList(TNodeType elementToAdd) {
	
	unsigned int listSize = listOfEdges.size();
	//	If the list is empty
	if (listSize == 0) {
		listOfEdges.push_back(vector<TNodeType>(elementToAdd));
		return true;
	}

	//	Search for the existing node
	for (int vectorIterator = 0; vectorIterator < listSize; vectorIterator++) {
		if (listOfEdges[vectorIterator][0] == elementToAdd)
			return false;
	}
	listOfEdges.push_back(vector<TNodeType>(elementToAdd));
	return true;
}

template<typename TNodeType>
bool tvv::Graph<TNodeType>::addElementMatrix(TNodeType elementToAdd) {
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
