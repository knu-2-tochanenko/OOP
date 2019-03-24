#pragma once

#include <iostream>
#include <ctime>

class Deputy {
private:
	int id;
	static int MaxID;

	bool *lawCoef;		// true - law is preferred, if else - is not
	bool *electCoef;	// true - law is preferred, if else - is not
	bool *workCoef;		// true - law is preferred, if else - is not

	const int SUPPORT_COEFF = 0.33;	//	Minimal coefficient to support party
public:
	Deputy(int lawsSize, int electSize, int workSize) {
		this->id = MaxID;
		MaxID++;

		srand(clock());
		lawCoef = new bool[lawsSize];
		electCoef = new bool[electSize];
		workCoef = new bool[workSize];

		//	Randomly generate preferences
		for (int i = 0; i < lawsSize; i++)
			lawCoef[i] = rand() % 2;
		for (int i = 0; i < electSize; i++)
			electCoef[i] = rand() % 2;
		for (int i = 0; i < workSize; i++)
			workCoef[i] = rand() % 2;
	}

	/*
	*	Checks if current Deputy wants to vote for law, which is
	*	elections type. His decision is made based of the same
	*	number of supported laws.
	*/
	bool voteForParty(int lawID, int mainPartyID, int lawsSize, bool* lawsList, int electSize, bool *electList, int workSize, bool *workList) {
		double totalNumberOfSameLaws = 0.0;

		for (int i = 0; i < lawsSize; i++)
			if ((lawCoef[i])
				&& (lawsList[i]))
				totalNumberOfSameLaws += 1.0;
		for (int i = 0; i < electSize; i++)
			if ((lawCoef[i])
				&& (electList[i]))
				totalNumberOfSameLaws += 1.0;
		for (int i = 0; i < workSize; i++)
			if ((lawCoef[i])
				&& (workList[i]))
				totalNumberOfSameLaws += 1.0;

		return (((double)lawsSize / totalNumberOfSameLaws) >= SUPPORT_COEFF);
	}

	//	Kind of getters
	bool standardLawVote(int lawID) { return lawCoef[lawID]; }
	bool electLawVote(int lawID) { return electCoef[lawID]; }
	bool workLawVote(int lawID) { return workCoef[lawID]; }
};

int Deputy::MaxID = 0;
