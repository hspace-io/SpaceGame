from sage.all import *

flags = "hspace{????????????????????????????????????????????????}"
remain_mod = [ord(flag) for flag in flags]
remain = []
MOD = 127

x=var('x')
equation = remain_mod[-1]*x==remain_mod[-2]
solution = int(solve_mod(equation,MOD)[0][0])
remain.append(remain_mod[-1])
remain.append(remain_mod[-1]*solution)

lens = len(remain_mod)
for i in range(lens-2):
    equation = remain_mod[lens-i-1]+remain_mod[lens-i-2]*x==remain_mod[lens-i-3]
    solution = int(solve_mod(equation,MOD)[0][0])
    remain.append(remain[i]+remain[i+1]*solution)

print(f"a = {remain[-1]}")
print(f"b = {remain[-2]}")