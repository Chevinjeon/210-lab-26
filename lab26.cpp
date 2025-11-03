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
    list<string> lst1;
    set<string> st1;
    
    results[run][0][0] = read_vector(vec1);  // Read Vector
    results[run][0][1] = read_list(lst1);    // Read List
    results[run][0][2] = read_set(st1);      // Read Set
    
    // Race 2: SORT
    vector<string> vec2 = vec1;
    list<string> lst2 = lst1;
    
    results[run][1][0] = sort_vector(vec2);  // Sort Vector
    results[run][1][1] = sort_list(lst2);    // Sort List
    results[run][1][2] = -1;                  // Set already sorted
    
    // Race 3: INSERT
    vector<string> vec3 = vec2;
    list<string> lst3 = lst2;
    set<string> st3 = st1;
    
    results[run][2][0] = insert_vector(vec3);  // Insert Vector
    results[run][2][1] = insert_list(lst3);    // Insert List
    results[run][2][2] = insert_set(st3);      // Insert Set
    
    // Race 4: DELETE
    vector<string> vec4 = vec2;  // Use pre-insert state
    list<string> lst4 = lst2;
    set<string> st4 = st1;
    
    results[run][3][0] = delete_vector(vec4);  // Delete Vector
    results[run][3][1] = delete_list(lst4);    // Delete List
    results[run][3][2] = delete_set(st4);      // Delete Set
}

// Calculate averages from accumulated sums
void calculate_averages() {
    for (int op = 0; op < NUM_OPERATIONS; op++) {
        for (int str = 0; str < NUM_STRUCTURES; str++) {
            if (op == 1 && str == 2) {
                // Sort for Set is always -1, so average is 0
                results[NUM_RUNS][op][str] = 0;
            } else {
                // Calculate average (already summed, divide by NUM_RUNS)
                results[NUM_RUNS][op][str] = results[NUM_RUNS][op][str] / NUM_RUNS;
            }
        }
    }
}

// Display final averaged results
void display_results() {
    cout << "Number of simulations: " << NUM_RUNS << "\n";
    cout << left << setw(12) << "Operation" 
         << setw(12) << "Vector" 
         << setw(12) << "List" 
         << "Set" << endl;
    
    string operations[] = {"Read", "Sort", "Insert", "Delete"};
    
    for (int op = 0; op < NUM_OPERATIONS; op++) {
        cout << left << setw(12) << operations[op];
        for (int str = 0; str < NUM_STRUCTURES; str++) {
            if (op == 1 && str == 2) {
                // Sort for Set: display 0
                cout << setw(12) << 0;
            } else {
                cout << setw(12) << results[NUM_RUNS][op][str];
            }
        }
        cout << endl;
    }
}

// Race functions (adapted from Lab 25)
long long read_vector(vector<string>& vec) {
    auto start = high_resolution_clock::now();
    ifstream file(DATA_FILE);
    string line;
    while (getline(file, line)) {
        vec.push_back(line);
    }
    file.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
}

long long read_list(list<string>& lst) {
    auto start = high_resolution_clock::now();
    ifstream file(DATA_FILE);
    string line;
    while (getline(file, line)) {
        lst.push_back(line);
    }
    file.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
}

long long read_set(set<string>& st) {
    auto start = high_resolution_clock::now();
    ifstream file(DATA_FILE);
    string line;
    while (getline(file, line)) {
        st.insert(line);
    }
    file.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
}

long long sort_vector(vector<string>& vec) {
    auto start = high_resolution_clock::now();
    sort(vec.begin(), vec.end());
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
}

long long sort_list(list<string>& lst) {
    auto start = high_resolution_clock::now();
    lst.sort();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
}
