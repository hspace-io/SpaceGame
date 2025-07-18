# BabyHash

# Usage
- 

# Concept
- Hash
- Rainbow Table
- Bruteforce

# Writeup

남길 문자를 고를 수 있음

1. 1개의 문자만 남김
마지막 블럭은 (BSIZE+1) 가지의 경우의 수가 있음.
충분히 작으므로 bruteforce 가능.
-> 각 문자가 몇개 있는지 계산 가능

2. 2개의 문자만 남김
각 블럭은 2**BSIZE 가지의 경우의 수가 있음.
충분히 작으므로 bruteforce 가능. 출제자 개인 PC 기준 한 블럭의 rainbow table을 만드는 데에 약 4분 소요.
또한, 마지막 블럭의 경우 2**((두 문자 개수의 합)%BSIZE) 가지의 경우의 수가 있음. 이는 2**BSIZE에 비해 매우 작으므로 negligible함
16C2 = 120이므로 이를 120번 반복하면 두 문자만 남기는 모든 경우의 해시 역산이 가능함.
8스레드 기준 약 1시간에 가능.

3. message 복구
message의 첫 바이트는 다른 모든 값과의 비교에서 맨 앞에 위치함.
이를 이용하여 첫 바이트를 구한 후 모든 결과에서 이를 제거하는 방식으로 message의 prefix를 구할 수 있음.
이를 반복하여 message 전체를 복구.