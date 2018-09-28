// InitialCohortState.cpp

#include "stdafx.h"
#include "InitialCohortState.h"

// class InitialCohortState is used for cohort zero, which is the initial membership of the community at the start of the simulation

InitialCohortState::InitialCohortState() : CohortState()
{
}

InitialCohortState::InitialCohortState(float initialX, float initialY) : CohortState(initialX, initialY)
{
}

InitialCohortState::~InitialCohortState()
{
}

double InitialCohortState::startingCohortSize()	// number of individuals placed into cohort (before any deaths)
{
	return initialPopulation;
}
