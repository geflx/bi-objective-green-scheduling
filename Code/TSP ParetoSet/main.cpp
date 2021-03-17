/**
 *	@author Gabriel Felix
 * 	@date	16/03/21
 */

#include "ParetoSet.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <numeric>

using namespace std;

void generateAdjMatrix (const int N, vector<vector<int>> &matrix) {

	matrix.resize(N + 1, vector<int> (N + 1, 0));

	for(int i = 0; i < N + 1; i++)
		for(int j = 0; j < i; j++)
			matrix[i][j] = matrix[j][i] = rand() % 1000;
}

int getObjF (const int N, const vector<vector<int>> &adjMatrix, const vector<int> &positions) {

	int cost = 0;

	// Adding cost of origin -> first and last -> origin
	cost += adjMatrix[0][positions[0]];
	cost += adjMatrix[0][positions[N - 1]];

	// Adding remaining route costs
	for(int i = 0; i < N - 1; i++) {

		int from = positions[i];
		int to = positions[i + 1];

		cost += adjMatrix[from][to];
	}

	return cost;
}

void generateSolutions (ParetoSet &solutions, const int N, const int numSolutions,
						const vector<vector<int>> &adjMatrix1, const vector<vector<int>> &adjMatrix2) {

	for(int i = 0; i < numSolutions; i++) {

		Solution S(N);

		// Initialize vectors with positions {1, 2, 3 ...}
		vector<int> positions1(N), positions2(N);
		iota(begin(positions1), end(positions1), 1);
		iota(begin(positions2), end(positions2), 1);

		// Random shuffle vectors
		random_shuffle(positions1.begin(), positions1.end());
		random_shuffle(positions2.begin(), positions2.end());

		// Calculate route's objective function.
		int route1Cost = getObjF(N, adjMatrix1, positions1);
		int route2Cost = getObjF(N, adjMatrix2, positions2);

		// Adding route configuration to solution.
		for(int i = 0; i < N; i++){
			S.setV(0, i, positions1[i]);
			S.setV(1, i, positions2[i]);
		}

		// Storing obj. function costs in solution.
		S.setObj(route1Cost, route2Cost);

		// Adding solution to ParetoSet.
		solutions.insertSol(route1Cost, route2Cost, S);
	}
}

int main(){

	// Number of nodes, number of solutions, ParetoSet and adj. matrix.
	int N, numSolutions;
	ParetoSet solutions;
	vector<vector<int>> adjMatrix1, adjMatrix2;

	// Reading values.
	printf("Insert number of nodes (not counting root): ");
	cin >> N;

	printf("Insert number of solutions: ");
	cin >> numSolutions;

	// Creating random adj. matrix of N + 1 nodes (counting root).
	generateAdjMatrix(N, adjMatrix1);
	generateAdjMatrix(N, adjMatrix2);

	generateSolutions(solutions, N, numSolutions, adjMatrix1, adjMatrix2);

	solutions.printSet();
}

