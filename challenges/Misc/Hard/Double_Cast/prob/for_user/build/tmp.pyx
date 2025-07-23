cdef extern from "nunpy.h":
    int CheckMana(long long int money, long long int cost)
    int Sub(int a, int b)
    int Add(int a, int b)
    int Mul(int a, int b)

def checkmana(long long int a, long long int b) :
    return CheckMana(a, b)

def sub(long long int a, long long int b) :
    return Sub(a, b)

def add(long long int a, long long int b) :
    return Add(a, b)

def mul(long long int a, long long int b) :
    return Mul(a, b)
