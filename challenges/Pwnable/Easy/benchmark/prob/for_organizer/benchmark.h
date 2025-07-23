#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

typedef struct _benchmark {
    thread t;
    int joined; 
    int rc;
    int parent;
    milliseconds elapsed;
    int arg_len;
    char *arg;
} benchmark;

void benchmark_func_inner(char *arg);
void benchmark_func(int bid);
int fetch_available_bid();
void run_benchmark();
void cleanup_benchmark();
void revise_benchmark();
void rerun_benchmark();
void status();