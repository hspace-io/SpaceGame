from Crypto.Util.number import *
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from param import a, c, m, flag

class LCG:
    def __init__(self, seed):
        self.seed = seed
        self.a = a
        self.c = c   
        self.m = m
    
    def next(self):
        self.seed = (self.seed * self.a + self.c) % self.m
        return self.seed

seed = getRandomNBitInteger(512)

lcg = LCG(seed)

L1 = sum([lcg.next() for _ in range(64)])

for _ in range(2**24):
    lcg.next()

L2 = sum([lcg.next() for _ in range(1024)])

key = long_to_bytes(lcg.next())[:16]
iv = long_to_bytes(lcg.next())[:16]
cipher = AES.new(key, AES.MODE_CBC, iv)
ct = cipher.encrypt(pad(flag, 16)).hex()

print(f"{L1 = }")
print(f"{L2 = }")
print(f"{a = }")
print(f"{m = }")
print(f"{ct = }")

'''
L1 = 241567042824877810149769698010725234571863087300429886493467736591361830402407989730389929582138289880779056704034574102349581918390860585364250814597813960
L2 = 3560137977650990350876385972138118155455685879669462248067961580950561005225305730339166933577187993028588680659978359635864187949266834759036352171521759042
a = 8650959439946381492696375959638088201472847309265986883283455960918508724347371879384698748638212778309244168449280117140463540177485356687055154864923643
m = 6930282648600799403864127663891377825098185546201951026476666876233245652027885632534388198412110094917599730573972452921155406474974881990789963999228667
ct = '1a2b11f60ecc01a0620caf25e9e9a277efa0bbafd5a7c15f3cee09aef89bd1c533e01b8215c3ac1574fc90cee9bdcf7ea97cc166cad68eb12db4b5ee328a2ba2'
'''