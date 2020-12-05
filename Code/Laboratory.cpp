/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include "Laboratory.h"
#include <fstream>
#include <limits>
#include <algorithm>
#include <omp.h>

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

/*
struct FenwickTree {
    vector<int> bit;  // binary indexed tree
    int n;

    // Op2: initializing with an empty array
    FenwickTree(int n) {
        this->n = n + 1;
        bit.assign(n + 1, 0);
    }

    // Op2: initializing with existing array
    FenwickTree(vector<int> a)
        : FenwickTree(a.size()) {
        for (size_t i = 0; i < a.size(); i++)
            add(i, a[i]);
    }

    void add(int idx, int delta) {
        for (++idx; idx < n; idx += idx & -idx)
            bit[idx] += delta;
    }

    // Attention: don't mix both!! Choose one!

	// 1 - Point Update and Range Query

    int sum(int idx) {
        int ret = 0;
        for (++idx; idx > 0; idx -= idx & -idx)
            ret += bit[idx];
        return ret;
    }

    int sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }
};
*/

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
		 * Format: {ini, end} 0-indexed
		 */
		vector<vector<pair<int, int>>> occupiedSlots (M);

		// vector<FenwickTree> occupiedSlots(M, FenwickTree(K));

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
					for(int z = 0; z < occupiedSlots[m].size(); z++)
						if((occupiedSlots[m][z].first <= ini && ini <= occupiedSlots[m][z].second)
						   || (occupiedSlots[m][z].first <= end && end <= occupiedSlots[m][z].second)){
							available = false;
							break;
						}

					// printf("Sum is: %d\n", occupiedSlots[m].sum(ini, end));
					// if(occupiedSlots[m].sum(ini, end) != 0)
					// 	available = false;

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

				// Mark selected position as occupied in that machine.
				occupiedSlots[bestPosition[3]].push_back(make_pair(bestPosition[1], bestPosition[2]));
				// for(int z = bestPosition[1]; z <= bestPosition[2]; z++)
					// occupiedSlots[bestPosition[3]].add(z, 1);
			}
		}

		// If a solution was successfully generated, save it.
		if(!impossibleToInsert){
			printf("Inserted: (%d, %d)\n", solEnergyConsumption, makespan + 1);
			solutions.insertSol(solEnergyConsumption, makespan + 1, S);
		}
	}	
}