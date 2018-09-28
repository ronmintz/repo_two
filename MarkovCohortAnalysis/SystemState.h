// SystemState.h

#pragma once
#include "stdafx.h"
#include <string>
#include "constants.h"
#include "CohortState.h"
#include "InitialCohortState.h"
using namespace std;

class SystemState
{
private:
	long long int tick;		// current time tick
	vector<CohortState *> cohorts;	// vector containing the number of positive & negative individuals in each cohort

	// string for each row of ProportionLiving and ProportionPositive tables.  Each row of these tables represents a cohort.
	// Each column of these tables represents an age, beginning with the age of maturity.
	// The ProportionLiving table specifies the percentage of each cohort who is still alive at each age.
	// The ProportionPositive table specifies the percentage of the living members of each cohort who are positive at each age.

	vector<stringstream *> ssProportionLivingTableRow;	 // elements of these vectors are indexed by CohortNumber like cohorts[cnum]
	vector<stringstream *> ssProportionPositiveTableRow; // But these stringstreams cannot be part of CohortState because they must be retained
														 // for file output at the end of program after cohorts[cnum] has been deleted.
	stringstream ssSystemOutFile;	// accumulates output text file for the entire system

	vector<double> averageLifetime;	// averageLifetime[cnum] is set to the average lifetime of cohort cnum when this cohort becomes empty.

	CohortNumber earliestLivingCohort;
	CohortNumber latestCohort;


	void addNewCohort();	// add new cohort with zero members. New members will be added at each tick.
	void processBirths();
	void processDeaths();	// record deaths in each cohort for current tick

	// record transitions from positive to negative over all cohorts
	void processNegativeToPositiveTransitions(double currentTransitionRatePerYear);

	bool executeAllTransitionsForOneTick();	// executes all transitions for current tick
	void updateSumOfLifetimesInEachCohort();

	void updateCohortOutputs(CohortNumber cnum);
	void updateSystemOutput();
	void updateOutputsForYear();	// update the output file for entire system and for each cohort with this year's data

public:
	SystemState();
	~SystemState();
	int currentYear();
	double numLiving();		// total number of living individuals in all cohorts
	double numPositives();	// total number of positive individuals in all cohorts
	double numNegatives();	// total number of negative individuals in all cohorts

	double proportionPositive(); // proportion of positive individuals over all cohorts
	double transitionRatePerYear();     // rate of transition of a negative individual to positive

	int cohortAge(CohortNumber cnum);	// age of specified cohort as integral number of years
		// Cohort 0 is the initial membership of the community at the start of the simulation,
		// which may be composed of individuals of varying ages.  But for informational
		// purposes, the birth time of cohort 0 is assumed to be tick 0.

	bool processAllCohorts(CohortNumber maxCohort);
	void storeSystemOutFile(char *filename);
	void storeOutputTable(char *filename, const vector<stringstream *>& ssTableRow);

	/*************************************************************************************************************************
	void test()	// for testing of virtual function startingCohortSize() called from proportionLiving()	
	{
		cout << "\ncohort 0: startingCohortSize(via proportionLiving()) returned " << (cohorts[0]->numLiving()) / (cohorts[0]->proportionLiving());
		cout << "\ncohort 1: startingCohortSize(via proportionLiving()) returned " << (cohorts[1]->numLiving()) / (cohorts[1]->proportionLiving());
		cout << "\ncohort 2: startingCohortSize(via proportionLiving()) returned " << (cohorts[2]->numLiving()) / (cohorts[2]->proportionLiving());

		cout << "\ncohort 0: startingCohortSize(direct) returned " << cohorts[0]->startingCohortSize();
		cout << "\ncohort 1: startingCohortSize(direct) returned " << cohorts[1]->startingCohortSize();
		cout << "\ncohort 2: startingCohortSize(direct) returned " << cohorts[2]->startingCohortSize();
	}
	*************************************************************************************************************************/
};
