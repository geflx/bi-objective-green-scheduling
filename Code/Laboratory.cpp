/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include "Laboratory.h"
#include <fstream>
#include <limits>
#include <algorithm>
#include <omp.h>
#include <assert.h>
#include <iostream>

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


void Laboratory::checkSolutions() {

	vector<Solution> vecSol = this->solutions.getSolutions();

	for(Solution &S : vecSol){

		int cost = 0, makespan = -1;

		for(int i = 0; i < N; i++){

			int machine = S.getV(0, i);
			int pos = S.getV(1, i);
			int end = pos + procTime[i] - 1;


			if(machine < 0 || machine > M) {
				printf("Machine is out of boundaries [0, %d], id is: %d.\n", M - 1, machine);
				exit(0);
			}

			if(pos < 0 || pos > S.getObj().second) {
				printf("Initial position is out of boundaries [0, %d], position is: %d.\n", S.getObj().second, pos);
				exit(0);
			}

			if(end < 0 || end > S.getObj().second) {
				printf("End position is out of boundaries [0, %d], end is: %d.\n", S.getObj().second, end);
				exit(0);
			}

			// Updating total cost and total makespan.
			for(int k = pos; k <= end; k++)
				cost += (timePrice[k] * machineEnergy[machine]);
	
			makespan = max(makespan, pos + procTime[i] - 1);

			for(int j = 0; j < N; j++){

				int machineTmp = S.getV(0, j);

				// Return if is same job or different machines.
				if(i == j || (machine != machineTmp))
					continue;
				
				int posTmp = S.getV(1, j);
				int endTmp = posTmp + procTime[j] - 1;

				// Checking job's gap overlay.
				if((posTmp >= pos && posTmp <= end) || (endTmp >= pos && endTmp <= end)) {

					pair<int, int> tmpObj = S.getObj();
					printf("Solution (%d,%d) because job %d position conflicts with job %d at machine %d.",
							tmpObj.first, tmpObj.second, i, j, machine);
					exit(0);
				}
			}
		}

		pair<int, int> objF = S.getObj();

		if(objF.first != cost) {
			printf("Problem in solution (%d,%d): expected cost %d differs from main cost %d (calculated during construction).",
					objF.first, objF.second, cost, objF.first);
			exit(0);
		}

		if(objF.second != makespan) {
			printf("Problem in solution (%d,%d): expected makespan %d differs from main makespan %d (calculated during construction).",
					objF.first, objF.second, makespan, objF.second);
			exit(0);
		}
	}
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
		vector<FenwickTree> occupiedSlots(M, FenwickTree(K));	

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

				// Updating solution makespan.
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

// Reviews: 1
bool Laboratory::SimpleSplitGreedyCH_AssignLocation(vector<vector<int>> &assignmentTable, const Location &insertionLocation, const int jobId, int &objFunction) {
	
	// Invalid machine, it means there is no such feasible location.
	if(insertionLocation.machine == -1)
		return false;

	for(int i = insertionLocation.beg; i <= insertionLocation.end; i++)
		if(assignmentTable[insertionLocation.machine][i] == -1)
			assignmentTable[insertionLocation.machine][i] = jobId;

	objFunction += insertionLocation.cost;

	return true;
}

// Reviews: 1
Solution Laboratory::SimpleSplitGreedyCH_ConvertSolution(const vector<vector<int>> &assignmentTable, const int currK, const int SolutionCost, const int SolutionMakespan) {
	
	Solution S(N);
	set<int> insertedJobs;

	for(int i = 0; i < M; i++) {

		int nextSlot = -1;

		for(int j = 0; j <= currK; j++) {

			// Reseting next slot cause a free slot was found.
			if(assignmentTable[i][j] == -1 && nextSlot != -1)
				nextSlot = -1;

			if(assignmentTable[i][j] != -1 && insertedJobs.find(assignmentTable[i][j]) == insertedJobs.end()) {

				if(nextSlot == -1) {

					//Set job x in machine i at slot j.
					S.setV(assignmentTable[i][j], i, j);
					nextSlot = j + procTime[assignmentTable[i][j]];
				}
				else {

					S.setV(assignmentTable[i][j], i, nextSlot);
					nextSlot += procTime[assignmentTable[i][j]];
				}

				insertedJobs.insert(assignmentTable[i][j]);
			}
		}
	}

	S.setObj(SolutionCost, SolutionMakespan);

	// \for(int i = 0; i < M; i++) {
	// 	for(int j = 0; j < currK; j++) {
	// 		printf("%d ", assignmentTable[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// printf("\n\n\n\n");

	return S;
}

// Reviews: 0
Location Laboratory::SimpleSplitGreedyCH_GetBestLocation(const vector<vector<int>> &assignmentTable, int currK, int currP) {

	Location bestLocation;

	for(int i = 0; i < M; i++) {

		int leftDelimiter = currK - currP + 1;

		for(int j = 0; j <= leftDelimiter; j++){

			// Location must start in an empty slot.
			if(assignmentTable[i][j] != -1)
				continue;

			int numSlots = 0, cost = 0, end = j;

			for(end = j; (end <= currK) && (numSlots < currP); end++){

				// If slot is empty...
				if(assignmentTable[i][end] == -1){
					
					numSlots++;
					cost += timePrice[end] * machineEnergy[i];
				}
			}
			
			if(numSlots != currP)
				continue;

			if(cost != 0 && cost < bestLocation.cost || (cost == bestLocation.cost && j < bestLocation.beg)) {

				bestLocation.machine = i;
				bestLocation.cost = cost;
				bestLocation.beg = j;
				bestLocation.end = end - 1;
			}
		}
	}

	return bestLocation;
}

// Reviews: 0
void Laboratory::SimpleSplitGreedyCH() {

	bool impossibleToInsert = false;

	// Format: {value, id}.
	vector<pair<int, int>> sortedJobs(N);

	for(int i = 0; i < N; i++)
		sortedJobs[i] = make_pair(procTime[i], i);

	// Sorting jobs in decreasing order of processing time.
	sort(sortedJobs.begin(), sortedJobs.end(), greater<pair<int, int>>());

	
	for(int i = K - 1; i >= 0 && !impossibleToInsert; i--){
		
		// Assignment table starts empty.
		vector<vector<int>> assignmentTable (M, vector<int> (i + 1, -1));
		int solutionCost = 0, solutionMakespan = -1;

		for(int j = 0; j < N; j++) {

			// Get best location to insert.
			Location insertionLocation = SimpleSplitGreedyCH_GetBestLocation(assignmentTable, i, sortedJobs[j].first);
			
			// Update solution makespan.
			solutionMakespan = max(solutionMakespan, insertionLocation.end);

			// Trying to assign job to location.
			if(!SimpleSplitGreedyCH_AssignLocation(assignmentTable, insertionLocation, sortedJobs[j].second, solutionCost)){
				impossibleToInsert = true;
				break;
			}
		}
		
		if(!impossibleToInsert) {
			Solution S = SimpleSplitGreedyCH_ConvertSolution(assignmentTable, i, solutionCost, solutionMakespan);
			solutions.insertSol(solutionCost, solutionMakespan, S); 
		}
	}	
}

bool Laboratory::FenwickTreeSplitGreedyCH_AssignLocation(vector<vector<int>> &assignmentTable, const Location &insertionLocation, const int jobId, 
														int &solutionCost, vector<FenwickTree> &occupationFT, vector<FenwickTree> &costFT){

	// No location available, unable to assign job.
	if(insertionLocation.machine == -1)
		return false;

	int machine = insertionLocation.machine;
	int assignedSlots = 0;

	for(int i = insertionLocation.beg; i <= insertionLocation.end && assignedSlots < procTime[jobId]; i++) {

		if(assignmentTable[machine][i] == -1) {

			assignmentTable[machine][i] = jobId;
			assignedSlots++;

			occupationFT[machine].add(i, -1);

			costFT[machine].add(i, -timePrice[i]);	
		}
	}

	solutionCost += insertionLocation.cost;

	return true;

}

void Laboratory::FenwickTreeSplitGreedyCH_BuildFreeLocations(vector<vector<Location>> &vecLocation, const int machine, const int currK, const int currP, const vector<vector<int>> &assignmentTable, vector<FenwickTree> &occupationFT, vector<FenwickTree> &costFT){

	vecLocation[machine].clear();

	for(int i = 0; i <= currK && occupationFT[machine].sum(i, currK) >= currP; i++) {

		if(assignmentTable[machine][i] != -1)
			continue;

		int beg = i, end = currK;

		while(occupationFT[machine].sum(i, end) > currP){

			int mid = (beg + end) / 2;
			int f = occupationFT[machine].sum(i, mid);

			if(f < currP) {
				beg = mid;
			}
			else if(f == currP) {
				end = mid;
			}
			else if(f > currP) {
				end = mid - 1;
			}
		}

		Location tmp;
		tmp.cost = costFT[machine].sum(i, end) * machineEnergy[machine];
		tmp.beg = i;
		tmp.end = end;
		tmp.machine = machine;

		vecLocation[machine].push_back(tmp);

	}

	sort(vecLocation[machine].begin(), vecLocation[machine].end(), [](const Location &a, const Location &b) {return (a.cost > b.cost);});
}

void Laboratory::FenwickTreeSplitGreedyCH(){
	
	bool impossibleToInsert = false;

	map<int, vector<int>, greater<int>> procTimeToJob;

	for(int i = 0; i < N; i++)
		procTimeToJob[procTime[i]].push_back(i);

	// Template vectors: occupationFT will initialize with value "1" and costFT with time slot prices.
	vector<int> occupationFT_Template(K, 1);
	vector<int> costFT_Template = timePrice;

	for(int i = K - 1; i >= 0 && !impossibleToInsert; i--){

		// Resizing template vectors to match with currK iteration size.
		occupationFT_Template.resize(i + 1);
		costFT_Template.resize(i + 1);

		// Building Occupation Fenwick Trees and Cost Fenwick Trees, one per each machine.
		vector<FenwickTree> occupationFT(M, FenwickTree(occupationFT_Template));
		vector<FenwickTree> costFT(M, FenwickTree(costFT_Template));

		// Assignment table starts empty.
		vector<vector<int>> assignmentTable (M, vector<int> (i + 1, -1));
		int solutionCost = 0, solutionMakespan = -1;

		for(auto it : procTimeToJob) {

			int currP = it.first;

			vector<vector<Location>> vecLocation (M); 

			for(int k = 0; k < M; k++)
				FenwickTreeSplitGreedyCH_BuildFreeLocations(vecLocation, k, i, currP, assignmentTable, occupationFT, costFT);
			

			for(int j = 0; j < it.second.size(); j++) {

				Location bestLocation;

				for(int k = 0; k < M; k++)
					if(vecLocation[k].size() != 0)
						if(vecLocation[k].back().cost < bestLocation.cost || 
						  (vecLocation[k].back().cost == bestLocation.cost && vecLocation[k].back().beg < bestLocation.beg))
							bestLocation = vecLocation[k].back();

				if(!FenwickTreeSplitGreedyCH_AssignLocation(assignmentTable, bestLocation, it.second[j], solutionCost, occupationFT, costFT)){
					impossibleToInsert = true;
					break;
				}

				solutionMakespan = max(solutionMakespan, bestLocation.end);

				// If there is another job which happens after "j", rebuild free locations from the current best location machine.
				if(j + 1 < it.second.size())
					FenwickTreeSplitGreedyCH_BuildFreeLocations(vecLocation, bestLocation.machine, i, currP, assignmentTable, occupationFT, costFT);
			}

			if(impossibleToInsert){
				break;
			}
		}

		if(!impossibleToInsert) {
			Solution S = SimpleSplitGreedyCH_ConvertSolution(assignmentTable, i, solutionCost, solutionMakespan);
			solutions.insertSol(solutionCost, solutionMakespan, S); 
		}
	}
}


void Laboratory::Fast_FenwickTreeSplitGreedyCH_GetBestFreeLocation(Location &bestLocation, const int currK, const int currP, const vector<vector<int>> &assignmentTable, vector<FenwickTree> &occupationFT, vector<FenwickTree> &costFT){

	for(int machine = 0; machine < M; machine++){
		for(int i = 0; i <= currK && occupationFT[machine].sum(i, currK) >= currP; i++) {

			if(assignmentTable[machine][i] != -1)
				continue;

			int beg = i, end = currK;

			while(occupationFT[machine].sum(i, end) > currP){

				int mid = (beg + end) / 2;
				int f = occupationFT[machine].sum(i, mid);

				if(f < currP) {
					beg = mid;
				}
				else if(f == currP) {
					end = mid;
				}
				else if(f > currP) {
					end = mid - 1;
				}
			}

			int costTmp = costFT[machine].sum(i, end) * machineEnergy[machine];
			if(costTmp < bestLocation.cost || (costTmp == bestLocation.cost && i < bestLocation.beg)){
				bestLocation.cost = costTmp;
				bestLocation.beg = i;
				bestLocation.end = end;
				bestLocation.machine = machine;
			}
		}
	}
}

void Laboratory::Fast_FenwickTreeSplitGreedyCH(){
	
	map<int, vector<int>, greater<int>> procTimeToJob;
	int maxPTime = -1;

	for(int i = 0; i < N; i++){
		maxPTime = max(maxPTime, procTime[i]);
		procTimeToJob[procTime[i]].push_back(i);
	}

	// Template vectors: occupationFT will initialize with value "1" and costFT with time slot prices.
	vector<int> occupationFT_Template(K, 1);
	vector<int> costFT_Template = timePrice;

	#pragma omp parallel for
		for(int i = K - 1; i >= maxPTime; i--){

			// Building Occupation Fenwick Trees and Cost Fenwick Trees, one per each machine.
			vector<FenwickTree> occupationFT(M, FenwickTree(occupationFT_Template, i + 1));
			vector<FenwickTree> costFT(M, FenwickTree(costFT_Template, i + 1));

			// Assignment table starts empty.
			vector<vector<int>> assignmentTable (M, vector<int> (i + 1, -1));
			int solutionCost = 0, solutionMakespan = -1;

			bool infeasible = false;

			for(auto it : procTimeToJob) {

				if(infeasible)
					break;

				int currP = it.first;

				vector<vector<Location>> vecLocation (M); 

				for(int k = 0; k < M; k++)
					FenwickTreeSplitGreedyCH_BuildFreeLocations(vecLocation, k, i, currP, assignmentTable, occupationFT, costFT);
				
				for(int j = 0; j < it.second.size() && !infeasible; j++) {

					Location bestLocation;

					for(int k = 0; k < M; k++)
						if(vecLocation[k].size() != 0)
							if(vecLocation[k].back().cost < bestLocation.cost || 
							  (vecLocation[k].back().cost == bestLocation.cost && vecLocation[k].back().beg < bestLocation.beg))
								bestLocation = vecLocation[k].back();

					if(!FenwickTreeSplitGreedyCH_AssignLocation(assignmentTable, bestLocation, it.second[j], solutionCost, occupationFT, costFT)){
						infeasible = true;
						break;
					}
					else{
						solutionMakespan = max(solutionMakespan, bestLocation.end);
					}

					// If there is another job which happens after "j", rebuild free locations from the current best location machine.
					if(!infeasible && j + 1 < it.second.size())
						FenwickTreeSplitGreedyCH_BuildFreeLocations(vecLocation, bestLocation.machine, i, currP, assignmentTable, occupationFT, costFT);
				}
			}

			if(!infeasible) {
				Solution S = SimpleSplitGreedyCH_ConvertSolution(assignmentTable, i, solutionCost, solutionMakespan);
				solutions.insertSol(solutionCost, solutionMakespan, S); 
			}
		}
}

