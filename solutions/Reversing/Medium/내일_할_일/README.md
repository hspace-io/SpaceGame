# 내일 할 일

# Usage

# Concept

- bruteforce, md5, lcg, aes, base128

# Writeup

1 바이트 랜덤 시드를 사용하여 2개의 LCG의 시드로 사용한다.

이를 이용해서 base128의 테이블을 섞고 AES의 키를 생성한다

이후 AES로 입력을 암호화 하고 base128로 인코딩하여 출력한다.

때문에 해당 루틴들을 리버싱하여 시드를 브루트포스하여 플래그가 나올 때 까지 돌리면된다.