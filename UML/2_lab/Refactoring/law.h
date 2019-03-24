#pragma once

#include <iostream>
#include <string>
#include <time.h>

using std::string;

//	Standard law
class Law {
private:
	int complexity;				/*	Law complexity. [complexity] + 1 = number of days
									which are needed to process current law */
	double neededVotes;			//	Part of notes which are needed to process current law [0.1 - 1]
	static int MaxID;
	int ID;
public:
	Law(int complexity, double neededVotes) {
		this->complexity = complexity;
		this->neededVotes = neededVotes;

		this->ID = MaxID;
		MaxID++;
	}

	//	Getters
	double getComplexity() { return complexity; }
	double getNeededVotes() { return neededVotes; }
	string getIDString() {
		string res = "";
		if (ID < 10)
			res += "00";
		else if (ID < 100)
			res += "0";
		res += std::to_string(ID);
		return res;
	}
};

int Law::MaxID = 0;

//	Elections law
class ElectionsLaw {
private:
	int complexity;				/*	Law complexity. [complexity] + 1 = number of days
									which are needed to process current law */
	double neededVotes;			//	Part of notes which are needed to process current law [0.1 - 1]

	static int MaxID;
	int ID;
	int electionTerm;			//	Days between each elections of main party
	int singlePartyDeputies;	/*	Number of deputies from main party,
									higher than from another parties */
	int mainPartyDeputies;		//	Number of deputies from non-main party
	int mainPartyID;			//	Main party 
	bool *preferredParty;		//	Preferred parties for this law

public:
	ElectionsLaw(int complexity, double neededVotes, int daysBetweenElections,
		int mainPartyDeputies, int singlePartyDeputies, int mainPartyID,
		int partySize) {

		this->complexity = complexity;
		this->neededVotes = neededVotes;

		this->ID = MaxID;
		MaxID++;

		this->electionTerm = daysBetweenElections;
		this->singlePartyDeputies = singlePartyDeputies;
		this->mainPartyDeputies = mainPartyDeputies;
		this->mainPartyID = mainPartyID;

		preferredParty = new bool[partySize];
		//	randomly support parties
		srand(clock());
		for (int i = 0; i < partySize; i++)
			if (((rand() % 3) != 0) || (mainPartyID == i))
				preferredParty[i] = true;
			else
				preferredParty[i] = false;
	}

	//	Getters
	double getComplexity() { return complexity; }
	double getNeededVotes() { return neededVotes; }
	string getIDString() {
		string res = "";
		if (ID < 10)
			res += "00";
		else if (ID < 100)
			res += "0";
		res += std::to_string(ID);
		return res;
	}

	//	More getters
	int getElectionTerm() { return electionTerm; }
	int getMainPartyDeputies() { return singlePartyDeputies; }
	int getSinglePartyDeputies() { return mainPartyDeputies; }
	int getMainParty() { return mainPartyID; }

	//	Check whether specific party is preferred for this law
	bool isPartyPreferred(int ID) { return (preferredParty[ID]); }
};

int ElectionsLaw::MaxID = 0;

//	Standard law
class WorkingProcessLaw {
private:
	int complexity;				/*	Law complexity. [complexity] + 1 = number of days
									which are needed to process current law */
	double neededVotes;			//	Part of notes which are needed to process current law [0.1 - 1]
	static int MaxID;
	int ID;
	double newComplexityFactor;			//	A number from [0.1 - 2] which defines days for each law to be processed
	double newNeededVotesFactor;		//	A number from [0.1 - 0.9] which defines number of votes which are needed

public:
	WorkingProcessLaw(int complexity, double neededVotes,
		double newComplexityFactor, double newNeededVotesFactor) {

		this->complexity = complexity;
		this->neededVotes = neededVotes;

		this->ID = MaxID;
		MaxID++;

		this->newComplexityFactor = newComplexityFactor;
		this->newNeededVotesFactor = newNeededVotesFactor;
	}

	//	Getters
	double getComplexity() { return complexity; }
	double getNeededVotes() { return neededVotes; }
	string getIDString() {
		string res = "";
		if (ID < 10)
			res += "00";
		else if (ID < 100)
			res += "0";
		res += std::to_string(ID);
		return res;
	}

	//	More getters
	double getNewComplexityFactor() { return newComplexityFactor; }
	double getNewNeededVotesFactor() { return newNeededVotesFactor; }
};

int WorkingProcessLaw::MaxID = 0;
