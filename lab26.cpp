#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <chrono>
#include <algorithm>
#include <iomanip>
using namespace std;
using namespace std::chrono;

const string DATA_FILE = "codes.txt";
const string TEST_VALUE = "TESTCODE";
const int NUM_ELEMENTS = 20000;
const int NUM_RUNS = 15;
const int NUM_OPERATIONS = 4;  // Read, Sort, Insert, Delete
const int NUM_STRUCTURES = 3;  // Vector, List, Set

// 3D Array structure: [run][operation][structure]
// Dimension 0: Run number (0-14) + accumulator slice at [NUM_RUNS]
// Dimension 1: Operation (0=Read, 1=Sort, 2=Insert, 3=Delete)
// Dimension 2: Structure (0=Vector, 1=List, 2=Set)
long long results[NUM_RUNS + 1][NUM_OPERATIONS][NUM_STRUCTURES];

// Function declarations for race operations
long long read_vector(vector<string>& vec);
long long read_list(list<string>& lst);
long long read_set(set<string>& st);
long long sort_vector(vector<string>& vec);
long long sort_list(list<string>& lst);
long long insert_vector(vector<string>& vec);
long long insert_list(list<string>& lst);
long long insert_set(set<string>& st);
long long delete_vector(vector<string>& vec);
long long delete_list(list<string>& lst);
long long delete_set(set<string>& st);

// Helper functions for running experiments
void run_single_experiment(int run);
void initialize_accumulator();
void calculate_averages();
void display_results();

int main() {
    cout << "Data Structures Olympics - Lab 26\n";
    cout << "Running " << NUM_RUNS << " simulations...\n\n";
    
    // Initialize accumulator slice to 0
    initialize_accumulator();
    
    // Run experiments 15 times
    for (int run = 0; run < NUM_RUNS; run++) {
        run_single_experiment(run);
        
        // Accumulate results into the accumulator slice
        for (int op = 0; op < NUM_OPERATIONS; op++) {
            for (int str = 0; str < NUM_STRUCTURES; str++) {
                // Skip -1 values (Set Sort), treat as 0
                if (results[run][op][str] != -1) {
                    results[NUM_RUNS][op][str] += results[run][op][str];
                }
            }
        }
    }
    
    // Calculate and display averages
    calculate_averages();
    display_results();
    
    return 0;
}

// Initialize accumulator slice to zero
void initialize_accumulator() {
    for (int op = 0; op < NUM_OPERATIONS; op++) {
        for (int str = 0; str < NUM_STRUCTURES; str++) {
            results[NUM_RUNS][op][str] = 0;
        }
    }
}

// Run a single experiment (one complete set of races)
void run_single_experiment(int run) {
    // Race 1: READ
    vector<string> vec1;
