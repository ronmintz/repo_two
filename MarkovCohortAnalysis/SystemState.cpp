// SystemState.cpp

#include "stdafx.h"
#include "SystemState.h"
using namespace std;

	// start at tick 0 with one cohort containing initial population
	// Default constructor of stringstream is called for ssSystemOutFile.
SystemState::SystemState() : tick(0), earliestLivingCohort(0), latestCohort(0),
							 cohorts(1, new InitialCohortState(initialPositives, initialNegatives)),
							 ssProportionLivingTableRow(1, new stringstream),
							 ssProportionPositiveTableRow(1, new stringstream),
							 ssSystemOutFile()
{
	ssSystemOutFile << "year" << TAB << "numLiving" << TAB << "numPositives" << TAB << "percentPositive" << NEWLINE;
	ssSystemOutFile << "----" << TAB << "---------" << TAB << "------------" << TAB << "---------------" << NEWLINE;
}

SystemState::~SystemState()
{
	for (CohortNumber cnum = earliestLivingCohort; cnum <= latestCohort; ++cnum)
	{
		if (cohorts[cnum] != 0)
		{
			delete cohorts[cnum];
		}
	}

	cout << "At end of run, deleted cohorts " << earliestLivingCohort << " through " << latestCohort << " (unless already deleted)\n";
}

int SystemState::currentYear()
{
	return static_cast<int>(tick / ticksPerYear);
}

double SystemState::numLiving()		// total number of living individuals in all cohorts
{
	CohortNumber i;
	double sum = 0.0;

	for (i = earliestLivingCohort; i <= latestCohort; ++i)
	{
		if (cohorts[i] != 0)
			sum += cohorts[i]->numLiving();
	}
	return sum;
}

double SystemState::numPositives()	// total number of positive individuals in all cohorts
{
	CohortNumber i;
	double sum = 0.0;

	for (i = earliestLivingCohort; i <= latestCohort; ++i)
	{
		if (cohorts[i] != 0)
			sum += cohorts[i]->numPositives();
	}
	return sum;
}

double SystemState::numNegatives()	// total number of negative individuals in all cohorts
{
	CohortNumber i;
	double sum = 0.0;

	for (i = earliestLivingCohort; i <= latestCohort; ++i)
	{
		if (cohorts[i] != 0)
			sum += cohorts[i]->numNegatives();
	}

	return sum;
}

double SystemState::proportionPositive()	// proportion of positive individuals over all cohorts
{
	double x = numPositives();
	double totalLiving = numLiving();

	if (totalLiving >= minimumCohortSize)
		return x / totalLiving;
	else
		return -1;	// indicates all cohorts are empty
}

double SystemState::transitionRatePerYear()     // rate of transition of a negative individual to positive
{
	if (proportionPositive() >= 0.0)
		return alpha * proportionPositive();
	else
		return -1;	// indicates all cohorts are empty
}

int SystemState::cohortAge(CohortNumber cnum)	// age of specified cohort as integral number of years
{
	// tickOfEntranceToSystem = average value of tick when each member of this cohort
	// entered the system, which happened at this member's age of maturity.
	// Average tick of entrance to system of members of cohort 1 is (0.5 * ticksPerCohort)
	// since members'are assumed to uniformly enter the system over the entrance period of cohort.
	// Cohort 0 is the initial condition of the community at the start of the simulation,
	// which may be composed of individuals of varying ages.  But for informational
	// purposes, cohort 0 is assumed to enter the system at tick 0.

	long long int tickOfEntranceToSystem = 0;	// for cohort 0

	if (cnum > 0)
		tickOfEntranceToSystem = static_cast<long long int>((cnum - 0.5) * ticksPerCohort);

		// return the number of years that this cohort has been in the system plus age at maturity,
		// since cohort members are assumed to enter system at maturity.
	return ageAtMaturity + static_cast<int>((tick - tickOfEntranceToSystem) / ticksPerYear);
}

void SystemState::addNewCohort()
{
	cohorts.push_back(new CohortState); // add new cohort with zero members. New members will be added at each tick.
	ssProportionLivingTableRow.push_back(new stringstream);
	ssProportionPositiveTableRow.push_back(new stringstream);
	averageLifetime.push_back(0.0);
	++latestCohort;
	cout << "added cohort " << latestCohort << " (latest cohort)\n";
}

void SystemState::processBirths()	// record births in latestCohort for current tick
{
	cohorts[latestCohort]->processBirthsPerTick(positiveBirthsPerYear / ticksPerYear, negativeBirthsPerYear / ticksPerYear);
}

void SystemState::processDeaths()	// record deaths in each cohort for current tick
{
	for (CohortNumber cnum = earliestLivingCohort; cnum <= latestCohort; ++cnum)
	{
		if (cohorts[cnum] != 0)
		{
			// In the present implementation, deathRateOfPositivesPerYear and deathRateOfNegativesPerYear are constants defined in constants.h.
			// But we may later extend the code to allow these values to depend on cohortAge(cnum), except for cohort 0.

			float numSurvivors = cohorts[cnum]->processDeathsPerTick(deathRateOfPositivesPerYear / ticksPerYear, deathRateOfNegativesPerYear / ticksPerYear);

			// If the number of surviving members of cohort has fallen below minimumCohortSize, cohort is presumed to be empty.
			// So remove this cohort unless it is the latest cohort into which new members are still being added.
			// Before the cohort is removed, its outputs are stored to a disk file.
			if (numSurvivors < minimumCohortSize  &&  cnum != latestCohort)
			{
				cohorts[cnum]->storeCohortOutFile("C:/Users/ronmintz/Visual C++ programs/MarkovCohortAnalysis/outputSmoking/OutputFromCohort" + to_string(cnum));
				*(ssProportionLivingTableRow[cnum])   << NEWLINE;
				*(ssProportionPositiveTableRow[cnum]) << NEWLINE;
				averageLifetime[cnum] = cohorts[cnum]->averageLifetime();

				delete cohorts[cnum];
				cohorts[cnum] = 0;
				cout << "deleted dead cohort " << cnum << '\n';

				while (cohorts[earliestLivingCohort] == 0)
				{
					if (earliestLivingCohort >= latestCohort)
						exit(EXIT_FAILURE);	// no cohorts left in system, impossible to continue run
					else
					{
						++earliestLivingCohort;
						cout << "earliest living cohort " << earliestLivingCohort << '\n';
					}
				}
			}
		}
	}
}

void SystemState::processNegativeToPositiveTransitions(double currentTransitionRatePerYear) // record transitions from positive to negative over all cohorts
{
	for (CohortNumber cnum = earliestLivingCohort; cnum <= latestCohort; ++cnum)
	{
		if (cohorts[cnum] != 0)
		{
			cohorts[cnum]->processNegativeToPositiveTransitionsPerTick(currentTransitionRatePerYear / ticksPerYear);
		}
	}
}

bool SystemState::executeAllTransitionsForOneTick()	// executes all transitions at current tick
{
	processDeaths();
	processBirths();	// done after processDeaths() to prevent newborns from dying

	double currentTransitionRatePerYear = transitionRatePerYear();

	if (currentTransitionRatePerYear >= 0.0)
	{
		processNegativeToPositiveTransitions(currentTransitionRatePerYear);	// apply same negative to positive transition rate to all non-empty cohorts
		return true;	// successfully processed transitions for each cohort
	}
	else
	{
		return false; // all cohorts are empty
	}
}

void SystemState::updateSumOfLifetimesInEachCohort()
{
	for (CohortNumber cnum = earliestLivingCohort; cnum <= latestCohort; ++cnum)
	{
		if (cohorts[cnum] != 0)
		{
			cohorts[cnum]->updateSumOfLifetimesPerTick();
		}
	}
}

void SystemState::updateCohortOutputs(CohortNumber cnum)
{
	CohortState *cState = cohorts[cnum];
	cState->ssCohortOutFile << currentYear() << TAB << cohortAge(cnum) << TAB << cState->numLiving() << TAB << cState->numPositives() << TAB << 100.0 * cState->proportionLiving() << " %" << TAB << 100.0 * cState->proportionPositive() << " %" << NEWLINE;
	*(ssProportionLivingTableRow[cnum])   << TAB << 100.0 * cState->proportionLiving()   << " %";
	*(ssProportionPositiveTableRow[cnum]) << TAB << 100.0 * cState->proportionPositive() << " %";
}


void SystemState::updateSystemOutput()
{
	ssSystemOutFile << currentYear() << TAB << numLiving() << TAB << numPositives() << TAB << 100.0 * proportionPositive() << " %" << NEWLINE;
}

	// Update the output file for entire system and for each cohort with this year's data.
	// There is no need to store output data into cohorts beyond maxCohort because these
	// cohorts will generally not be emptied before the end of run.  So their output data
	// will be incomplete at the end of run and is therefore not of interest.  However, the
	// state of these cohorts may interact with the cohorts which will be output
	// (intercohort diffusion).  Cohorts beyond maxCohort are included in the system statistics
	// recorded in updateSystemOutput().
void SystemState::updateOutputsForYear()
{
	for (CohortNumber cnum = earliestLivingCohort; cnum <= latestCohort && cnum <= maxCohort; ++cnum)
	{
		if (cohorts[cnum] != 0)
		{
			updateCohortOutputs(cnum);
		}
	}

	updateSystemOutput();
}

bool SystemState::processAllCohorts(CohortNumber maxCohort)
{
	while (earliestLivingCohort <= maxCohort)
	{
		if (tick % ticksPerYear == 0)	// update output strings with data for year that has just ended
		{
			updateOutputsForYear();
		}

		if (tick % ticksPerCohort == 0)
		{
			cout << "\ntick " << tick << '\n';
			addNewCohort();	// add empty cohort, to be filled gradually over period of cohort
		}

		executeAllTransitionsForOneTick();
		updateSumOfLifetimesInEachCohort();
		++tick;

/*****************************************************
		if (tick == 5 * ticksPerCohort)
		{
			test(); // for testing of virtual functions
			cin.get();
		}
*******************************************************/
	}

	storeSystemOutFile("C:/Users/ronmintz/Visual C++ programs/MarkovCohortAnalysis/outputSmoking/TotalSystemOutput");
	storeOutputTable("C:/Users/ronmintz/Visual C++ programs/MarkovCohortAnalysis/outputSmoking/ProportionLivingTable", ssProportionLivingTableRow);
	storeOutputTable("C:/Users/ronmintz/Visual C++ programs/MarkovCohortAnalysis/outputSmoking/ProportionPositiveTable", ssProportionPositiveTableRow);

	cout << "all cohorts processed successfully\n";
	return true;	// successful run
}

void SystemState::storeSystemOutFile(char *filename)
{
	ofstream outFile(filename);
	outFile << ssSystemOutFile.str();
	outFile.close();
}

void SystemState::storeOutputTable(char *filename, const vector<stringstream *>& ssTableRow)
{
	ofstream outFile(filename);

	outFile << "cohort" << TAB << "avg lifetime";

	outFile << TAB << "age " << 13.5F;

	for (float age = 14.5F; age <= ageMax; ++age)
	{
		outFile << TAB << age;
	}

	outFile << NEWLINE;

	for (CohortNumber cnum = 0; cnum <= maxCohort; ++cnum)
	{
		outFile << cnum << TAB << averageLifetime[cnum] << ssTableRow[cnum]->str();
	}
}
