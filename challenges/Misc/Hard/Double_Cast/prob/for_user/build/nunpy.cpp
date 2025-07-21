#include "nunpy.h"
#include <algorithm>
using namespace std;

int CheckMana(long long int money, long long int cost) {
    return abs(money)>=cost;
}

int Sub(long long int a, long long int b) {
    return a-b;
}

int Add(long long int a, long long int b) {
    return a+b;
}

int Mul(long long int a, long long int b) {
    return a*b;
}
