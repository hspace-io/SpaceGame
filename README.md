# SpaceGame
## 사용법
### 구조
```
Spacegame/
├── problems/
│   ├── Pwnable/
│   │   ├── Beginner/
│   │   └── Easy/
│   │
│   ├── Webhacking/
│   │   ├── Beginner/
│   │   └── Hard/
│   │
│   └── Reversing/
│       ├── Medium/
│       └── Hard/
│
└── solutions/
    ├── Pwnable/
    │   ├── Beginner/
    │   └── Easy/
    │
    ├── Webhacking/
    │   ├── Easy/
    │   └── Medium/
    │
    └── Reversing/
        ├── Medium/
        └── Hard/
 
```
### 문제 풀이
- `deploy_select.sh` 실행
- 풀이하고 싶은 문제 번호 선택
- 도커 빌드가 완료되면 각 문제별 challenge.yaml 에 적혀있는 로컬 주소에서 문제 풀이 가능
- 하단의 테이블에서 문제 디렉토리 or 솔루션 디렉토리 이동 가능

### 도커 삭제
- `delete-docker.sh` 실행

## List

|    Category   |  difficulty  |                 problem                  |                                                      Link                                                     |     port     |    server    |     출제일시     |
|---------------|--------------|------------------------------------------|---------------------------------------------------------------------------------------------------------------|--------------|--------------|------------------|
| Webhacking    | Beginner     | flagreading                              | [problem][flagreading prob], [solution][flagreading sol]                                                      | 5678         | o            | 2024년 4월 #1    |
| Webhacking    | Beginner     | jsgood                                   | [problem][jsgood prob] [solution][jsgood sol]                                                                 | x            | X            |                  |
| Webhacking    | Beginner     | Scroll_Master                            | [problem][Scroll_Master prob], [solution][Scroll_Master sol]                                                  | 12312        | o            | 2024년 5월 #1    |
| Webhacking    | Beginner     | soeasy                                   | [problem][soeasy prob] [solution][soeasy sol]                                                                 | o            | 8000         |                  |
| Webhacking    | Easy         | 8_chances                                | [problem][8_chances prob] [solution][8_chances sol]                                                           | o            | 10000        |                  |
| Webhacking    | Easy         | Exec Anything                            | [problem][Exec Anything prob], [solution][Exec Anything sol]                                                  | 8000         | o            | 2024년 5월 #1    |
| Webhacking    | Easy         | HspaceCross                              | [problem][HspaceCross prob] [solution][HspaceCross sol]                                                       | o            | 5000         |                  |
| Webhacking    | Easy         | jelly shop                               | [problem][jelly shop prob], [solution][jelly shop sol]                                                        | 2007         | o            | 2024년 5월 #1    |
| Webhacking    | Easy         | LoX-goblin                               | [problem][LoX-goblin prob], [solution][LoX-goblin sol]                                                        | 22222        | o            | 2024년 5월 #1    |
| Webhacking    | Easy         | RandomSecret                             | [problem][RandomSecret prob], [solution][RandomSecret sol]                                                    | 4000         | o            | 2024년 4월 #1    |
| Webhacking    | Easy         | WhiteSpade                               | [problem][WhiteSpade prob], [solution][WhiteSpade sol]                                                        | 28000        | o            | 2024년 6월 #1    |
| Webhacking    | Medium       | Animal                                   | [problem][Animal prob], [solution][Animal sol]                                                                | 35135        | o            | 2024년 6월 #1    |
| Webhacking    | Medium       | error segfault                           | [problem][error segfault prob] [solution][error segfault sol]                                                 | o            | 2222:80      |                  |
| Webhacking    | Medium       | N7inx                                    | [problem][N7inx prob], [solution][N7inx sol]                                                                  | 80           | o            | 2024년 5월 #1    |
| Webhacking    | Medium       | One\'s blog                              | [problem][One's blog prob] [solution][One's blog sol]                                                         | o            | 3000         |                  |
| Webhacking    | Medium       | photoxss                                 | [problem][photoxss prob], [solution][photoxss sol]                                                            | 3000         | o            | 2024년 4월 #1    |
| Webhacking    | Medium       | Space Casino                             | [problem][Space Casino prob] [solution][Space Casino sol]                                                     | o            | 5000         |                  |
| Webhacking    | Medium       | svelte_api_v2                            | [problem][svelte_api_v2 prob], [solution][svelte_api_v2 sol]                                                  | 49614        | o            | 2024년 5월 #1    |
| Webhacking    | Medium       | yurutube                                 | [problem][yurutube prob] [solution][yurutube sol]                                                             | o            | 44444        |                  |
| Webhacking    | Hard         | hello_proxy                              | [problem][hello_proxy prob], [solution][hello_proxy sol]                                                      | 49621        | o            | 2024년 5월 #1    |
| Webhacking    | Hard         | LoX-Zombie                               | [problem][LoX-Zombie prob], [solution][LoX-Zombie sol]                                                        | 44444        | o            | 2024년 5월 #1    |
| Webhacking    | Hard         | Spiderman                                | [problem][Spiderman prob] [solution][Spiderman sol]                                                           | o            | 44444        |                  |
| Pwnable       | Beginner     | bxorf                                    | [problem][bxorf prob], [solution][bxorf sol]                                                                  | 12345        | o            | 2025년 1월       |
| Pwnable       | Beginner     | cli-painter                              | [problem][cli-painter prob], [solution][cli-painter sol]                                                      | 39393        | o            | 2024년 4월 #1    |
| Pwnable       | Beginner     | only_takes_one_bullet                    | [problem][only_takes_one_bullet prob], [solution][only_takes_one_bullet sol]                                  | 8888         | o            | 2024년 5월 #2    |
| Pwnable       | Beginner     | wasd_space                               | [problem][wasd_space prob], [solution][wasd_space sol]                                                        | 6789         | o            | 2024년 5월 #2    |
| Pwnable       | Easy         | babykernel                               | [problem][babykernel prob] [solution][babykernel sol]                                                         | o            | 55555        |                  |
| Pwnable       | Easy         | benchmark                                | [problem][benchmark prob], [solution][benchmark sol]                                                          | 30030        | o            | 2024년 5월 #2    |
| Pwnable       | Easy         | cRPC v1                                  | [problem][cRPC v1 prob], [solution][cRPC v1 sol]                                                              | 18181        | o            | 2024년 5월 #2    |
| Pwnable       | Easy         | Hspace Babynote                          | [problem][Hspace Babynote prob], [solution][Hspace Babynote sol]                                              | 11111        | o            | 2024 HCTF        |
| Pwnable       | Easy         | simple_note                              | [problem][simple_note prob] [solution][simple_note sol]                                                       | o            | 15252        |                  |
| Pwnable       | Easy         | simple_note_hm                           | [problem][simple_note_hm prob] [solution][simple_note_hm sol]                                                 | o            | 15252        | HM               |
| Pwnable       | Easy         | tinyerror                                | [problem][tinyerror prob], [solution][tinyerror sol]                                                          | 11115        | o            | 2025년 1월       |
| Pwnable       | Easy         | uaf_basic                                | [problem][uaf_basic prob] [solution][uaf_basic sol]                                                           | o            | 4021         |                  |
| Pwnable       | Easy         | ZeroSuperTigerDomain                     | [problem][ZeroSuperTigerDomain prob] [solution][ZeroSuperTigerDomain sol]                                     | o            | 15252        | (working title)  |
| Pwnable       | Hard         | chatgpt_evm                              | [problem][chatgpt_evm prob], [solution][chatgpt_evm sol]                                                      | 19191        | o            | 2024년 5월 #2    |
| Pwnable       | Hard         | hspaceETH                                | [problem][hspaceETH prob], [solution][hspaceETH sol]                                                          | 30031        | o            | 2024년 5월 #2    |
| Pwnable       | Hard         | lottery                                  | [problem][lottery prob], [solution][lottery sol]                                                              | 15252, 15353 | o            | 2024년 4월 #1    |
| Pwnable       | Hard         | system_vuln                              | [problem][system_vuln prob], [solution][system_vuln sol]                                                      | 15252        | o            | 2024년 6월 #1    |
| Pwnable       | Medium       | Antivirus Sandbox                        | [problem][Antivirus Sandbox prob], [solution][Antivirus Sandbox sol]                                          | 11111        | o            | 2024년 6월 #1    |
| Pwnable       | Medium       | ez_vm                                    | [problem][ez_vm prob], [solution][ez_vm sol]                                                                  | 31337        | o            | 2025년 1월       |
| Pwnable       | Medium       | Hspace Note                              | [problem][Hspace Note prob], [solution][Hspace Note sol]                                                      | 11112        | o            | 2024 HCTF        |
| Pwnable       | Medium       | only one                                 | [problem][only one prob], [solution][only one sol]                                                            | 31337        | o            | 2024년 4월 #1    |
| Pwnable       | Medium       | open_read_write                          | [problem][open_read_write prob], [solution][open_read_write sol]                                              | 10101        | o            | 2024년 5월 #2    |
| Pwnable       | Medium       | p-shell                                  | [problem][p-shell prob] [solution][p-shell sol]                                                               | o            | 18775        |                  |
| Pwnable       | Medium       | perfect machine                          | [problem][perfect machine prob], [solution][perfect machine sol]                                              | 15252        | o            | 2025년 1월       |
| Pwnable       | Medium       | pwn101                                   | [problem][pwn101 prob] [solution][pwn101 sol]                                                                 | o            | 8080         |                  |
| Pwnable       | Medium       | secret file                              | [problem][secret file prob], [solution][secret file sol]                                                      | 15252        | o            | 2024년 5월 #2    |
| Reversing     | Beginner     | Ah...?                                   | [problem][Ah...? prob] [solution][Ah...? sol]                                                                 | 2346         | x            | 2024년 6월 #1    |
| Reversing     | Beginner     | serial                                   | [problem][serial prob] [solution][serial sol]                                                                 | x            | .            |                  |
| Reversing     | Beginner     | vuln_func                                | [problem][vuln_func prob] [solution][vuln_func sol]                                                           | x            | 12345        |                  |
| Reversing     | Easy         | find_answer                              | [problem][find_answer prob] [solution][find_answer sol]                                                       | x            | 23456        |                  |
| Reversing     | Easy         | My VM                                    | [problem][My VM prob] [solution][My VM sol]                                                                   | x            | 서버 X       |                  |
| Reversing     | Easy         | permutation                              | [problem][permutation prob] [solution][permutation sol]                                                       | x            |              |                  |
| Reversing     | Easy         | 아크                                     | [problem][아크 prob] [solution][아크 sol]                                                                     | x            | x            |                  |
| Reversing     | Hard         | Surprising Painful Infrastructure        | [problem][Surprising Painful Infrastructure prob] [solution][Surprising Painful Infrastructure sol]           | o            | 5678         |                  |
| Reversing     | Hard         | Verwandeltes Labyrinth                   | [problem][Verwandeltes Labyrinth prob] [solution][Verwandeltes Labyrinth sol]                                 | x            | X            | 2024년 4월 #1    |
| Reversing     | Medium       | Catnip                                   | [problem][Catnip prob] [solution][Catnip sol]                                                                 | x            | -            |                  |
| Reversing     | Medium       | Chip VM                                  | [problem][Chip VM prob] [solution][Chip VM sol]                                                               |              | X            | 2024년 4월 #1    |
| Reversing     | Medium       | ObfuSWF                                  | [problem][ObfuSWF prob] [solution][ObfuSWF sol]                                                               | x            | -            |                  |
| Reversing     | Medium       | sigvm                                    | [problem][sigvm prob] [solution][sigvm sol]                                                                   |              | X            | 2024년 4월 #1    |
| Reversing     | Medium       | 내일 할 일                               | [problem][내일 할 일 prob] [solution][내일 할 일 sol]                                                         | x            | x            |                  |
| Cryptography  | Beginner     | quadratic function                       | [problem][quadratic function prob] [solution][quadratic function sol]                                         | x            | X            | 2024년 4월 #1    |
| Cryptography  | Easy         | NAEDILCUE                                | [problem][NAEDILCUE prob] [solution][NAEDILCUE sol]                                                           | x            | null         | 2024년 4월 #1    |
| Cryptography  | Easy         | GP                                       | [problem][GP prob] [solution][GP sol]                                                                         | x            | X            | 2024년 6월 #1    |
| Cryptography  | Easy         | Weird dhke                               | [problem][Weird dhke prob] [solution][Weird dhke sol]                                                         | o            | 54321        |                  |
| Cryptography  | Medium       | SquRSA_xx                                | [problem][SquRSA_xx prob] [solution][SquRSA_xx sol]                                                           | x            | null         | 2024년 6월 #1    |
| Cryptography  | Medium       | BabyHash                                 | [problem][BabyHash prob] [solution][BabyHash sol]                                                             | o            | 1971         |                  |
| Cryptography  | Medium, Hard | SquRSA_xx                                | [problem][SquRSA_xx_H prob] [solution][SquRSA_xx_H sol]                                                       | x            | null         | 2024년 4월 #1    |
| Crypto + misc | Medium       | simple some/ip                           | [problem][simple some/ip prob] [solution][simple some/ip sol]                                                 | x            | X            |                  |
| Misc          | Easy         | READ (Read, Extract, Analyze, Datasheet) | [problem][READ prob] [solution][READ sol]                                                                     | o            | 21345, 21346 | 2024 PhyiscalLAB |
| Misc          | Medium       | Mine Circuit                             | [problem][Mine Circuit prob] [solution][Mine Circuit sol]                                                     | x            | x            |                  |
| Misc          | Medium       | Mr.11100101101111110001011011111110100   | [problem][Mr.11100101101111110001011011111110100 prob] [solution][Mr.11100101101111110001011011111110100 sol] | o            | 4997         |                  |
| Misc          | Easy         | Steins;Password                          | [problem][Steins;Password prob] [solution][Steins;Password sol]                                               | x            | 0            |                  |
| Misc          | Hard         | Double_Cast                              | [problem][Double_Cast prob] [solution][Double_Cast sol]                                                       | o            | 44444:1337   |                  |
| 1-day         | Easy         | 1-day_cve                                | [problem][1-day_cve prob] [solution][1-day_cve sol]                                                           | x            | none         |                  |
| 1-day         | Easy         | apt_report                               | [problem][apt_report prob] [solution][apt_report sol]                                                         | x            | none         |                  |
| Forensic      | Beginner     | Get-Flag-From-Space                      | [problem][Get-Flag-From-Space prob] [solution][Get-Flag-From-Space sol]                                       | x            | None         |                  |
| Forensic      | Easy         | Internal Audit 1                         | [problem][Internal Audit 1 prob] [solution][Internal Audit 1 sol]                                             | x            | x            |                  |
| Forensic      | Easy         | Internal Audit 2                         | [problem][Internal Audit 2 prob] [solution][Internal Audit 2 sol]                                             | x            | x            |                  |
| Forensic      | Easy         | Internal Audit 3                         | [problem][Internal Audit 3 prob] [solution][Internal Audit 3 sol]                                             | x            | x            |                  |
| Forensic      | Easy         | Internal Audit 4                         | [problem][Internal Audit 4 prob] [solution][Internal Audit 4 sol]                                             | x            | x            |                  |
| Forensic      | Easy         | Internal Audit 5                         | [problem][Internal Audit 5 prob] [solution][Internal Audit 5 sol]                                             | x            | x            |                  |
| Forensic      | Beginner     | 컴퓨터가 이상해                          | [problem][컴퓨터가 이상해 prob] [solution][컴퓨터가 이상해 sol]                                               | x            | X            |                  |
| Forensic      | Medium       | 분명 유출됐는데...                       | [problem][분명 유출됐는데... prob] [solution][분명 유출됐는데... sol]                                         | x            | X            |                  |
| Forensic      | Hard         | 민간 포렌식 기업 퇴사 사건               | [problem][민간 포렌식 기업 퇴사 사건 prob] [solution][민간 포렌식 기업 퇴사 사건 sol]                         | x            | X            |                  |
| Forensic      | Medium       | C국에게 받은 비밀지령                    | [problem][C국에게 받은 비밀지령 prob] [solution][C국에게 받은 비밀지령 sol]                                   | x            | X            |                  |
<!-- Webhacking -->

[flagreading prob]: ./problems/Webhacking/Beginner/flagreading
[flagreading sol]: ./solutions/Webhacking/Beginner/flagreading

[Scroll_Master prob]: ./problems/Webhacking/Beginner/Scroll_Master
[Scroll_Master sol]: ./solutions/Webhacking/Beginner/Scroll_Master

[soeasy prob]: ./problems/Webhacking/Beginner/soeasy
[soeasy sol]: ./solutions/Webhacking/Beginner/soeasy

[jsgood prob]: ./problems/Webhacking/Beginner/jsgood
[jsgood sol]: ./solutions/Webhacking/Beginner/jsgood

[Exec Anything prob]: ./problems/Webhacking/Easy/Exec_Anything
[Exec Anything sol]: ./solutions/Webhacking/Easy/Exec_Anything

[jelly shop prob]: ./problems/Webhacking/Easy/jelly_shop
[jelly shop sol]: ./solutions/Webhacking/Easy/jelly_shop

[LoX-goblin prob]: ./problems/Webhacking/Easy/LoX-goblin
[LoX-goblin sol]: ./solutions/Webhacking/Easy/LoX-goblin

[RandomSecret prob]: ./problems/Webhacking/Easy/RandomSecret
[RandomSecret sol]: ./solutions/Webhacking/Easy/RandomSecret

[WhiteSpade prob]: ./problems/Webhacking/Easy/WhiteSpade
[WhiteSpade sol]: ./solutions/Webhacking/Easy/WhiteSpade

[hello_proxy prob]: ./problems/Webhacking/Hard/hello_proxy
[hello_proxy sol]: ./solutions/Webhacking/Hard/hello_proxy

[LoX-Zombie prob]: ./problems/Webhacking/Hard/LoX-Zombie
[LoX-Zombie sol]: ./solutions/Webhacking/Hard/LoX-Zombie

[Animal prob]: ./problems/Webhacking/Medium/Animal
[Animal sol]: ./solutions/Webhacking/Medium/Animal

[N7inx prob]: ./problems/Webhacking/Medium/N7inx
[N7inx sol]: ./solutions/Webhacking/Medium/N7inx

[photoxss prob]: ./problems/Webhacking/Medium/photoxss
[photoxss sol]: ./solutions/Webhacking/Medium/photoxss

[svelte_api_v2 prob]: ./problems/Webhacking/Medium/svelte_api_v2
[svelte_api_v2 sol]: ./solutions/Webhacking/Medium/svelte_api_v2

[Space Casino prob]: ./problems/Webhacking/Medium/Space_Casino
[Space Casino sol]: ./solutions/Webhacking/Medium/Space_Casino

[yurutube prob]: ./problems/Webhacking/Medium/yurutube
[yurutube sol]: ./solutions/Webhacking/Medium/yurutube

[error segfault prob]: ./problems/Webhacking/Medium/error_segfault
[error segfault sol]: ./solutions/Webhacking/Medium/error_segfault

[Spiderman prob]: ./problems/Webhacking/Hard/Spiderman
[Spiderman sol]: ./solutions/Webhacking/Hard/Spiderman

[8_chances prob]: ./problems/Webhacking/Easy/8_chances
[8_chances sol]: ./solutions/Webhacking/Easy/8_chances

[HspaceCross prob]: ./problems/Webhacking/Easy/HspaceCross
[HspaceCross sol]: ./solutions/Webhacking/Easy/HspaceCross

[One's blog prob]: ./problems/Webhacking/Medium/One\'s_blog
[One's blog sol]: ./solutions/Webhacking/Medium/One\'s_blog

<!-- Pwnable -->
[bxorf prob]: ./problems/Pwnable/Beginner/bxorf
[bxorf sol]: ./solutions/Pwnable/Beginner/bxorf

[cli-painter prob]: ./problems/Pwnable/Beginner/cli-painter
[cli-painter sol]: ./solutions/Pwnable/Beginner/cli-painter

[only_takes_one_bullet prob]: ./problems/Pwnable/Beginner/only_takes_one_bullet
[only_takes_one_bullet sol]: ./solutions/Pwnable/Beginner/only_takes_one_bullet

[wasd_space prob]: ./problems/Pwnable/Beginner/wasd_space
[wasd_space sol]: ./solutions/Pwnable/Beginner/wasd_space

[benchmark prob]: ./problems/Pwnable/Easy/benchmark
[benchmark sol]: ./solutions/Pwnable/Easy/benchmark

[cRPC v1 prob]: ./problems/Pwnable/Easy/cRPC_v1
[cRPC v1 sol]: ./solutions/Pwnable/Easy/cRPC_v1

[Hspace Babynote prob]: ./problems/Pwnable/Easy/Hspace_Babynote
[Hspace Babynote sol]: ./solutions/Pwnable/Easy/Hspace_Babynote

[tinyerror prob]: ./problems/Pwnable/Easy/tinyerror
[tinyerror sol]: ./solutions/Pwnable/Easy/tinyerror

[simple_note prob]: ./problems/Pwnable/Easy/simple_note
[simple_note sol]: ./solutions/Pwnable/Easy/simple_note

[Antivirus Sandbox prob]: ./problems/Pwnable/Medium/Antivirus_Sandbox
[Antivirus Sandbox sol]: ./solutions/Pwnable/Medium/Antivirus_Sandbox

[ez_vm prob]: ./problems/Pwnable/Medium/ez_vm
[ez_vm sol]: ./solutions/Pwnable/Medium/ez_vm

[Hspace Note prob]: ./problems/Pwnable/Medium/Hspace_Note
[Hspace Note sol]: ./solutions/Pwnable/Medium/Hspace_Note

[only one prob]: ./problems/Pwnable/Medium/only_one
[only one sol]: ./solutions/Pwnable/Medium/only_one

[open_read_write prob]: ./problems/Pwnable/Medium/open_read_write
[open_read_write sol]: ./solutions/Pwnable/Medium/open_read_write

[perfect machine prob]: ./problems/Pwnable/Medium/perfect_machine
[perfect machine sol]: ./solutions/Pwnable/Medium/perfect_machine

[secret file prob]: ./problems/Pwnable/Medium/secret_file
[secret file sol]: ./solutions/Pwnable/Medium/secret_file

[chatgpt_evm prob]: ./problems/Pwnable/Hard/chatgpt_evm
[chatgpt_evm sol]: ./solutions/Pwnable/Hard/chatgpt_evm

[hspaceETH prob]: ./problems/Pwnable/Hard/hspaceETH
[hspaceETH sol]: ./solutions/Pwnable/Hard/hspaceETH

[lottery prob]: ./problems/Pwnable/Hard/lottery
[lottery sol]: ./solutions/Pwnable/Hard/lottery

[system_vuln prob]: ./problems/Pwnable/Hard/system_vuln
[system_vuln sol]: ./solutions/Pwnable/Hard/system_vuln


[uaf_basic prob]: ./problems/Pwnable/Easy/uaf_basic
[uaf_basic sol]: ./solutions/Pwnable/Easy/uaf_basic

[simple_note_hm prob]: ./problems/Pwnable/Easy/simple_note_hm
[simple_note_hm sol]: ./solutions/Pwnable/Easy/simple_note_hm

[babykernel prob]: ./problems/Pwnable/Easy/babykernel
[babykernel sol]: ./solutions/Pwnable/Easy/babykernel

[ZeroSuperTigerDomain prob]: ./problems/Pwnable/Easy/ZeroSuperTigerDomain
[ZeroSuperTigerDomain sol]: ./solutions/Pwnable/Easy/ZeroSuperTigerDomain

[pwn101 prob]: ./problems/Pwnable/Medium/pwn101
[pwn101 sol]: ./solutions/Pwnable/Medium/pwn101

[p-shell prob]: ./problems/Pwnable/Medium/p-shell
[p-shell sol]: ./solutions/Pwnable/Medium/p-shell



<!-- Reversing -->

[Ah...? prob]: ./problems/Reversing/Beginner/ah,,,?
[Ah...? sol]: ./solutions/Reversing/Beginner/ah,,,?
<!-- . -> , -->

[sigvm prob]: ./problems/Reversing/Medium/sigvm
[sigvm sol]: ./solutions/Reversing/Medium/sigvm

[Chip VM prob]: ./problems/Reversing/Medium/Chip_VM
[Chip VM sol]: ./solutions/Reversing/Medium/Chip_VM

[Verwandeltes Labyrinth prob]: ./problems/Reversing/Hard/Verwandeltes_Labyrinth
[Verwandeltes Labyrinth sol]: ./solutions/Reversing/Hard/Verwandeltes_Labyrinth

[find_answer prob]: ./problems/Reversing/Easy/find_answer
[find_answer sol]: ./solutions/Reversing/Easy/find_answer

[vuln_func prob]: ./problems/Reversing/Beginner/vuln_func
[vuln_func sol]: ./solutions/Reversing/Beginner/vuln_func

[Surprising Painful Infrastructure prob]: ./problems/Reversing/Hard/Surprising_Painful_Infrastructure
[Surprising Painful Infrastructure sol]: ./solutions/Reversing/Hard/Surprising_Painful_Infrastructure

[Catnip prob]: ./problems/Reversing/Medium/Catnip
[Catnip sol]: ./solutions/Reversing/Medium/Catnip

[My VM prob]: ./problems/Reversing/Easy/My_VM
[My VM sol]: ./solutions/Reversing/Easy/My_VM

[내일 할 일 prob]: ./problems/Reversing/Medium/내일_할_일
[내일 할 일 sol]: ./solutions/Reversing/Medium/내일_할_일

[ObfuSWF prob]: ./problems/Reversing/Medium/ObfuSWF
[ObfuSWF sol]: ./solutions/Reversing/Medium/ObfuSWF

[serial prob]: ./problems/Reversing/Beginner/serial
[serial sol]: ./solutions/Reversing/Beginner/serial

[permutation prob]: ./problems/Reversing/Easy/permutation
[permutation sol]: ./solutions/Reversing/Easy/permutation

[아크 prob]: ./problems/Reversing/Easy/아크
[아크 sol]: ./solutions/Reversing/Easy/아크


<!-- Cryptography -->
[quadratic function prob]: ./problems/Cryptography/Beginner/quadratic_function
[quadratic function sol]: ./solutions/Cryptography/Beginner/quadratic_function

[NAEDILCUE prob]: ./problems/Cryptography/Easy/NAEDILCUE
[NAEDILCUE sol]: ./solutions/Cryptography/Easy/NAEDILCUE

[GP prob]: ./problems/Cryptography/Easy/GP
[GP sol]: ./solutions/Cryptography/Easy/GP

[SquRSA_xx prob]: ./problems/Cryptography/Medium/SquRSA_xx
[SquRSA_xx sol]: ./solutions/Cryptography/Medium/SquRSA_xx

[SquRSA_xx_H prob]: ./problems/Cryptography/Hard/SquRSA_xx
[SquRSA_xx_H sol]: ./solutions/Cryptography/Hard/SquRSA_xx

[BabyHash prob]: ./problems/Cryptography/Medium/BabyHash
[BabyHash sol]: ./solutions/Cryptography/Medium/BabyHash

[Weird dhke prob]: ./problems/Cryptography/Easy/Weird_dhke
[Weird dhke sol]: ./solutions/Cryptography/Easy/Weird_dhke

<!-- Misc -->
[READ prob]: ./problems/Misc/Easy/READ
[READ sol]: ./solutions/Misc/Easy/READ

[Mine Circuit prob]: ./problems/Misc/Medium/Mine_Circuit
[Mine Circuit sol]: ./solutions/Misc/Medium/Mine_Circuit

[Mr.11100101101111110001011011111110100 prob]: ./problems/Misc/Medium/Mr.11100101101111110001011011111110100
[Mr.11100101101111110001011011111110100 sol]: ./solutions/Misc/Medium/Mr.11100101101111110001011011111110100

[Steins;Password prob]: ./problems/Misc/Easy/Steins;Password
[Steins;Password sol]: ./solutions/Misc/Easy/Steins;Password

[Double_Cast prob]: ./problems/Misc/Hard/Double_Cast
[Double_Cast sol]: ./solutions/Misc/Hard/Double_Cast

<!-- 1-day -->
[1-day_cve prob]: ./problems/1-day/Easy/1-day_cve
[1-day_cve sol]: ./solutions/1-day/Easy/1-day_cve

[apt_report prob]: ./problems/1-day/Easy/apt_report
[apt_report sol]: ./solutions/1-day/Easy/apt_report

<!-- Forensic -->
[Internal Audit 1 prob]: ./problems/Forensic/Easy/Internal_Audit_1
[Internal Audit 1 sol]: ./solutions/Forensic/Easy/Internal_Audit_1

[Internal Audit 2 prob]: ./problems/Forensic/Easy/Internal_Audit_2
[Internal Audit 2 sol]: ./solutions/Forensic/Easy/Internal_Audit_2

[Internal Audit 3 prob]: ./problems/Forensic/Easy/Internal_Audit_3
[Internal Audit 3 sol]: ./solutions/Forensic/Easy/Internal_Audit_3

[Internal Audit 4 prob]: ./problems/Forensic/Easy/Internal_Audit_4
[Internal Audit 4 sol]: ./solutions/Forensic/Easy/Internal_Audit_4

[Internal Audit 5 prob]: ./problems/Forensic/Easy/Internal_Audit_5
[Internal Audit 5 sol]: ./solutions/Forensic/Easy/Internal_Audit_5

[컴퓨터가 이상해 prob]: ./problems/Forensic/Beginner/컴퓨터가_이상해
[컴퓨터가 이상해 sol]: ./solutions/Forensic/Beginner/컴퓨터가_이상해

[분명 유출됐는데... prob]: ./problems/Forensic/Medium/분명_유출됐는데,,,
[분명 유출됐는데... sol]: ./solutions/Forensic/Medium/분명_유출됐는데,,,

[민간 포렌식 기업 퇴사 사건 prob]: ./problems/Forensic/Hard/민간_포렌식_기업_퇴사_사건
[민간 포렌식 기업 퇴사 사건 sol]: ./solutions/Forensic/Hard/민간_포렌식_기업_퇴사_사건

[C국에게 받은 비밀지령 prob]: ./problems/Forensic/Medium/C국에게_받은_비밀지령
[C국에게 받은 비밀지령 sol]: ./solutions/Forensic/Medium/C국에게_받은_비밀지령

[Get-Flag-From-Space prob]: ./problems/Forensic/Beginner/Get-Flag-From-Space
[Get-Flag-From-Space sol]: ./solutions/Forensic/Beginner/Get-Flag-From-Space


<!-- google -->

<!-- Hardware -->
[PhysicalLab-Whisperer prob]: ./problems/Hardware/Medium/PhysicalLab-Whisperer
[PhysicalLab-Whisperer sol]: ./solutions/Hardware/Medium/PhysicalLab-Whisperer

[Password Guessing prob]: ./problems/Hardware/Hard/Password_Guessing
[Password Guessing sol]: ./solutions/Hardware/Hard/Password_Guessing

[u art? prob]: ./problems/hardware/Beginner/u_art?
[u art? sol]: ./solutions/hardware/Beginner/u_art?
<!-- 이거 포너 -->




[asdf prob]: http://google.com
[asdf sol]: http://google.com


