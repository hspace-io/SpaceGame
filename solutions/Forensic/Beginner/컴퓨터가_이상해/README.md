# 컴퓨터가 이상해

# Usage

# Concept
- TI
- IR

# Writeup
G씨는 평소와 같이 컴퓨터를 이용하던중, 갑작스럽게 이상한 창이 떴다가 사라지는 등의 이상 현상을 발견하게 된다. 
해당 PC를 조사해서 1. 공격자가 사용하는 PC의 이름은 무엇인지, 2. 공격자의 IP는 무엇인지, 3. 공격자의 유입경로는 어디인지를 알아내야 한다.
1, 2, 3의 정답을 언더바(_)로 구분해 인증


**공격자가 사용하는 PC이름**
제공된 덤프에는 가상화폐 지갑으로 위장한 wallet.exe 파일을 다운로드 폴더에서 확인할 수 있다. 해당 파일은 커스텀 rat 클라이언트 프로그램으로서, 직접 빌드 했기 때문에 PDB Path를 확인할 수 있다. 
PEView 등의 프로그램을 통해 .text 섹션의 IMAGE_DEBUG_TYPE_CODEVIEW를 살펴보면 C:\Users\jjanghacker\Downloads~ 와 같은 디렉터리를 확인할 수 있다.
(jjanghacker)

**공격자의 IP 주소**
wallet.exe 프로그램을 리버싱 하거나, 문자열 검색을 통해 찾을 수 있다.
(172.30.1.63)

**공격자의 유입경로**
피싱 이메일을 통해 유입된 것을 아티팩트 분석을 통해 알 수 있다. 웹 히스토리에서 이메일에 접근한 기록과 함께 윈도우 자체 이메일 프로그램에서도 해당 아티팩트를 확인할 수 있다. 해당 피싱 이메일을 전송한 사용자 이름을 확인한다. 
(divorcee_napping947@simplelogin.com)

**Flag**
SPACEWAR{jjanghacker_172.30.1.63_divorcee_napping947@simplelogin.com}