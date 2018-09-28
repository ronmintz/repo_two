#pragma once
#include "stdafx.h"

const char TAB = '\t';
const char NEWLINE = '\n';
const int  maxCohort = 200;	// cohorts beyond this max will be created because their interaction with other cohorts is needed,
							// but the run will generally terminate before these cohorts are emptied.  The run terminates as soon
							// as cohorts 0 through maxCohort are emptied.
const long long int ticksPerCohort = 365;
const long long int ticksPerYear = 365;
const float initialPopulation = 10000;	// community of 10,000 people (cohort 0) defines initial conditions.
const float initialPositives = initialPopulation * 0.0195795F;	// based on 1.95795% of NYC population
																// being HIV positive.
const float initialNegatives = initialPopulation - initialPositives;
const float positiveBirthsPerYear = 0;
const float negativeBirthsPerYear = 1000;

// cohortSize is the number of individuals born into each cohort except cohort 0.  These births are uniformly
// distributed over each tick in the birth time span of the cohort.  cohortSize is computed by multiplying the
// number of births per tick by the number of ticks in the time interval that the cohort is accepting births.
const float cohortSize = (positiveBirthsPerYear + negativeBirthsPerYear) * ticksPerCohort / ticksPerYear;

const int ageAtMaturity = 13;

// we may later refine these numbers to depend on age of cohort and use
// special death rates for cohort 0, which is the initial distribution
// whose age is unknown.
const double deathRateOfPositivesPerYear = 0.02;
const double deathRateOfNegativesPerYear = 0.0068;

// each cohort willl be deleted when its number of members falls below minimumCohortSize
const float minimumCohortSize = 1.0F;

const double alpha = 0.001 * 70;	// rate of transmission per year to a negative individual if all
									// partners are positive.  Needs to be scaled by proportion of
									// positive individuals in population.  Roughly the product of
									// probability of transmission per sex act with positive partner
									// and number of sex acts per year.
const float ageMax = 500.5;