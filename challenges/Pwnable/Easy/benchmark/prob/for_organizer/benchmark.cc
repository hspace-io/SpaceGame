#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstring>
#include "benchmark.h"
#define MAX_BENCHMARKS 0x20

using namespace std;
using namespace chrono;

typedef struct _benchmark benchmark;
benchmark *g_benchmarks[MAX_BENCHMARKS];

void benchmark_func_inner(char *arg) {
    for (int i=0; i<strlen(arg); i++) {
        for (int j=0; j<1000000; j++) {
        }
    }
}

void benchmark_func(int bid) {
    benchmark *cur = g_benchmarks[bid];
    system_clock::time_point start_time = system_clock::now();  

    benchmark_func_inner(cur->arg);

    system_clock::time_point end_time = system_clock::now();
    milliseconds elapsed = duration_cast<milliseconds>(end_time - start_time);

    cur->elapsed = elapsed;
    cur->joined = 1;

    return;
}

int fetch_available_bid() {
    int bid;
    for (bid = 0; ; bid++) {
        if (bid == MAX_BENCHMARKS) break;
        if (g_benchmarks[bid] == NULL) return bid;
    }
    return -1;
}

void run_benchmark() {
    int bid = fetch_available_bid();
    if (bid < 0) {
        cout << "Cannot run more benchmark\n" << endl;
        return;
    }

    string input;
    cout << "Arg length : ";
    cin >> input;
    int arg_size;

    try {
        arg_size =  stoi(input);
        if (arg_size >= 0x1000 || arg_size < 0) {
            cout << "Invalid size\n" << endl;
            return;    
        }
    } catch (...) {
        cout << "Invalid size\n" << endl;
        return;
    }

    char * arg = new char[arg_size];
    cout << "Arg : ";
    int res = read(0, arg, arg_size);
    if (arg[res-1] == '\n')
        arg[res-1] = '\0';
    
    benchmark *cur = new benchmark;
    g_benchmarks[bid] = cur;

    cout << "Now running Benchmark " << bid << endl << endl;

    cur->joined = 0;
    cur->rc = 0;
    cur->parent = -1;
    cur->arg = arg;
    cur->arg_len = arg_size;
    cur->t = thread(benchmark_func, bid);

    return;
}

void cleanup_benchmark() {
    string input;
    cout << "Benchmark ID : ";
    cin >> input;
    int bid;

    try {
        bid =  stoi(input);
    } catch (...) {
        cout << "Invalid id\n" << endl;
        return;
    }

    if (g_benchmarks[bid] == NULL || (bid >= MAX_BENCHMARKS) || (bid < 0)) {
        cout << "Invalid id\n" << endl;
        return;
    }

    benchmark *cur = g_benchmarks[bid];
    if (cur->joined == 0) {
        cout << "Benchmark is yet running\n" << endl;
        return;
    }

    /* vuln
    if (cur->rc) {
        cout << "Benchmark is yet re-running\n" << endl;
        return;
    }
    */
    
    if (cur->parent < 0)
        delete cur->arg;
    if (cur->parent >= 0) {
        g_benchmarks[cur->parent]->rc--;
    }        
    cur->t.join();
    delete cur;
    g_benchmarks[bid] = NULL;

    cout << "Cleaned up Benchmark " << bid << endl << endl;
    return;
}

void revise_benchmark() {
    string input;
    cout << "Benchmark ID : ";
    cin >> input;
    int bid;

    try {
        bid =  stoi(input);
    } catch (...) {
        cout << "Invalid id\n" << endl;
        return;
    }

    if (g_benchmarks[bid] == NULL || (bid >= MAX_BENCHMARKS) || (bid < 0)) {
        cout << "Invalid id\n" << endl;
        return;
    }

    benchmark *cur = g_benchmarks[bid];
    if (cur->joined == 0) {
        cout << "Benchmark is yet running\n" << endl;
        return;
    }
    
    cout << "Arg : ";
    read(0, cur->arg, cur->arg_len);

    cout << "Revised Benchmark " << bid << endl << endl;
    return;
}

void rerun_benchmark() {
    int bid = fetch_available_bid();
    if (bid < 0) {
        cout << "Cannot run more benchmark\n" << endl;
        return;
    }

    string input;
    cout << "Benchmark ID : ";
    cin >> input;
    int parent_id;

    try {
        parent_id =  stoi(input);
    } catch (...) {
        cout << "Invalid ID\n" << endl;
        return;
    }

    if (g_benchmarks[parent_id] == NULL || (parent_id >= MAX_BENCHMARKS) || (parent_id < 0)) {
        cout << "Invalid ID\n" << endl;
        return;        
    }

    benchmark *parent = g_benchmarks[parent_id];

    if (parent->joined == 0) {
        cout << "Benchmark is yet running\n" << endl;
        return;
    }

    benchmark *cur = new benchmark;
    g_benchmarks[bid] = cur;

    cout << "Now running Benchmark " << bid << endl << endl;

    cur->joined = 0;
    cur->rc = 0;
    cur->arg = parent->arg;
    cur->arg_len = parent->arg_len;
    cur->parent = parent_id;
    parent->rc++;
    cur->t = thread(benchmark_func, bid);    
}

void status() {
    for (int i=0; i<MAX_BENCHMARKS; i++) {
        if (g_benchmarks[i] == NULL) continue;

        benchmark *cur = g_benchmarks[i];
        cout << "Benchmark " << i << " ";
        const char *b_status = cur->joined ? "<Joined>" : "<Running>";
        cout << b_status;
        cout << "(arg: " << cur->arg << ")";
        if (cur->joined)
            cout << ": " << cur->elapsed.count() << " ms";
        cout << endl;
    }
    cout << endl;
}