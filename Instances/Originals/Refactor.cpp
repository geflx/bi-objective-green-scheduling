/**
 * @author 	Gabriel Felix
 * @date 	11/30/20
 */

#include <bits/stdc++.h>
using namespace std;

int main(){

	for(int i = 1; i <= 90; i++){

		ifstream timeSlot("Data_c" + to_string(i) + ".txt");
		ifstream procTime("Data_p" + to_string(i) + ".txt");
		ifstream macPrice("Data_e" + to_string(i) + ".txt");

		int K = 0, N = 0, M = 0, tmp;
		vector<int> T, J, H;

		while(timeSlot >> tmp){
			T.push_back(tmp);
			K++;
		}

		while(procTime >> tmp){
			J.push_back(tmp);
			N++;
		}

		while(macPrice >> tmp){
			H.push_back(tmp);
			M++;
		}

		ofstream instance("RData_" + to_string(i) + ".txt");
		instance << K << " " << N << " " << M << "\n";

		for(int j = 0; j < K; j++)
			instance << T[j] << " \n"[j == K - 1];

		for(int j = 0; j < N; j++)
			instance << J[j] << " \n"[j == N - 1];

		for(int j = 0; j < M; j++)
			instance << H[j] << " \n"[j == M - 1];

		timeSlot.close(), procTime.close(), macPrice.close(), instance.close();
	}
} 
