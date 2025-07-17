# challenge_name
Password Guessing

# Usage
docker compose up --build -d

# Concept
- Hardware
- Side Channel Attack

# Writeup

python 코드와 elf 파일을 주기 때문에, instruction 수 세는게 어느 코드에서부터 시작해서 어느 코드에서 끝나는지 알 수 있습니다.

Instruction 수가 나왔을 때 알 수 있는 정보 : 
몇번의 bit 검사가 이루어졌나. 이유 -> bit 검사에 몇번의 instruction이 사용되는지 알 수 있음.
	
그럼 몇번의 bit 검사가 이루어졌는지로 알 수 있는 정보 :
경우의 수 유추 가능.

아래와 같은 상황일 때, d는 첫 번째 자리입니다. 왜냐하면 190은 첫번째 자리만큼 문자열 검사하고 끝났을 때 instruction 수이기 때문입니다.

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/5e9d3964-8271-4668-a6e3-e4abbb2221ec/83f19b59-16a4-4b99-bb49-0f507faa040e/image.png)

이런 식으로 계속 정답과 오답의 instruction 수를 비교하다보면, 아래와 같은 식을 만들 수 있습니다.

```python
instruction 수 = 
17 * (bit check) + (41 * (글자 인덱스)) - (만약 숫자라면 2 * 글자 위치 아니면 0) + (실패 했다면 47, 성공 했다면 54)
```

이렇게 되면, 주어진 6번의 기회 안에서, 4글자 중 1~2글자를 맞춘다면, 나머지 2글자는 instruction 수를 연산하여, 정답 2글자를 맞출 동안 몇번의 bit 연산이 일어났나 유추해서 알아낼 수 있습니다.

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/5e9d3964-8271-4668-a6e3-e4abbb2221ec/45d33140-e865-4890-9d1d-37f126e32aa0/image.png)

위와 같이 a가 Correct 일 때, 아래와 같이 연산할 수 있습니다.

17 * ? + 41 * 1 + 54 = 95 + (17 * ?) = 95 + (17 * 11) = 282

a는 2번째 글자이고, 그동안 11번의 bit 연산을 진행했다. 이때, 8번의 검사는 a의 것일테니,

a = b0110 0000, 그럼 2번째까지는 00, 3번째 때 틀렸으니, 100

그럼 b0110 0100(d), b0110 1100(l), b0011 0100(4) 중 하나 일 것입니다.

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/5e9d3964-8271-4668-a6e3-e4abbb2221ec/ec8400bd-dd01-48b4-8869-2da2a54cfbaf/image.png)

l이 정답이다. 앞의 두 자리를 알았으면, 이제 틀린 것으로도 정보를 얻을 수 있습니다.

d의 경우 b0110 0100인데, 앞에 두 자리에서 몇 번의 연산을 진행했을지 알 수 있으니, 나머지 뒤 두자리에서 몇 번의 연산이 진행 됐을 지 알 수 있습니다.

이런 방식으로 문제를 풀 수 있습니다.