# challenge_name
Surprising Painful Infrastructure

# Usage
외부 서버에서 진행

# Concept
SPI 통신을 분석하여 특정한 메모리에 원하는 값을 올리는 과정을 진행야함.

# Writeup
1. Mode 의 분기에 따라서 0→1로 변하는 시점을 특이점으로 보는지, 1 → 0으로 변하는 시점을 특이점으로 보는지 값을 찾아야 한다.
    
2. 초기 설정시에 각각의 slaveA->MISO, slaveB->MISO, slaveC->MISO에는 AES키의 특정 부분이 존재한다. 해당하는 부분을 MASTER 로 옮긴후 읽기 작업이 가능하다.
    
3. 2에서 작업한 KEY내용을 가지고, get_flag를 호출했을때 해당하는 로직을 맞춰주면 된다.