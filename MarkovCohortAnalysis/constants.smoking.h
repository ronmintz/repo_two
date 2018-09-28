// constants.smoking.h

#pragma once
#include "stdafx.h"

const char TAB = '\t';
const char NEWLINE = '\n';
const int  maxCohort = 200;	// cohorts beyond this max will be created because their interaction with other cohorts is needed,
							// but the run will generally terminate before these cohorts are emptied.  The run terminates as soon
							// as cohorts 0 through maxCohort are emptied.
const long long int ticksPerCohort = 365;
const long long int ticksPerYear = 365;

// In the initial population of 10,000 individuals, assume that 80% are smokers.
// Assume that 1000 individuals reach maturity of 13 each year and are still non-smokers at this age.
// Assume that the death rate per year of non-smokers is  0.0068 and that the death rate of smokers is 50% higher.
// If entire population were smokers, assume that the probability of an individual remaining a non-smoker for a
// lifetime of 90 years after maturity is 50% because of the new discovery of the dangers of smoking.  Thus
// (1 - alpha) ^ 90 = 0.5 so that alpha = 1 - 0.5 ^ (1/90) = 0.007672



const float initialPopulation = 10000;	// community of 10,000 people (cohort 0) defines initial conditions.
const float initialPositives = initialPopulation * 0.80F;	// In initial community (cohort 0), 80% of population smokes.
const float initialNegatives = initialPopulation - initialPositives;

const float positiveBirthsPerYear = 0;
const float negativeBirthsPerYear = 1000;	// all individuals at age of maturity are initially non-smokers

// cohortSize is the number of individuals born into each cohort except cohort 0.  These births are uniformly
// distributed over each tick in the birth time span of the cohort.  cohortSize is computed by multiplying the
// number of births per tick by the number of ticks in the time interval that the cohort is accepting births.
const float cohortSize = (positiveBirthsPerYear + negativeBirthsPerYear) * ticksPerCohort / ticksPerYear;

const int ageAtMaturity = 13;

// we may later refine these numbers to depend on age of cohort and use
// special death rates for cohort 0, which is the initial distribution
// whose age is unknown.
const double deathRateOfNegativesPerYear = 0.0068;
	// the death rate for smokers is 50% higher than non-smokers
const double deathRateOfPositivesPerYear = 1.5 * deathRateOfNegativesPerYear;

// each cohort willl be deleted when its number of members falls below minimumCohortSize
const float minimumCohortSize = 1.0F;

const double alpha = 0.007672;	// probability per year that a non-smoker will start smoking if
								// entire population smokes = 1 - 0.5 ^ (1/90).  Needs to be scaled
								// by proportion of smokers in population.
const float ageMax = 500.5;