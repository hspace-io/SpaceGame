# 분명 유출됐는데...

# Usage


# Concept
- Digital Forensic
- Filesystem

# Writeup
- WIndows 10 파일 시스템으로 구성된 CREDENTAIL.E01 파일을 열어준다
- - 문서들과 폴더 등이 꽤 있으므로 전수조사는 힘드니 해당 PC를 사용했던 이용자의 인터넷 검색 기록으로 사건 경위나 행위 분석을 먼저 하도록 하겠다.
- 해당 E01 파일에서 유일하게 설치된 인터넷 브라우저인 Edge의 Web History 경로를 들어가준다.
- Web History를 DB SQlite로 확인하고, urls를 들어가게 되면 휴지통에서 확인할 수 있었던 zipzipzipzip.zip이라는 수상한 파일명이 기재된 title과 googledrive라는 클라우드 서비스 url을 확인할 수 있다.
- drivegoogle.com/fifle 이 두 개인데 접속 가능한 것으로 들어가준다.
- https://drive.google.com/file/d/1XCMFJJSycH6PBLknB3C1duAc2pxLWIFy/view로 들어가면 위와 같은 zip 파일이 나오게 된다. 아마도 사용자는 PC에는 문서 파일을 지우고 주요한 기밀이 담긴 문서를 압축파일로 하여서 구글 드라이브에 업로드함으로써 유출한 것 같다.

파일을 다운받아보자.
- zipzipzip.zip이라는 압축 파일을 탐색기에서 확인하면 그 안에 hello.txt(1KB)와 zipzip.zip(51KB) 두 파일이 들어있는 것을 확인할 수 있다. 
- hello.txt를 읽어보면 “Let’s seeee the end of…!!”라는 문구가 적혀있고 정상적으로 잘 열리며 용량도 작은 것을 보아하니 그 외 다른 특이점은 없는 것 같다.
- 반면 zipzipzip.zip 파일 안에는 두 파일이 있는 것을 확인할 수 있지만, 정상적인 방법으로는 압축해제가 되지 않고 복구도 되지 않는 것을 확인할 수 있다.


- HxD 도구로 zipzipzip.zip을 열어보면 다음과 같은 모습을 볼 수 있다. zip 압축 파일의 헤더 시그니처는 50 4B 로, Ctrl+F를 사용하여 16진수 값을 검색해준다.
- zip 파일에 있는 첫 번째 문서는 pdf로, “Renaissance.pdf”라는 파일명을 가지고 있는 것을 확인할 수 있다. 
- 두 번째 문서는 txt로, “sign.txt”라는 메모장 제목임을 의미한다. 


- 압축이 정상적으로 해제되지 않은 zip 파일에서 확인할 수 있었던 문서 개수는 총 2개로, “Renaissance.pdf”와 “sign.txt”외에 다른 문서는 없는 것을 알 수 있다. 방금 문서명을 확인하였을 때, 50 4B 03 04 라는 구조로 확인을 하였다. 이는 zip 압축 파일의 “Local Header”를 의미하는 것이다.
- “sign.txt”라는 문서명을 확인한 다음에는, Offset CAB3 ~ Offset CB26에서 50 4B 01 02 라는 시그니처로 시작하는 부분을 연속적으로 확인할 수 있다. 이는 Central Directory로, 압축된 데이터의 크기와 파일 이름 등을 확인할 수 있는 압축 파일 구조 중 한 부분이다. 그러나 Central Directory의 0x2E부터는 파일 이름이 들어가야하는 부분이지만, 두 문서의 Cetnral Directory에서는 Local Header 부분과 다르게 파일 이름이 지워진 것을 확인할 수 있다. 


- 0x14부터 압축된 파일 사이즈 및 원본 파일 사이즈 등을 확인할 수 있다. 따라서 파일 구조를 분석하였을 때 용량이 더 큰 Central Directory에는 “Renaissance.pdf”의 파일명을, 용량이 더 작은 Central Direcotry에는 “sign.txt” 파일명을 작성하여 손상된 header 부분을 복구해주도록 하겠다. 
- 첫 번째 Central Directory Header 부분을 살펴보면, 0x0000CA35가 압축된 파일 크기를 의미함을 알 수 있다. 이는 10진수로 변환하면 51,765로 pdf의 Central Directory임을 알 수 있다.
- 동일한 방식으로 두 번째 Central Direcotry Header 부분을 살펴보면 0x0000002B로 파일 크기가 43인 sign.txt임을 알 수 있다.
- 따라서 적절한 위치에 파일명을 알맞은 위치에 기입을 해준다.


- 정상적으로 파일명이 나와있으며, 압축이 풀리는 것을 확인할 수 있다.
- sign.txt에는 “~의 끝부분에는 여전히 이상한 것이 있다…”라고 영문으로 적혀있다.
- zipzipzipzip.zip에 들어있던 또다른 문서 파일인 “Renaissance.pdf”에는 영어로 된 시가 쭉 작성되어 있다. 
- 문서는 총 5페이지로, 끝까지 내려봐도 특별한 정황 및 문구는 없었다. HxD로 숨겨진 단서 등이 있는지 확인을 해보자. 
- 확장자인 pdf의 푸터 시그니처인 25 45 4F 46 의 뒷 부분에 문구가 작성되어 있는 것을 보아, sign.txt가 나름의 힌트를 주고 있던 것 같다. zip 파일의 구조 복구만 하였다면 pdf에 숨겨진 flag는 손쉽게 찾을 수 있었다. 


- FLAG : SPACEWAR{@n5w3r_i5_4lw4y5_1n_the_3nd!!!!!}