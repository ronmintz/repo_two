// CohortState.h

#pragma once
#include "stdafx.h"
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ios>
#include "constants.h"
using namespace std;

class CohortState;
// class SystemState;

typedef vector<CohortState *>::size_type CohortNumber;


class CohortState
{
private:
	float x;	// number of positive individuals in cohort
	float y;	// number of negative individuals in cohort
	double sumOfLifetimes;	// sum of lifetime (in ticks) of each cohort member up to current tick, excluding time
							// before maturity (also known as Cumulative Utility).

public:
	stringstream ssCohortOutFile;	// accumulates output text file for this cohort

	CohortState();	// cohort is initially empty.  Members will be aded at each tick.
	CohortState(float initialX, float initialY);

	virtual ~CohortState()
	{
	}

	virtual double startingCohortSize();	// number of individuals to be placed into cohort (before any deaths)

	double numLiving();		// number of individuals in cohort who are currently alive
	double numPositives();	// number of positive individuals in cohort
	double numNegatives();	// number of negative individuals in cohort
	double proportionLiving();		// proportion of cohort members who are currently alive
	double proportionPositive();	// proportion of positive individuals within living members of cohort

	void processBirthsPerTick(float positivesPerTick, float negativesPerTick);

	// returns number of surviving members of cohort after the deaths.
	float processDeathsPerTick(double deathRateOfPositivesPerTick, double deathRateOfNegativesPerTick);

	void processNegativeToPositiveTransitionsPerTick(double transitionRatePerTick);
	void updateSumOfLifetimesPerTick();
	double averageLifetime();	// in years
	void storeCohortOutFile(string filename);

//	friend void SystemState::updateCohortOutputs(CohortNumber cnum);
};
