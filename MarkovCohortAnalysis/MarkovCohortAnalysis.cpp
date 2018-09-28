// MarkovCohortAnalysis.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>
#include "SystemState.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SystemState state;
	state.processAllCohorts(maxCohort);
	return 0;
}

