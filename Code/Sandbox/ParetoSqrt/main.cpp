#include "ParetoSqrt.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <chrono> 
#include <ctime>

using namespace std;

int random(long int *seed, int low, int high){
    int  unif_ret;
    long int M, a,b,c, k;
    double value_0_1;
    M = 2147483647;
    a = 16807;
    b = 127773;
    c = 2836;
    k = *seed/b;
    *seed = a*(*seed%b) -  k*c;
    if( *seed <0 )  *seed = *seed + M;
    value_0_1 = (double)*seed/M;
    unif_ret = low + int(value_0_1 *(high - low +1));
    return(unif_ret);
 }

long int S[5] = {480121850, 890621852, 310626852,  620621852, 650926852};

int main(){

	ParetoSqrt sol(2);
	
    long int semente = S[1];
    int x, y, p;
    float seconds;
    clock_t start, end;
    start = clock();
    
    vector<int> obj = {0, 0};
    
    for(int n=0; n<500000000; n++){
        
        // if(n % 1000000 == 0)
        //     cout << "cont is " << n << endl;

        p = random(&semente, 1, 4);
        if(p==1){
            x = random(&semente, 0, 250000);
            y = random(&semente, 800001, 1200000);
            //z = random(&semente, 250000, 0);
            obj[0] = x;
            obj[1] = y;
    		Solution s(obj);
            sol.insertSol(s);
            //sol.insertSol(x, y, z);
        }
        if(p==2){
            x = random(&semente, 250001, 500000);
            y = random(&semente, 500001, 800000);
            //z = random(&semente, 500000, 250000);

            obj[0] = x;
            obj[1] = y;
    		Solution s(obj);
            sol.insertSol(s);
            //sol.insertSol(x, y, z);
        }
        if(p==3){
            x = random(&semente, 500001, 800000);
            y = random(&semente, 250001, 500000);
            //z = random(&semente, 800000, 500000);

            obj[0] = x;
            obj[1] = y;
    		Solution s(obj);
            sol.insertSol(s);
            //sol.insertSol(x, y, z);
        }
        if(p==4){
            x = random(&semente, 800001, 120000);
            y = random(&semente, 0, 250000);
            //z = random(&semente, 120000, 800000);
            
            obj[0] = x;
            obj[1] = y;
    		Solution s(obj);
            sol.insertSol(s);
            //sol.insertSol(x, y, z);
        }
    }
    end = clock();

	sol.printVec();
    
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    cout <<"Tempo: "<< seconds*1000 << " ms." << endl;

	
    
 //    ParetoSqrt sol(2);

	// vector<int> obj = {1, 4};
	// Solution s(obj);
	// sol.insertSol(s);
	// // cout << "hi\n";

	// obj[0] = 2;
	// obj[1] = 2;
	// Solution s2(obj);
	// sol.insertSol(s2);

	// obj[0] = 2;
	// obj[1] = 3;
	// Solution s3(obj);
	// sol.insertSol(s3);

	// obj[0] = 4;
	// obj[1] = 2;
	// Solution s4(obj);
	// sol.insertSol(s4);

	// obj[0] = 2;
	// obj[1] = 1;
	// Solution s5(obj);
	// sol.insertSol(s5);

 //    obj[0] = 5;
 //    obj[1] = 0;
 //    Solution s6(obj);
 //    sol.insertSol(s6);

	// sol.printVec();

	
	
}
