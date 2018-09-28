// CohortState.cpp

#include "stdafx.h"
#include "CohortState.h"
using namespace std;

CohortState::CohortState() : ssCohortOutFile()	// cohort is initially empty.  Members will be added at each tick.
{												// Default constructor of stringstream is called for ssCohortOutFile.
	x = 0;
	y = 0;
	sumOfLifetimes = 0;
	ssCohortOutFile << "year" << TAB << "cohort age" << TAB << "numLiving" << TAB << "numPositives" << TAB << "percentLiving" << TAB << "percentPositive" << NEWLINE;
	ssCohortOutFile << "----" << TAB << "----------" << TAB << "---------" << TAB << "------------" << TAB << "----------------" << TAB << "------------------" << NEWLINE;
}

CohortState::CohortState(float initialX, float initialY) : ssCohortOutFile()
{														// Default constructor of stringstream is called for ssCohortOutFile.
	x = initialX;
	y = initialY;
	sumOfLifetimes = 0;
	ssCohortOutFile << "year" << TAB << "cohort age" << TAB << "numLiving" << TAB << "numPositives" << TAB << "percentLiving" << TAB << "percentPositive" << NEWLINE;
	ssCohortOutFile << "----" << TAB << "----------" << TAB << "---------" << TAB << "------------" << TAB << "----------------" << TAB << "------------------" << NEWLINE;
}

double CohortState::startingCohortSize()	// number of individuals to be placed into cohort (before any deaths)
{
	return cohortSize;
}

double CohortState::numLiving()		// number of individuals in cohort who are currently alive
{
	return x + y;
}

double CohortState::numPositives()	// number of positive individuals in cohort
{
	return x;
}


double CohortState::numNegatives()	// number of negative individuals in cohort
{
	return y;
}

double CohortState::proportionLiving()		// proportion of cohort members who are currently alive
{
	return numLiving() / startingCohortSize();
}

double CohortState::proportionPositive()	// proportion of positive individuals within living members of cohort
{
	if (x + y >= minimumCohortSize)
		return x / (x + y);
	else
		return -1;	// indicates all members of cohort are dead
}

void CohortState::processBirthsPerTick(float positivesPerTick, float negativesPerTick)
{
	x += positivesPerTick;
	y += negativesPerTick;
}

// returns number of surviving members of cohort after the deaths.
float CohortState::processDeathsPerTick(double deathRateOfPositivesPerTick, double deathRateOfNegativesPerTick)
{
	x = static_cast<float>(x - deathRateOfPositivesPerTick * x);
	y = static_cast<float>(y - deathRateOfNegativesPerTick * y);
	return x + y;
}

void CohortState::processNegativeToPositiveTransitionsPerTick(double transitionRatePerTick)
{
	float numTransitions = static_cast<float>(transitionRatePerTick * y);
	x += numTransitions;	// positives
	y -= numTransitions;	// negatives
}

	// accumulate sum of number of living cohort members at each tick for duration of cohort.
	// This excludes the time before maturity.
void CohortState::updateSumOfLifetimesPerTick()
{
	sumOfLifetimes += numLiving();
}

double CohortState::averageLifetime()	// in years, including the time before maturity
{
	// must divide sumOfLifetimes (in ticks) by number of ticks in a year
	// then divide by number of individuals born into cohort.
	return sumOfLifetimes / (ticksPerYear * startingCohortSize()) + ageAtMaturity;
}

void CohortState::storeCohortOutFile(string filename)
{
	ofstream outFile(filename.c_str());
	ssCohortOutFile << NEWLINE << "Average Lifetime for Cohort = " << averageLifetime() << NEWLINE;
	outFile << ssCohortOutFile.str();
	outFile.close();
}
