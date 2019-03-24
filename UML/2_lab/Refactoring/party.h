#pragma once

#include "deputy.h"
#include <iostream>
#include <vector>

using std::vector;

class Party {
private:
	int ID;
	static int MaxID;

	vector<Deputy*> deputiesList;	//	List of deputies
	bool *lawCoef;					//	true - law is preferred, if else - is not
	bool *electCoef;				//	true - law is preferred, if else - is not
	bool *workCoef;					//	true - law is preferred, if else - is not

public:
	Party() {
		this->ID = MaxID;
		MaxID++;
	}

	//	The second stage of initialization
	void generateCoefficients(int lawsSize, int electSize, int workSize) {
		srand(clock());

		lawCoef = new bool[lawsSize];
		electCoef = new bool[electSize];
		workCoef = new bool[workSize];

		for (int i = 0; i < lawsSize; i++)
			lawCoef[i] = rand() % 2;
		for (int i = 0; i < electSize; i++)
			electCoef[i] = rand() % 2;
		for (int i = 0; i < workSize; i++)
			workCoef[i] = rand() % 2;
	}

	//	The third stage of initialization
	void generateDeputies(int deputiesNumber, int lawsSize, int electSize, int workSize) {
		for (int i = 0; i < deputiesNumber; i++) {
			Deputy *newDeputy = new Deputy(lawsSize, electSize, workSize);
			deputiesList.push_back(newDeputy);
		}
	}

	//	Get how many deputies have voted for election
	int electVotesNumber(int lawID, int mainPartyID, int lawsSize, int electSize, int workSize, Party *votedParty) {
		int res = 0;

		//	Get preferred laws for voted party
		bool* lawsList;		lawsList = new bool[lawsSize];
		bool* electList;	electList = new bool[electSize];
		bool* workList;		workList = new bool[workSize];
		for (int i = 0; i < lawsSize; i++)
			lawsList[i] = votedParty->standardLawVote(i);
		for (int i = 0; i < electSize; i++)
			electList[i] = votedParty->electLawVote(i);
		for (int i = 0; i < workSize; i++)
			workList[i] = votedParty->workLawVote(i);

		int deputiesSize = deputiesList.size();
		for (int i = 0; i < deputiesSize; i++)
			if (deputiesList[i]->voteForParty(lawID, mainPartyID, lawsSize, lawsList, electSize, electList, workSize, workList));
		res++;
		return res;
	}

	//	Adds another randomly generated deputy
	bool addDeputy(int lawsSize, int electSize, int workSize) {
		Deputy *newDeputy = new Deputy(lawsSize, electSize, workSize);
		deputiesList.push_back(newDeputy);
		return true;
	}

	//	Removes least favorite deputy
	bool removeDeputy(int lawsSize, int electSize, int workSize) {
		//	If deputies list is empty
		if (deputiesList.size() == 0)
			return false;

		int deputySize = deputiesList.size();
		int *similarCoef;
		similarCoef = new int[deputySize];
		for (int i = 0; i < deputySize; i++)
			similarCoef[i] = 0;

		//	Check for every deputy for similar votes
		for (int i = 0; i < deputySize; i++) {
			for (int j = 0; j < lawsSize; j++)
				if (deputiesList[i]->standardLawVote(j) && lawCoef[j])
					similarCoef[i]++;
			for (int j = 0; j < electSize; j++)
				if (deputiesList[i]->electLawVote(j) && electCoef[j])
					similarCoef[i]++;
			for (int j = 0; j < workSize; j++)
				if (deputiesList[i]->workLawVote(j) && workCoef[j])
					similarCoef[i]++;
		}

		//	Find the least suitable deputy
		int lowestNum = 0;
		for (int i = 1; i < deputySize; i++)
			if (similarCoef[lowestNum] > similarCoef[i])
				lowestNum = i;

		//	Delete it!
		deputiesList.erase(deputiesList.begin() + lowestNum);
		return true;
	}

	//	Getters
	int getID() { return ID; }

	//	Kind of getters
	bool standardLawVote(int lawID) { return lawCoef[lawID]; }
	bool electLawVote(int lawID) { return electCoef[lawID]; }
	bool workLawVote(int lawID) { return workCoef[lawID]; }

	bool* getLawCoef() { return lawCoef; }
	bool* getElectCoef() { return electCoef; }
	bool* getWorkCoef() { return workCoef; }

};

int Party::MaxID = 0;
