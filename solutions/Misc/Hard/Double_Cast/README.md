# 문제이름
- Double_Cast

# Usage
- docker compose up --build

# Concept
- Integer Overflow, Js and Python Confusion

# Writeup
- 1번 아공간 주머니 메뉴를 보면 마나 2배, 제곱 포션이 있다.
  EXPLOSION 마법에서 Double Cast를 사용하면 몬스터를 쓰러뜨릴수 있다.
  그리고 몬스터를 쓰러뜨리면 flag를 준다.
  EXPLOSION 마법을 Double Cast로 사용하려면 마나가 2147483648 만큼필요하다.
  nunpy 라는 모듈을 사용하는데 이 모듈은 numpy가 아닌 짝퉁 모듈로 int 32를 return type로 사용해서 오버플로우가 난다.
  또한 CheckMana 함수를 보면 절댓값을 씌우고 return 한다.
  따라서 마나를 -2147483648 로 만들고 CheckMana 함수를 실행하면 마나가 2147483648가 되면서 EXPLOSION을 Double cast로 사용할 수 있다.
  -2147483648를 만들기 위해서는 2번 공격 메뉴에서 마법을 사용해 마나를 깎고 마나 2배, 마나 제곱을 해야한다.
  그 후 공격하기 옵션에 들어가서 EXPLOSION 을 Double Cast로 사용하면 된다.
