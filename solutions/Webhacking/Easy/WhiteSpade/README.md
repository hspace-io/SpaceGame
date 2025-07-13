# 문제이름
WhiteSpade

# Usage
- 

# Concept
- Command injection, 공백(White Space) 우회, 와일드카드 문자 사용

# Writeup
필터링을 우회하여 Command injection을 수행해야 하는 문제이다.  
  
입력란에 한 자씩, 또는 한 단어씩 입력해보면, 모든 공백문자(White Space)와 cat, flag, '*', '/', '=', '-'가 필터링 되어 있는 것을 확인할 수 있다.  
ls 명령어를 통해 flag.txt가 있는 것을 확인할 수 있으므로, cat 명령어와 공백문자의 사용 없이 flag.txt를 출력할 방법을 찾아야 한다.  
  
`fold${IFS}fla?.txt`  
위 명령어를 입력해 플래그를 획득할 수 있다.  

https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/Command%20Injection#filter-bypasses 참고