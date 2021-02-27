#include <vector>
using namespace std;

struct FenwickTree {
    vector<int> bit;  // binary indexed tree
    int n;
    
    // Constructor 1: initializing empty array
    FenwickTree(int n) {
        this->n = n + 1;
        bit.assign(n + 1, 0);
    }

    // Constructor 2: initializing with existing array
    FenwickTree(const vector<int> &a) : FenwickTree(a.size()) {
        for (size_t i = 0; i < a.size(); i++)
            add(i, a[i]);
    }

    // Do the same as constructor 2 but in a late initialization.
    void LateInit(const vector<int> &v) {
        if(v.size() != this->n) {
            printf("BIT size and Vector 'v' size doesn't match! Aborting.\n");
            exit(0);
        }

        for(size_t i = 0; i < v.size(); i++) {
            add(i, v[i]);
        }
    }

    void add(int idx, int delta) {
        for (++idx; idx < n; idx += idx & -idx)
            bit[idx] += delta;
    }

	// Point Update and Range Query
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