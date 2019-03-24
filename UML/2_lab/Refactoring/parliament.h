#pragma once

#include "law.h"
#include "party.h"
#include <iostream>
#include <vector>

using std::vector;
using std::pair;
using std::cout;

//	Max number of laws
const int MAX_S_LAWS = 16;		//	Max number of standard laws
const int MAX_E_LAWS = 4;		//	Max number of election laws
const int MAX_W_LAWS = 8;		//	Max number of working laws

const int MAX_DEPUTIES = 10;	//	Max number of deputies from each party
const int MAX_PARTIES = 5;		//	Max number of parties

const int MAX_L_DAYS = 16;		//	Max number of days to process one law
const int MAX_E_DAYS = 16;		//	Max number of days between elections
const int WORKING_DAYS = 256;	//	Number of working days
const int PENDING_LAWS = 5;		//	Number of simultaneously processed laws

class Parliament {
private:
	//	Working type variables
	double complexityFactor;		//	A number from [0.1 - 2] which defines days for each law to be processed
	double neededVotesFactor;		//	A number from [0.1 - 0.9] which defines number of votes which are needed

	//	Election type variables
	int electionTerm;			//	Days between each elections of main party
	int singlePartyDeputies;	/*	Number of deputies from main party,
									higher than from another parties */
	int mainPartyDeputies;		//	Number of deputies from non-main party
	int mainPartyID;			//	Main party ID

	//	Parliament data
	vector<Party*> partyList;					//	List of parties
	bool partyAvail[MAX_PARTIES];

	vector<Law*> standardLawList;				//	List of standard laws
	bool lawStAvail[MAX_S_LAWS];

	vector<ElectionsLaw*> electionLawList;		//	List of election laws
	bool electionLawAvail[MAX_E_LAWS];

	vector<WorkingProcessLaw*> workingLawList;	//	List of working laws
	bool workingLawAvail[MAX_W_LAWS];

	//	Generation Functions
	void generateParties(int partiesSize) {
		for (int i = 0; i < partiesSize; i++) {
			Party *party = new Party();
			partyList.push_back(party);

			partyAvail[i] = true;
		}
	}

	void generateLaws(int lawsSize, int electSize, int workSize) {
		srand(clock());

		int randomValue, complexity, daysBetweenElections, newMainPartyDeputies,
			newSinglePartyDeputies, newMainPartyID, newTerm;
		double neededVotes, newComplexityFactor, newNeededVotesFactor;

		//	Generate standard laws
		for (int i = 0; i < lawsSize; i++) {
			complexity = (rand() % MAX_L_DAYS);
			neededVotes = (double)(rand() % 10 + 1) / 10.0;
			Law *newLaw = new Law(complexity, neededVotes);
			standardLawList.push_back(newLaw);
			lawStAvail[i] = true;
		}

		//	Generate elections laws
		for (int i = 0; i < electSize; i++) {
			complexity = (rand() % MAX_L_DAYS);
			neededVotes = (double)(rand() % 10 + 1) / 10.0;

			newTerm = rand() % MAX_E_DAYS;

			//	Maximum number of deputies. From single party is in range [MAX_DEPUTIES / 2, MAX_DEPUTIES]
			newSinglePartyDeputies = (rand() % (MAX_DEPUTIES / 2)) + (MAX_DEPUTIES / 2);
			newMainPartyDeputies = MAX_DEPUTIES;
			newMainPartyID = rand() % MAX_PARTIES;

			ElectionsLaw *newLaw = new ElectionsLaw(complexity, neededVotes, newMainPartyDeputies, newTerm,
				newSinglePartyDeputies, newMainPartyID, partyList.size());
			electionLawList.push_back(newLaw);

			electionLawAvail[i] = true;
		}

		//	Generate working process law
		for (int i = 0; i < workSize; i++) {
			complexity = (rand() % MAX_L_DAYS);
			neededVotes = (double)(rand() % 10 + 1) / 10.0;

			newComplexityFactor = (double)(rand() % 20 + 1) / 10.0;		//	A number from [0.1 - 2] which defines days for each law to be processed
			newNeededVotesFactor = (double)(rand() % 10 + 1) / 10.0;	//	A number from [0.1 - 0.9] which defines number of votes which are needed
			WorkingProcessLaw *newLaw = new WorkingProcessLaw(complexity, neededVotes, newComplexityFactor, newNeededVotesFactor);
			workingLawList.push_back(newLaw);

			workingLawAvail[i] = true;
		}
	}

	void generateDeputies(int deputiesSize) {
		srand(clock());
		for (int i = 0; i < MAX_PARTIES; i++) {
			if (i == mainPartyID)
				partyList[i]->generateDeputies(this->mainPartyDeputies, standardLawList.size(), electionLawList.size(), workingLawList.size());
			else
				partyList[i]->generateDeputies(deputiesSize, standardLawList.size(), electionLawList.size(), workingLawList.size());
		}
	}
public:
	Parliament() {
		complexityFactor = 1.0;		//	Standard value of complexity
		neededVotesFactor = 1.0;	//	Standard number of votes
		electionTerm = 8;			//	Standard number between each election

		//	Maximum number of deputies. From single party is in range [MAX_DEPUTIES / 2, MAX_DEPUTIES]
		singlePartyDeputies = (rand() % (MAX_DEPUTIES / 2)) + (MAX_DEPUTIES / 2);
		mainPartyDeputies = MAX_DEPUTIES;

		mainPartyID = rand() % MAX_DEPUTIES;

		//	Generation functions
		generateParties(MAX_PARTIES);
		generateLaws(MAX_S_LAWS, MAX_E_LAWS, MAX_W_LAWS);
		generateDeputies(singlePartyDeputies);
	}

	void workDay() {
		srand(clock());
		vector< pair<int, int> > pendingStandard;	//	<Law ID, number of processed days>
		vector< pair<int, int> > pendingWorking;	//	<Law ID, number of processed days>

		int pendingLaws = 0;

		int daysSinceLastElection = 0;
		for (int day = 0; day < WORKING_DAYS; day++) {
			//	If it's time to make an election!
			if (daysSinceLastElection == (electionTerm - 1)) {
				electNewParty();
				daysSinceLastElection = 0;
			}

			// If there is less then PENDING_LAWS laws are being processed
			if (pendingLaws < PENDING_LAWS) {
				//	Decide what type of law to process
				int type = rand() % 2;
				if (type == 0) {	//	Standard type
					for (int i = 0; i < MAX_S_LAWS; i++)
						if (lawStAvail[i]) {
							pendingStandard.push_back(pair<int, int>(i, 0));
							pendingLaws++;
							lawStAvail[i] = false;
							break;	//	New law to process is found
						}
				}
				else {				//	Working type
					for (int i = 0; i < MAX_W_LAWS; i++)
						if (workingLawAvail[i]) {
							pendingWorking.push_back(pair<int, int>(i, 0));
							pendingLaws++;
							workingLawAvail[i] = false;
							break;	//	New law to process is found
						}
				}
			}

			//	Check if some of laws have been fully processed
			int pendingSSize = pendingStandard.size();
			int pendingWSize = pendingWorking.size();
			if (pendingSSize != 0)
				for (int i = 0; i < pendingSSize; i++) {
					//	The law is fully processed
					if (pendingStandard[i].second >= (standardLawList[pendingStandard[i].first]->getComplexity() * complexityFactor)) {

						//	OUTPUT
						cout << standardLawList[pendingStandard[i].first]->getIDString() << " law is processed and it has been ";

						//	Check for success
						int totalVotes = 0, totalParties = MAX_PARTIES;
						for (int j = 0; j < MAX_PARTIES; j++)
							if (partyAvail[i]) {
								if (partyList[i]->standardLawVote(pendingStandard[i].first))
									totalVotes++;
							}
							else
								totalParties--;

						if (((double)totalVotes / totalParties) >= standardLawList[pendingStandard[i].first]->getNeededVotes())
							cout << "approved!\n";	//	OUTPUT
						else
							cout << "denied!\n";	//	OUTPUT

						// Delete law from pending list
						pendingStandard.erase(pendingStandard.begin() + i);
						pendingSSize--;
						i--;
					}
				}

			if (pendingWSize != 0)
				for (int i = 0; i < pendingWSize; i++) {
					//	The law is fully processed
					if (pendingWorking[i].second >= (workingLawList[pendingWorking[i].first]->getComplexity() * complexityFactor)) {

						//	OUTPUT
						cout << workingLawList[pendingWorking[i].first]->getIDString() << " law is processed and it has been ";

						//	Check for success
						int totalVotes = 0, totalParties = 5;
						for (int j = 0; j < 5; j++)
							if (partyAvail[i]) {
								if (partyList[i]->standardLawVote(pendingWorking[i].first))
									totalVotes++;
							}
							else
								totalParties--;

						if (((double)totalVotes / totalParties) >= workingLawList[pendingWorking[i].first]->getNeededVotes()) {
							cout << "approved!\n";	//	OUTPUT

							//	Set new factors
							complexityFactor = workingLawList[pendingWorking[i].first]->getNewComplexityFactor();
							neededVotesFactor = workingLawList[pendingWorking[i].first]->getNewNeededVotesFactor();

						}
						else
							cout << "denied!\n";	//	OUTPUT

						// Delete law from pending list
						pendingWorking.erase(pendingWorking.begin() + i);
						pendingWSize--;
						i--;
					}
				}

			//	Add one day to all pending laws
			if (pendingSSize != 0)
				for (int i = 0; i < pendingSSize; i++)
					pendingStandard[i].second++;
			if (pendingWSize != 0)
				for (int i = 0; i < pendingWSize; i++)
					pendingWorking[i].second++;

			//	New day counter
			daysSinceLastElection++;
		}
	}

	//	Picks random election law
	bool electNewParty() {
		int electSize = electionLawList.size();
		srand(clock());

		int numberOfLeftLaws = electSize;
		for (int i = 0; i < electSize; i++)
			if (!electionLawAvail[i])
				numberOfLeftLaws--;

		//	If all of the election law have been processed
		if (numberOfLeftLaws == 0)
			return false;

		//	get first unprocessed elections law
		bool gotLaw = false;
		int electLaw;
		while (!gotLaw) {
			electLaw = rand() % electSize;
			if (electionLawAvail[electLaw])
				gotLaw = true;
		}

		//	Count votes
		int totalVotes = 0;
		int newMainPartyID = electionLawList[electSize]->getMainParty();
		for (int i = 0; i < MAX_PARTIES; i++) {
			totalVotes += partyList[i]->electVotesNumber(electLaw, newMainPartyID, standardLawList.size(),
				electionLawList.size(), workingLawList.size(), partyList[newMainPartyID]);
		}

		int deputiesSize = mainPartyDeputies + singlePartyDeputies * (MAX_DEPUTIES - 1);
		if (((double)totalVotes / deputiesSize) >= electionLawList[electLaw]->getNeededVotes()) {

			mainPartyID = electionLawList[electLaw]->getMainParty();
			electionTerm = electionLawList[electLaw]->getElectionTerm();

			//	New number of deputies from main party
			int newMainPartyDeputies = electionLawList[electLaw]->getMainPartyDeputies();
			if (newMainPartyDeputies > mainPartyDeputies)
				for (int i = 0; i < newMainPartyDeputies - mainPartyDeputies; i++)
					partyList[mainPartyID]->addDeputy(standardLawList.size(), electionLawList.size(), workingLawList.size());
			else if (newMainPartyDeputies < mainPartyDeputies)
				for (int i = 0; i < mainPartyDeputies - newMainPartyDeputies; i++)
					partyList[mainPartyID]->removeDeputy(standardLawList.size(), electionLawList.size(), workingLawList.size());
			mainPartyDeputies = newMainPartyDeputies;

			//	New number of deputies from other parties
			int newSinglePartyDeputies = electionLawList[electLaw]->getSinglePartyDeputies();
			for (int j = 0; j < MAX_PARTIES; j++)
				if (j != mainPartyID) {
					if (newSinglePartyDeputies > singlePartyDeputies)
						for (int i = 0; i < newSinglePartyDeputies - mainPartyDeputies; i++)
							partyList[j]->addDeputy(standardLawList.size(), electionLawList.size(), workingLawList.size());
					else if (newSinglePartyDeputies < singlePartyDeputies)
						for (int i = 0; i < mainPartyDeputies - newSinglePartyDeputies; i++)
							partyList[j]->removeDeputy(standardLawList.size(), electionLawList.size(), workingLawList.size());
				}
			singlePartyDeputies = newSinglePartyDeputies;

			//	Get all supported parties
			for (int i = 0; i < MAX_PARTIES; i++)
				partyAvail[i] = electionLawList[electLaw]->isPartyPreferred(i);
			return true;
		}
		else
			return false;
	}

};