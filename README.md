# SpaceGame
## 사용법
### 구조
```
Spacegame/
├── challenges/
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
| Webhacking    | Beginner     | flagreading                              | [challenges][flagreading prob], [solution][flagreading sol]                                                      | 5678         | o            | 2024년 4월 #1    |
| Webhacking    | Beginner     | jsgood                                   | [challenges][jsgood prob] [solution][jsgood sol]                                                                 | x            | X            |                  |
| Webhacking    | Beginner     | Scroll_Master                            | [challenges][Scroll_Master prob], [solution][Scroll_Master sol]                                                  | 12312        | o            | 2024년 5월 #1    |
| Webhacking    | Beginner     | soeasy                                   | [challenges][soeasy prob] [solution][soeasy sol]                                                                 | o            | 8000         |                  |
| Webhacking    | Easy         | 8_chances                                | [challenges][8_chances prob] [solution][8_chances sol]                                                           | o            | 10000        |                  |
| Webhacking    | Easy         | Exec Anything                            | [challenges][Exec Anything prob], [solution][Exec Anything sol]                                                  | 8000         | o            | 2024년 5월 #1    |
| Webhacking    | Easy         | HspaceCross                              | [challenges][HspaceCross prob] [solution][HspaceCross sol]                                                       | o            | 5000         |                  |
| Webhacking    | Easy         | jelly shop                               | [challenges][jelly shop prob], [solution][jelly shop sol]                                                        | 2007         | o            | 2024년 5월 #1    |
| Webhacking    | Easy         | LoX-goblin                               | [challenges][LoX-goblin prob], [solution][LoX-goblin sol]                                                        | 22222        | o            | 2024년 5월 #1    |
| Webhacking    | Easy         | RandomSecret                             | [challenges][RandomSecret prob], [solution][RandomSecret sol]                                                    | 4000         | o            | 2024년 4월 #1    |
| Webhacking    | Easy         | WhiteSpade                               | [challenges][WhiteSpade prob], [solution][WhiteSpade sol]                                                        | 28000        | o            | 2024년 6월 #1    |
| Webhacking    | Medium       | Animal                                   | [challenges][Animal prob], [solution][Animal sol]                                                                | 35135        | o            | 2024년 6월 #1    |
| Webhacking    | Medium       | error segfault                           | [challenges][error segfault prob] [solution][error segfault sol]                                                 | o            | 2222:80      |                  |
| Webhacking    | Medium       | N7inx                                    | [challenges][N7inx prob], [solution][N7inx sol]                                                                  | 80           | o            | 2024년 5월 #1    |
| Webhacking    | Medium       | One\'s blog                              | [challenges][One's blog prob] [solution][One's blog sol]                                                         | o            | 3000         |                  |
| Webhacking    | Medium       | photoxss                                 | [challenges][photoxss prob], [solution][photoxss sol]                                                            | 3000         | o            | 2024년 4월 #1    |
| Webhacking    | Medium       | Space Casino                             | [challenges][Space Casino prob] [solution][Space Casino sol]                                                     | o            | 5000         |                  |
| Webhacking    | Medium       | svelte_api_v2                            | [challenges][svelte_api_v2 prob], [solution][svelte_api_v2 sol]                                                  | 49614        | o            | 2024년 5월 #1    |
| Webhacking    | Medium       | yurutube                                 | [challenges][yurutube prob] [solution][yurutube sol]                                                             | o            | 44444        |                  |
| Webhacking    | Hard         | hello_proxy                              | [challenges][hello_proxy prob], [solution][hello_proxy sol]                                                      | 49621        | o            | 2024년 5월 #1    |
| Webhacking    | Hard         | LoX-Zombie                               | [challenges][LoX-Zombie prob], [solution][LoX-Zombie sol]                                                        | 44444        | o            | 2024년 5월 #1    |
| Webhacking    | Hard         | Spiderman                                | [challenges][Spiderman prob] [solution][Spiderman sol]                                                           | o            | 44444        |                  |
| Pwnable       | Beginner     | bxorf                                    | [challenges][bxorf prob], [solution][bxorf sol]                                                                  | 12345        | o            | 2025년 1월       |
| Pwnable       | Beginner     | cli-painter                              | [challenges][cli-painter prob], [solution][cli-painter sol]                                                      | 39393        | o            | 2024년 4월 #1    |
| Pwnable       | Beginner     | only_takes_one_bullet                    | [challenges][only_takes_one_bullet prob], [solution][only_takes_one_bullet sol]                                  | 8888         | o            | 2024년 5월 #2    |
| Pwnable       | Beginner     | wasd_space                               | [challenges][wasd_space prob], [solution][wasd_space sol]                                                        | 6789         | o            | 2024년 5월 #2    |
| Pwnable       | Easy         | babykernel                               | [challenges][babykernel prob] [solution][babykernel sol]                                                         | o            | 55555        |                  |
| Pwnable       | Easy         | benchmark                                | [challenges][benchmark prob], [solution][benchmark sol]                                                          | 30030        | o            | 2024년 5월 #2    |
| Pwnable       | Easy         | cRPC v1                                  | [challenges][cRPC v1 prob], [solution][cRPC v1 sol]                                                              | 18181        | o            | 2024년 5월 #2    |
| Pwnable       | Easy         | Hspace Babynote                          | [challenges][Hspace Babynote prob], [solution][Hspace Babynote sol]                                              | 11111        | o            | 2024 HCTF        |
| Pwnable       | Easy         | simple_note                              | [challenges][simple_note prob] [solution][simple_note sol]                                                       | o            | 15252        |                  |
| Pwnable       | Easy         | simple_note_hm                           | [challenges][simple_note_hm prob] [solution][simple_note_hm sol]                                                 | o            | 15252        | HM               |
| Pwnable       | Easy         | tinyerror                                | [challenges][tinyerror prob], [solution][tinyerror sol]                                                          | 11115        | o            | 2025년 1월       |
| Pwnable       | Easy         | uaf_basic                                | [challenges][uaf_basic prob] [solution][uaf_basic sol]                                                           | o            | 4021         |                  |
| Pwnable       | Easy         | ZeroSuperTigerDomain                     | [challenges][ZeroSuperTigerDomain prob] [solution][ZeroSuperTigerDomain sol]                                     | o            | 15252        | (working title)  |
| Pwnable       | Hard         | chatgpt_evm                              | [challenges][chatgpt_evm prob], [solution][chatgpt_evm sol]                                                      | 19191        | o            | 2024년 5월 #2    |
| Pwnable       | Hard         | hspaceETH                                | [challenges][hspaceETH prob], [solution][hspaceETH sol]                                                          | 30031        | o            | 2024년 5월 #2    |
| Pwnable       | Hard         | lottery                                  | [challenges][lottery prob], [solution][lottery sol]                                                              | 15252, 15353 | o            | 2024년 4월 #1    |
| Pwnable       | Hard         | system_vuln                              | [challenges][system_vuln prob], [solution][system_vuln sol]                                                      | 15252        | o            | 2024년 6월 #1    |
| Pwnable       | Medium       | Antivirus Sandbox                        | [challenges][Antivirus Sandbox prob], [solution][Antivirus Sandbox sol]                                          | 11111        | o            | 2024년 6월 #1    |
| Pwnable       | Medium       | ez_vm                                    | [challenges][ez_vm prob], [solution][ez_vm sol]                                                                  | 31337        | o            | 2025년 1월       |
| Pwnable       | Medium       | Hspace Note                              | [challenges][Hspace Note prob], [solution][Hspace Note sol]                                                      | 11112        | o            | 2024 HCTF        |
| Pwnable       | Medium       | only one                                 | [challenges][only one prob], [solution][only one sol]                                                            | 31337        | o            | 2024년 4월 #1    |
| Pwnable       | Medium       | open_read_write                          | [challenges][open_read_write prob], [solution][open_read_write sol]                                              | 10101        | o            | 2024년 5월 #2    |
| Pwnable       | Medium       | p-shell                                  | [challenges][p-shell prob] [solution][p-shell sol]                                                               | o            | 18775        |                  |
| Pwnable       | Medium       | perfect machine                          | [challenges][perfect machine prob], [solution][perfect machine sol]                                              | 15252        | o            | 2025년 1월       |
| Pwnable       | Medium       | pwn101                                   | [challenges][pwn101 prob] [solution][pwn101 sol]                                                                 | o            | 8080         |                  |
| Pwnable       | Medium       | secret file                              | [challenges][secret file prob], [solution][secret file sol]                                                      | 15252        | o            | 2024년 5월 #2    |
| Reversing     | Beginner     | Ah...?                                   | [challenges][Ah...? prob] [solution][Ah...? sol]                                                                 | 2346         | x            | 2024년 6월 #1    |
| Reversing     | Beginner     | serial                                   | [challenges][serial prob] [solution][serial sol]                                                                 | x            | .            |                  |
| Reversing     | Beginner     | vuln_func                                | [challenges][vuln_func prob] [solution][vuln_func sol]                                                           | x            | 12345        |                  |
| Reversing     | Easy         | find_answer                              | [challenges][find_answer prob] [solution][find_answer sol]                                                       | x            | 23456        |                  |
| Reversing     | Easy         | My VM                                    | [challenges][My VM prob] [solution][My VM sol]                                                                   | x            | 서버 X       |                  |
| Reversing     | Easy         | permutation                              | [challenges][permutation prob] [solution][permutation sol]                                                       | x            |              |                  |
| Reversing     | Easy         | 아크                                     | [challenges][아크 prob] [solution][아크 sol]                                                                     | x            | x            |                  |
| Reversing     | Hard         | Surprising Painful Infrastructure        | [challenges][Surprising Painful Infrastructure prob] [solution][Surprising Painful Infrastructure sol]           | o            | 5678         |                  |
| Reversing     | Hard         | Verwandeltes Labyrinth                   | [challenges][Verwandeltes Labyrinth prob] [solution][Verwandeltes Labyrinth sol]                                 | x            | X            | 2024년 4월 #1    |
| Reversing     | Medium       | Catnip                                   | [challenges][Catnip prob] [solution][Catnip sol]                                                                 | x            | -            |                  |
| Reversing     | Medium       | Chip VM                                  | [challenges][Chip VM prob] [solution][Chip VM sol]                                                               |              | X            | 2024년 4월 #1    |
| Reversing     | Medium       | ObfuSWF                                  | [challenges][ObfuSWF prob] [solution][ObfuSWF sol]                                                               | x            | -            |                  |
| Reversing     | Medium       | sigvm                                    | [challenges][sigvm prob] [solution][sigvm sol]                                                                   |              | X            | 2024년 4월 #1    |
| Reversing     | Medium       | 내일 할 일                               | [challenges][내일 할 일 prob] [solution][내일 할 일 sol]                                                         | x            | x            |                  |
| Cryptography  | Beginner     | quadratic function                       | [challenges][quadratic function prob] [solution][quadratic function sol]                                         | x            | X            | 2024년 4월 #1    |
| Cryptography  | Easy         | NAEDILCUE                                | [challenges][NAEDILCUE prob] [solution][NAEDILCUE sol]                                                           | x            | null         | 2024년 4월 #1    |
| Cryptography  | Easy         | GP                                       | [challenges][GP prob] [solution][GP sol]                                                                         | x            | X            | 2024년 6월 #1    |
| Cryptography  | Easy         | Weird dhke                               | [challenges][Weird dhke prob] [solution][Weird dhke sol]                                                         | o            | 54321        |                  |
| Cryptography  | Medium       | SquRSA_xx                                | [challenges][SquRSA_xx prob] [solution][SquRSA_xx sol]                                                           | x            | null         | 2024년 6월 #1    |
| Cryptography  | Medium       | BabyHash                                 | [challenges][BabyHash prob] [solution][BabyHash sol]                                                             | o            | 1971         |                  |
| Cryptography  | Medium, Hard | SquRSA_xx                                | [challenges][SquRSA_xx_H prob] [solution][SquRSA_xx_H sol]                                                       | x            | null         | 2024년 4월 #1    |
| Crypto + misc | Medium       | simple some/ip                           | [challenges][simple some/ip prob] [solution][simple some/ip sol]                                                 | x            | X            |                  |
| Misc          | Easy         | READ (Read, Extract, Analyze, Datasheet) | [challenges][READ prob] [solution][READ sol]                                                                     | o            | 21345, 21346 | 2024 PhyiscalLAB |
| Misc          | Medium       | Mine Circuit                             | [challenges][Mine Circuit prob] [solution][Mine Circuit sol]                                                     | x            | x            |                  |
| Misc          | Medium       | Mr.11100101101111110001011011111110100   | [challenges][Mr.11100101101111110001011011111110100 prob] [solution][Mr.11100101101111110001011011111110100 sol] | o            | 4997         |                  |
| Misc          | Easy         | Steins;Password                          | [challenges][Steins;Password prob] [solution][Steins;Password sol]                                               | x            | 0            |                  |
| Misc          | Hard         | Double_Cast                              | [challenges][Double_Cast prob] [solution][Double_Cast sol]                                                       | o            | 44444:1337   |                  |
| 1-day         | Easy         | 1-day_cve                                | [challenges][1-day_cve prob] [solution][1-day_cve sol]                                                           | x            | none         |                  |
| 1-day         | Easy         | apt_report                               | [challenges][apt_report prob] [solution][apt_report sol]                                                         | x            | none         |                  |
| Forensic      | Beginner     | Get-Flag-From-Space                      | [challenges][Get-Flag-From-Space prob] [solution][Get-Flag-From-Space sol]                                       | x            | None         |                  |
| Forensic      | Easy         | Internal Audit 1                         | [challenges][Internal Audit 1 prob] [solution][Internal Audit 1 sol]                                             | x            | x            |                  |
| Forensic      | Easy         | Internal Audit 2                         | [challenges][Internal Audit 2 prob] [solution][Internal Audit 2 sol]                                             | x            | x            |                  |
| Forensic      | Easy         | Internal Audit 3                         | [challenges][Internal Audit 3 prob] [solution][Internal Audit 3 sol]                                             | x            | x            |                  |
| Forensic      | Easy         | Internal Audit 4                         | [challenges][Internal Audit 4 prob] [solution][Internal Audit 4 sol]                                             | x            | x            |                  |
| Forensic      | Easy         | Internal Audit 5                         | [challenges][Internal Audit 5 prob] [solution][Internal Audit 5 sol]                                             | x            | x            |                  |
| Forensic      | Beginner     | 컴퓨터가 이상해                          | [challenges][컴퓨터가 이상해 prob] [solution][컴퓨터가 이상해 sol]                                               | x            | X            |                  |
| Forensic      | Medium       | 분명 유출됐는데...                       | [challenges][분명 유출됐는데... prob] [solution][분명 유출됐는데... sol]                                         | x            | X            |                  |
| Forensic      | Hard         | 민간 포렌식 기업 퇴사 사건               | [challenges][민간 포렌식 기업 퇴사 사건 prob] [solution][민간 포렌식 기업 퇴사 사건 sol]                         | x            | X            |                  |
| Forensic      | Medium       | C국에게 받은 비밀지령                    | [challenges][C국에게 받은 비밀지령 prob] [solution][C국에게 받은 비밀지령 sol]                                   | x            | X            |                  |
<!-- Webhacking -->

[flagreading prob]: ./challenges/Webhacking/Beginner/flagreading
[flagreading sol]: ./solutions/Webhacking/Beginner/flagreading

[Scroll_Master prob]: ./challenges/Webhacking/Beginner/Scroll_Master
[Scroll_Master sol]: ./solutions/Webhacking/Beginner/Scroll_Master

[soeasy prob]: ./challenges/Webhacking/Beginner/soeasy
[soeasy sol]: ./solutions/Webhacking/Beginner/soeasy

[jsgood prob]: ./challenges/Webhacking/Beginner/jsgood
[jsgood sol]: ./solutions/Webhacking/Beginner/jsgood

[Exec Anything prob]: ./challenges/Webhacking/Easy/Exec_Anything
[Exec Anything sol]: ./solutions/Webhacking/Easy/Exec_Anything

[jelly shop prob]: ./challenges/Webhacking/Easy/jelly_shop
[jelly shop sol]: ./solutions/Webhacking/Easy/jelly_shop

[LoX-goblin prob]: ./challenges/Webhacking/Easy/LoX-goblin
[LoX-goblin sol]: ./solutions/Webhacking/Easy/LoX-goblin

[RandomSecret prob]: ./challenges/Webhacking/Easy/RandomSecret
[RandomSecret sol]: ./solutions/Webhacking/Easy/RandomSecret

[WhiteSpade prob]: ./challenges/Webhacking/Easy/WhiteSpade
[WhiteSpade sol]: ./solutions/Webhacking/Easy/WhiteSpade

[hello_proxy prob]: ./challenges/Webhacking/Hard/hello_proxy
[hello_proxy sol]: ./solutions/Webhacking/Hard/hello_proxy

[LoX-Zombie prob]: ./challenges/Webhacking/Hard/LoX-Zombie
[LoX-Zombie sol]: ./solutions/Webhacking/Hard/LoX-Zombie

[Animal prob]: ./challenges/Webhacking/Medium/Animal
[Animal sol]: ./solutions/Webhacking/Medium/Animal

[N7inx prob]: ./challenges/Webhacking/Medium/N7inx
[N7inx sol]: ./solutions/Webhacking/Medium/N7inx

[photoxss prob]: ./challenges/Webhacking/Medium/photoxss
[photoxss sol]: ./solutions/Webhacking/Medium/photoxss

[svelte_api_v2 prob]: ./challenges/Webhacking/Medium/svelte_api_v2
[svelte_api_v2 sol]: ./solutions/Webhacking/Medium/svelte_api_v2

[Space Casino prob]: ./challenges/Webhacking/Medium/Space_Casino
[Space Casino sol]: ./solutions/Webhacking/Medium/Space_Casino

[yurutube prob]: ./challenges/Webhacking/Medium/yurutube
[yurutube sol]: ./solutions/Webhacking/Medium/yurutube

[error segfault prob]: ./challenges/Webhacking/Medium/error_segfault
[error segfault sol]: ./solutions/Webhacking/Medium/error_segfault

[Spiderman prob]: ./challenges/Webhacking/Hard/Spiderman
[Spiderman sol]: ./solutions/Webhacking/Hard/Spiderman

[8_chances prob]: ./challenges/Webhacking/Easy/8_chances
[8_chances sol]: ./solutions/Webhacking/Easy/8_chances

[HspaceCross prob]: ./challenges/Webhacking/Easy/HspaceCross
[HspaceCross sol]: ./solutions/Webhacking/Easy/HspaceCross

[One's blog prob]: ./challenges/Webhacking/Medium/One\'s_blog
[One's blog sol]: ./solutions/Webhacking/Medium/One\'s_blog

<!-- Pwnable -->
[bxorf prob]: ./challenges/Pwnable/Beginner/bxorf
[bxorf sol]: ./solutions/Pwnable/Beginner/bxorf

[cli-painter prob]: ./challenges/Pwnable/Beginner/cli-painter
[cli-painter sol]: ./solutions/Pwnable/Beginner/cli-painter

[only_takes_one_bullet prob]: ./challenges/Pwnable/Beginner/only_takes_one_bullet
[only_takes_one_bullet sol]: ./solutions/Pwnable/Beginner/only_takes_one_bullet

[wasd_space prob]: ./challenges/Pwnable/Beginner/wasd_space
[wasd_space sol]: ./solutions/Pwnable/Beginner/wasd_space

[benchmark prob]: ./challenges/Pwnable/Easy/benchmark
[benchmark sol]: ./solutions/Pwnable/Easy/benchmark

[cRPC v1 prob]: ./challenges/Pwnable/Easy/cRPC_v1
[cRPC v1 sol]: ./solutions/Pwnable/Easy/cRPC_v1

[Hspace Babynote prob]: ./challenges/Pwnable/Easy/Hspace_Babynote
[Hspace Babynote sol]: ./solutions/Pwnable/Easy/Hspace_Babynote

[tinyerror prob]: ./challenges/Pwnable/Easy/tinyerror
[tinyerror sol]: ./solutions/Pwnable/Easy/tinyerror

[simple_note prob]: ./challenges/Pwnable/Easy/simple_note
[simple_note sol]: ./solutions/Pwnable/Easy/simple_note

[Antivirus Sandbox prob]: ./challenges/Pwnable/Medium/Antivirus_Sandbox
[Antivirus Sandbox sol]: ./solutions/Pwnable/Medium/Antivirus_Sandbox

[ez_vm prob]: ./challenges/Pwnable/Medium/ez_vm
[ez_vm sol]: ./solutions/Pwnable/Medium/ez_vm

[Hspace Note prob]: ./challenges/Pwnable/Medium/Hspace_Note
[Hspace Note sol]: ./solutions/Pwnable/Medium/Hspace_Note

[only one prob]: ./challenges/Pwnable/Medium/only_one
[only one sol]: ./solutions/Pwnable/Medium/only_one

[open_read_write prob]: ./challenges/Pwnable/Medium/open_read_write
[open_read_write sol]: ./solutions/Pwnable/Medium/open_read_write

[perfect machine prob]: ./challenges/Pwnable/Medium/perfect_machine
[perfect machine sol]: ./solutions/Pwnable/Medium/perfect_machine

[secret file prob]: ./challenges/Pwnable/Medium/secret_file
[secret file sol]: ./solutions/Pwnable/Medium/secret_file

[chatgpt_evm prob]: ./challenges/Pwnable/Hard/chatgpt_evm
[chatgpt_evm sol]: ./solutions/Pwnable/Hard/chatgpt_evm

[hspaceETH prob]: ./challenges/Pwnable/Hard/hspaceETH
[hspaceETH sol]: ./solutions/Pwnable/Hard/hspaceETH

[lottery prob]: ./challenges/Pwnable/Hard/lottery
[lottery sol]: ./solutions/Pwnable/Hard/lottery

[system_vuln prob]: ./challenges/Pwnable/Hard/system_vuln
[system_vuln sol]: ./solutions/Pwnable/Hard/system_vuln


[uaf_basic prob]: ./challenges/Pwnable/Easy/uaf_basic
[uaf_basic sol]: ./solutions/Pwnable/Easy/uaf_basic

[simple_note_hm prob]: ./challenges/Pwnable/Easy/simple_note_hm
[simple_note_hm sol]: ./solutions/Pwnable/Easy/simple_note_hm

[babykernel prob]: ./challenges/Pwnable/Easy/babykernel
[babykernel sol]: ./solutions/Pwnable/Easy/babykernel

[ZeroSuperTigerDomain prob]: ./challenges/Pwnable/Easy/ZeroSuperTigerDomain
[ZeroSuperTigerDomain sol]: ./solutions/Pwnable/Easy/ZeroSuperTigerDomain

[pwn101 prob]: ./challenges/Pwnable/Medium/pwn101
[pwn101 sol]: ./solutions/Pwnable/Medium/pwn101

[p-shell prob]: ./challenges/Pwnable/Medium/p-shell
[p-shell sol]: ./solutions/Pwnable/Medium/p-shell



<!-- Reversing -->

[Ah...? prob]: ./challenges/Reversing/Beginner/ah,,,?
[Ah...? sol]: ./solutions/Reversing/Beginner/ah,,,?
<!-- . -> , -->

[sigvm prob]: ./challenges/Reversing/Medium/sigvm
[sigvm sol]: ./solutions/Reversing/Medium/sigvm

[Chip VM prob]: ./challenges/Reversing/Medium/Chip_VM
[Chip VM sol]: ./solutions/Reversing/Medium/Chip_VM

[Verwandeltes Labyrinth prob]: ./challenges/Reversing/Hard/Verwandeltes_Labyrinth
[Verwandeltes Labyrinth sol]: ./solutions/Reversing/Hard/Verwandeltes_Labyrinth

[find_answer prob]: ./challenges/Reversing/Easy/find_answer
[find_answer sol]: ./solutions/Reversing/Easy/find_answer

[vuln_func prob]: ./challenges/Reversing/Beginner/vuln_func
[vuln_func sol]: ./solutions/Reversing/Beginner/vuln_func

[Surprising Painful Infrastructure prob]: ./challenges/Reversing/Hard/Surprising_Painful_Infrastructure
[Surprising Painful Infrastructure sol]: ./solutions/Reversing/Hard/Surprising_Painful_Infrastructure

[Catnip prob]: ./challenges/Reversing/Medium/Catnip
[Catnip sol]: ./solutions/Reversing/Medium/Catnip

[My VM prob]: ./challenges/Reversing/Easy/My_VM
[My VM sol]: ./solutions/Reversing/Easy/My_VM

[내일 할 일 prob]: ./challenges/Reversing/Medium/내일_할_일
[내일 할 일 sol]: ./solutions/Reversing/Medium/내일_할_일

[ObfuSWF prob]: ./challenges/Reversing/Medium/ObfuSWF
[ObfuSWF sol]: ./solutions/Reversing/Medium/ObfuSWF

[serial prob]: ./challenges/Reversing/Beginner/serial
[serial sol]: ./solutions/Reversing/Beginner/serial

[permutation prob]: ./challenges/Reversing/Easy/permutation
[permutation sol]: ./solutions/Reversing/Easy/permutation

[아크 prob]: ./challenges/Reversing/Easy/아크
[아크 sol]: ./solutions/Reversing/Easy/아크


<!-- Cryptography -->
[quadratic function prob]: ./challenges/Cryptography/Beginner/quadratic_function
[quadratic function sol]: ./solutions/Cryptography/Beginner/quadratic_function

[NAEDILCUE prob]: ./challenges/Cryptography/Easy/NAEDILCUE
[NAEDILCUE sol]: ./solutions/Cryptography/Easy/NAEDILCUE

[GP prob]: ./challenges/Cryptography/Easy/GP
[GP sol]: ./solutions/Cryptography/Easy/GP

[SquRSA_xx prob]: ./challenges/Cryptography/Medium/SquRSA_xx
[SquRSA_xx sol]: ./solutions/Cryptography/Medium/SquRSA_xx

[SquRSA_xx_H prob]: ./challenges/Cryptography/Hard/SquRSA_xx
[SquRSA_xx_H sol]: ./solutions/Cryptography/Hard/SquRSA_xx

[BabyHash prob]: ./challenges/Cryptography/Medium/BabyHash
[BabyHash sol]: ./solutions/Cryptography/Medium/BabyHash

[Weird dhke prob]: ./challenges/Cryptography/Easy/Weird_dhke
[Weird dhke sol]: ./solutions/Cryptography/Easy/Weird_dhke

[simple some/ip prob]: ./challenges/Cryptography/Medium/simple_some-ip
[simple some/ip sol]: ./solutions/Cryptography/Medium/simple_some-ip

<!-- Misc -->
[READ prob]: ./challenges/Misc/Easy/READ
[READ sol]: ./solutions/Misc/Easy/READ

[Mine Circuit prob]: ./challenges/Misc/Medium/Mine_Circuit
[Mine Circuit sol]: ./solutions/Misc/Medium/Mine_Circuit

[Mr.11100101101111110001011011111110100 prob]: ./challenges/Misc/Medium/Mr.11100101101111110001011011111110100
[Mr.11100101101111110001011011111110100 sol]: ./solutions/Misc/Medium/Mr.11100101101111110001011011111110100

[Steins;Password prob]: ./challenges/Misc/Easy/Steins;Password
[Steins;Password sol]: ./solutions/Misc/Easy/Steins;Password

[Double_Cast prob]: ./challenges/Misc/Hard/Double_Cast
[Double_Cast sol]: ./solutions/Misc/Hard/Double_Cast

<!-- 1-day -->
[1-day_cve prob]: ./challenges/1-day/Easy/1-day_cve
[1-day_cve sol]: ./solutions/1-day/Easy/1-day_cve

[apt_report prob]: ./challenges/1-day/Easy/apt_report
[apt_report sol]: ./solutions/1-day/Easy/apt_report

<!-- Forensic -->
[Internal Audit 1 prob]: ./challenges/Forensic/Easy/Internal_Audit_1
[Internal Audit 1 sol]: ./solutions/Forensic/Easy/Internal_Audit_1

[Internal Audit 2 prob]: ./challenges/Forensic/Easy/Internal_Audit_2
[Internal Audit 2 sol]: ./solutions/Forensic/Easy/Internal_Audit_2

[Internal Audit 3 prob]: ./challenges/Forensic/Easy/Internal_Audit_3
[Internal Audit 3 sol]: ./solutions/Forensic/Easy/Internal_Audit_3

[Internal Audit 4 prob]: ./challenges/Forensic/Easy/Internal_Audit_4
[Internal Audit 4 sol]: ./solutions/Forensic/Easy/Internal_Audit_4

[Internal Audit 5 prob]: ./challenges/Forensic/Easy/Internal_Audit_5
[Internal Audit 5 sol]: ./solutions/Forensic/Easy/Internal_Audit_5

[컴퓨터가 이상해 prob]: ./challenges/Forensic/Beginner/컴퓨터가_이상해
[컴퓨터가 이상해 sol]: ./solutions/Forensic/Beginner/컴퓨터가_이상해

[분명 유출됐는데... prob]: ./challenges/Forensic/Medium/분명_유출됐는데,,,
[분명 유출됐는데... sol]: ./solutions/Forensic/Medium/분명_유출됐는데,,,

[민간 포렌식 기업 퇴사 사건 prob]: ./challenges/Forensic/Hard/민간_포렌식_기업_퇴사_사건
[민간 포렌식 기업 퇴사 사건 sol]: ./solutions/Forensic/Hard/민간_포렌식_기업_퇴사_사건

[C국에게 받은 비밀지령 prob]: ./challenges/Forensic/Medium/C국에게_받은_비밀지령
[C국에게 받은 비밀지령 sol]: ./solutions/Forensic/Medium/C국에게_받은_비밀지령

[Get-Flag-From-Space prob]: ./challenges/Forensic/Beginner/Get-Flag-From-Space
[Get-Flag-From-Space sol]: ./solutions/Forensic/Beginner/Get-Flag-From-Space


<!-- google -->

<!-- Hardware -->
[PhysicalLab-Whisperer prob]: ./challenges/Hardware/Medium/PhysicalLab-Whisperer
[PhysicalLab-Whisperer sol]: ./solutions/Hardware/Medium/PhysicalLab-Whisperer

[Password Guessing prob]: ./challenges/Hardware/Hard/Password_Guessing
[Password Guessing sol]: ./solutions/Hardware/Hard/Password_Guessing

[u art? prob]: ./challenges/hardware/Beginner/u_art?
[u art? sol]: ./solutions/hardware/Beginner/u_art?
<!-- 이거 포너 -->




[asdf prob]: http://google.com
[asdf sol]: http://google.com


