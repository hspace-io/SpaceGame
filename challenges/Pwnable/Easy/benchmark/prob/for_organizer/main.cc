#include <iostream>
#include <cstring>
#include "benchmark.h"

using namespace std;

void init_proc();
void print_menu();
int read_int();

const char *banner =                                                             
 "  _                     _                          _    \n\
 | |__   ___ _ __   ___| |__  _ __ ___   __ _ _ __| | __\n\
 | '_ \\ / _ \\ '_ \\ / __| '_ \\| '_ ` _ \\ / _` | '__| |/ /\n\
 | |_) |  __/ | | | (__| | | | | | | | | (_| | |  |   < \n\
 |_.__/ \\___|_| |_|\\___|_| |_|_| |_| |_|\\__,_|_|  |_|\\_\\\n\n";

int main(int argc, char **argv) {
    init_proc();

    while (1) {
        print_menu();
        switch (read_int()) {
            case 1:
                run_benchmark();
                break;
            case 2:
                cleanup_benchmark();
                break;
            case 3:
                rerun_benchmark();
                break;
            case 4:
                revise_benchmark();
                break;
            case 5:
                status();
                break;
            default:
                cout << "Invalid choice\n" << endl;
        }
    }
}

void init_proc() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    cout << banner;
}

void print_menu() {
    cout << "[1] Run benchmark" << endl;
    cout << "[2] Cleanup benchmark" << endl;
    cout << "[3] Re-run benchmark" << endl;
    cout << "[4] Revise benchmark" << endl;
    cout << "[5] Status" << endl;
    cout << " > ";
}

int read_int() {
    string input;
    cin >> input;

    try {
        return stoi(input);
    } catch (...) {
        return 0;
    }
}