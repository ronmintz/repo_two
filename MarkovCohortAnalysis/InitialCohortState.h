// InitialCohortState.h

#pragma once
#include "CohortState.h"

// class InitialCohortState is used for cohort zero, which is the initial membership of the community at the start of the simulation
class InitialCohortState :	public CohortState
{
public:
	InitialCohortState();
	InitialCohortState(float initialX, float initialY);
	virtual ~InitialCohortState();

	virtual double startingCohortSize();	// number of individuals placed into cohort (before any deaths)
};

