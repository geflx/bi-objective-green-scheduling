/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include "Laboratory.h"
#include <fstream>
#include <limits>
#include <algorithm>
#include <omp.h>
#include "FenwickTree.h"

using namespace std;

/**
 * Laboratory Constructor.
 * Reads instance from file directory.
 */
Laboratory::Laboratory(const char *fileDir){

	ifstream input(fileDir);

	input >> K >> N >> M;

	timePrice.resize(K);
	procTime.resize(N);
	machineEnergy.resize(M);

	for(int i = 0; i < K; i++) 
		input >> timePrice[i];

	for(int i = 0; i < N; i++) 
		input >> procTime[i];

	for(int i = 0; i < M; i++) 
		input >> machineEnergy[i];

	input.close();
}

/**
 * Print solutions.
 * Uses "ParetoSet.h" function to do this.
 */
void Laboratory::printSolutions(){
	solutions.printSet();
}

// Reviews: 0
void Laboratory::constructiveHeuristic(){

	// Format: {value, id}.
	vector<pair<int, int>> sortedMachines(M);
	vector<pair<int, int>> sortedJobs(N);
	vector<int> timePricePreffixSum(K);

	bool impossibleToInsert = false;

	for(int i = 0; i < M; i++)
		sortedMachines[i] = make_pair(machineEnergy[i], i);

	for(int i = 0; i < N; i++)
		sortedJobs[i] = make_pair(procTime[i], i);

	// Initialize preffixSum.
	for(int i = 0; i < K; i++)
		if(i == 0)
			timePricePreffixSum[0] = timePrice[0];
		else
			timePricePreffixSum[i] = timePricePreffixSum[i - 1] + timePrice[i];

	// Sorting machines in increasing order of energy consumption.
	sort(sortedMachines.begin(), sortedMachines.end());

	// Sorting jobs in decreasing order of processing time.
	sort(sortedJobs.begin(), sortedJobs.end(), greater<pair<int, int>>());

	for(int i = K - 1; i >= 0 && !impossibleToInsert; i--){

		/**
		 * Indicates current slot occupation in the M machines.
		 * Each machine's fenwickTree has M slots, if '1' the slot is visited and '0' otherwise.
		 */
		vector<FenwickTree> occupiedSlots(M, FenwickTree(K, 0));	

		Solution S(N);
		int solEnergyConsumption = 0, makespan = -1;

		for(int j = 0; j < N && !impossibleToInsert; j++){

			// {value, iniPos, endPos, machine}
			vector<int> bestPosition = {numeric_limits<int>::max(), -1, -1, -1};

			// Finding best slot to fit job.
			for(int k = 0; k <= i - procTime[sortedJobs[j].second] + 1; k++){
				
				int ini = k, end = k + procTime[sortedJobs[j].second] - 1;
				int slotSum = (ini == 0 ? timePricePreffixSum[end] : timePricePreffixSum[end] - timePricePreffixSum[ini - 1]);

				// Ignore slot checking if best scenario current sum is worse than bestPosition. 
				if(slotSum * sortedMachines[0].first >= bestPosition[0])
					continue;

				for(int m = 0; m < M; m++){

					bool available = true;

					// Check if slots are available in that machine.
					if(occupiedSlots[m].sum(ini, end) != 0)
						available = false;

					// If slot is available, check if value is interesting.
					if(available)
						if(slotSum * sortedMachines[m].first < bestPosition[0] || 
						   (slotSum * sortedMachines[m].first == bestPosition[0] && ini <= bestPosition[1])){
							bestPosition = {slotSum * sortedMachines[m].first, ini, end, m};
						}
				}
			}

			// Check if best position wasn't found.
			if(bestPosition[1] == -1){
				printf("Impossible, makespan: %d\n", i + 1);
				impossibleToInsert = true;
			}
			else{
				// Accumulate energy consumption in solution's obj. function.
				solEnergyConsumption += bestPosition[0];

				// Save job's destinated machine and position in solution.
				S.setV(sortedJobs[j].second, bestPosition[3], bestPosition[1]);

				// Updating solution makespan.\
				makespan = max(makespan, bestPosition[2]);

				// Set selected positions as occupied in that machine.
				for(int z = bestPosition[1]; z <= bestPosition[2]; z++)
					occupiedSlots[bestPosition[3]].add(z, 1);
			}
		}

		// If a solution was successfully generated, save it.
		if(!impossibleToInsert)
			solutions.insertSol(solEnergyConsumption, makespan + 1, S);
	}	
}

// Revisar!!!!!!!!
Location Laboratory::SimpleSplitGreedyCH_GetBestLocation(const vector<vector<int>> &assignmentTable, int currK, int currP) {

	Location bestLocation;

	for(int i = 0; i < M; i++) {

		int leftDelimiter = (currK - currP);

		for(int j = 0; j <= leftDelimiter; j++){

			// Location must start in an empty slot.
			if(assignmentTable[i][j] != -1)
				continue;

			int numSlots = 0, cost = 0, end = j;
			for(end = j; end <= currK && numSlots < currP; end++){
				if(assignmentTable[i][end] == -1){
					numSlots++;
					cost += procTime[end] * machineEnergy[i];
				}
			}

			if(numSlots == currP) {
				Location tmp;

				tmp.machine = i;
				tmp.cost = cost;
				tmp.beg = j;
				tmp.end = end - 1;
				tmp.isContinuous = (end - beg == currP ? true : false);

				bestLocation = max(bestLocation, tmp);
			}
		}
	}

	return bestLocation;
}

// Revisar!!!!!!!!
bool Laboratory::SimpleSplitGreedyCH_AssignLocation(vector<vector<int>> &assignmentTable, const Location &insertionLocation, const int jobId, int &objFunction) {
	
	// Invalid location. Return false.
	if(insertionLocation.machine == -1)
		return false;

	for(int i = insertionLocation.beg; i <= insertionLocation.end; i++)
		if(assignmentTable[insertionLocation.machine][i] == -1)
			assignmentTable[insertionLocation.machine][i] = jobId;

	objFunction += insertionLocation.cost;

	return true;
}

// Revisar!!!!!!!!
Solution Laboratory::SimpleSplitGreedyCH_ConvertSolution(const vector<vector<int>> &assignmentTable, const int currK, const int SolutionCost, const int SolutionMakespan) {
	Solution S;

	set<int> insertedJobs;

	for(int i = 0; i < H; i++) {

		int nextSlot = -1;

		for(int j = 0; j <= currK; j++) {

			if(assignmentTable[i][j] != -1 && insertedJobs.find(assignmentTable[i][j]) != insertedJobs.end()) {

				if(nextSlot == -1) {
					S.setV(assignmentTable[i][j], i, j);
					nextSlot = j + procTime[assignmentTable[i][j]];
				}
				else {
					S.setV(assignmentTable[i][j], i, nextSlot);
					nextSlot = nextSlot + procTime[assignmentTable[i][j]];
				}
			}
		}
	}

	S.setObj(SolutionCost, SolutionMakespan);

	return S;
}

// Revisar!!!!!!!!
void Laboratory::SimpleSplitGreedyCH() {

	bool impossibleToInsert = false;

	// Format: {value, id}.
	vector<pair<int, int>> sortedJobs(N);

	for(int i = 0; i < N; i++)
		sortedJobs[i] = make_pair(procTime[i], i);

	// Sorting jobs in decreasing order of processing time.
	sort(sortedJobs.begin(), sortedJobs.end(), greater<pair<int, int>>());

	for(int i = K - 1; i >= 0 && !impossibleToInsert; i--){

		// Initialization table starts empty.
		vector<vector<int>> assignmentTable (M, vector<int> (i, -1));
		int solutionCost = 0, solutionMakespan = -1;
		bool isSplitSchedule = false;

		for(int j = 0; j < N; j++) {

			Location insertionLocation = SimpleSplitGreedyCH_GetBestLocation(assignmentTable, i, sortedJobs[j].first);
			// if(!insertionLocation.isContinuous)
			// 	isSplitSchedule = true;
			solutionMakespan = max(solutionMakespan, insertionLocation.end);

			if(!SimpleSplitGreedyCH_AssignLocation(assignmentTable, insertionLocation, sortedJobs[j].second, solutionCost)){
				impossibleToInsert = true;
				break;
			}
		}

		if(!impossibleToInsert) {
			Solution S = convertSolution(assignmentTable, i, solutionCost, solutionMakespan);
			solutions.insertSol(solutionCost, solutionMakespan, S); // Warning: check if it is solutionMakespan or solutionMakespan + 1
		}
	}
}


/*
void Laboratory::SplitGreedyCH(){

	// Format: {value, id}.
	vector<pair<int, int>> sortedJobs(N);

	bool impossibleToInsert = false;

	for(int i = 0; i < N; i++)
		sortedJobs[i] = make_pair(procTime[i], i);

	// Sorting jobs in decreasing order of processing time.
	sort(sortedJobs.begin(), sortedJobs.end(), greater<pair<int, int>>());

	for(int i = K - 1; i >= 0 && !impossibleToInsert; i--){

		vector<FenwickTree> occupationFT(M, FenwickTree(i));
		vector<FenwickTree> costFT(M, FenwickTree(i));


	}
}
*/