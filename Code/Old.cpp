/* ######################################################################################################## */

// Using Set Pair <int, int> to set job to an interval.

vector<set<pair<int, int>>> occupiedSlots(M);

//inside makespan for-loop
for(int j = 0; j < M; j++)
	occupiedSlots[j].clear();

//inside machines for-loop
auto it = occupiedSlots[m].lower_bound(make_pair(ini, -1));
					
for(int z = 1; z <= 2; z++){
	if(it != occupiedSlots[m].end())
		if(it->first <= ini && ini <= it->second || it->first <= end && end <= it->second){
			available = false;
			break;
		}

	if(occupiedSlots[m].size() != 0)
		it--;
}

//Saving
occupiedSlots[bestPosition[3]].insert(make_pair(bestPosition[1], bestPosition[2]));


/* ######################################################################################################## */

// Using Vector Pair <int, int> to set job to an interval.

//inside makespan for-loop
vector<vector<pair<int, int>>> occupiedSlots (M);

//inside machines for-loop
for(int z = 0; z < occupiedSlots[m].size(); z++)
	if((occupiedSlots[m][z].first <= ini && ini <= occupiedSlots[m][z].second)
	   || (occupiedSlots[m][z].first <= end && end <= occupiedSlots[m][z].second)){
		available = false;
		break;
	}	

//Saving
occupiedSlots[bestPosition[3]].push_back(make_pair(bestPosition[1], bestPosition[2]));


/* ######################################################################################################## */