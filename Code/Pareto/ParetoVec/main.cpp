#include "ParetoVec.h"
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
	ParetoVec sol;
	
    long int semente = S[1];
    int x, y, p;
    float seconds;
    clock_t start, end;
    start = clock();
    
    for(int n=0; n<500000000; n++){
        
        p = random(&semente, 1, 4);
        if(p==1){
            x = random(&semente, 0, 250000);
            y = random(&semente, 800001, 1200000);
            //z = random(&semente, 250000, 0);
            sol.insertSol(x, y);
            //sol.insertSol(x, y, z);
        }
        if(p==2){
            x = random(&semente, 250001, 500000);
            y = random(&semente, 500001, 800000);
            //z = random(&semente, 500000, 250000);

            sol.insertSol(x, y);
            //sol.insertSol(x, y, z);
        }
        if(p==3){
            x = random(&semente, 500001, 800000);
            y = random(&semente, 250001, 500000);
            //z = random(&semente, 800000, 500000);

            sol.insertSol(x, y);
            //sol.insertSol(x, y, z);
        }
        if(p==4){
            x = random(&semente, 800001, 120000);
            y = random(&semente, 0, 250000);
            //z = random(&semente, 120000, 800000);
            
            sol.insertSol(x, y);
            //sol.insertSol(x, y, z);
        }
    }
    end = clock();

    
    
	/*sol.insertSol(10, 40);
	sol.insertSol(20, 10);
	sol.insertSol(20, 30);
	sol.insertSol(40, 20);
	sol.insertSol(50, 2);
    
    sol.insertSol(60, 2);
    sol.insertSol(70, 2);
    sol.insertSol(10, 50);
    sol.insertSol(10, 60);
    
    sol.insertSol(40, 5);
    sol.insertSol(15, 30);
    
    sol.insertSol(60, 1);
    sol.insertSol(5, 50);
    
    sol.insertSol(50, 1);
    sol.insertSol(5, 40);
    
    sol.insertSol(20, 9);*/


	sol.printVec();
    
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    cout <<"Tempo: "<< seconds*1000 << " ms." << endl;
}
