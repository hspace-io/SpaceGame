import uuid
import hashlib
import string
import dragon as dragon
import os
from time import *
import nunpy as np
from python import python_exec
from node import node_exec

HP_Dragon=999999999999999999999
MAX_SIZE = 2147483647

SPELL = {
'1' : '모든 힘의 근원이여 붉게 타오르는 빛나는 불꽃이여 내 손 안에 들어와 힘이 되어라', 
'2' : '뇌정이여, 자정의 충격으로, 쓰러뜨려라', 
'3' : '그대가 원하는 곳에 위대한 얼음의 가호가 있을지니 빙하의 탁류를 받아라',
'4' : '달려라 바람, 질주하라, 때려눕혀라', 
'5' : '빛을 품은 이 힘, 불가시를 내 앞으로 인도해 혼돈으로부터 접촉하고자 한다. 전격이여, 꿰뚫어라!',
'6' : '그것은 처음의 불꽃. 그것은 끝의 불꽃. 현재, 과거, 미래-만상을 뛰어넘는 금단의 힘. 불꽃이여, 나는 파괴자, 무자비한 이. 나의 피, 나의 살을 가지고, 옛 신들의 계율을 지금 깨노라!'
}

MP = 100000
HP = 14000

ItemName=['초초초초 상급 MP물약', '상급 Mp물약', '나침반', '마도서', '여분모자', '여벌 옷', '지갑']

MagicName=['Fireball', 'Shock bolt', 'Ice Spear', 'Wind Storm', 'Lite of Saber', 'EXPLOSION']
ManaCost = [['1', '20', '100', '5000', '50000', '2147483647'], ['10', '30', '1000', '10000', '200000', '2147483648']]
Damage = [['100', '300', '250', '500', '25000000', '400000000000'], ['200', '600', '500', '1000', '50000000', '999999999999999999999']]

def Status() :
    print("\n=======status=======")
    print(f"Current MP - {MP}")
    print(f"Current HP - {HP}")
    print("====================\n")

def Monster() :
    print("\n=========Uroboros========")
    print("Uroboros MP - 99999999999999999999")
    print(f"Uroboros HP - {HP_Dragon}")
    print("==========================\n")

def Menu() :
    Status()
    Monster()
    print('1. 아공간 주머니')
    print('2. 공격하기')
    print('3. 도망가기')
    print(end='>> ')
    try :
        return int(input())
    except :
        return -1

def Bag() :
    global MP
    print("소지품 ▼")
    for i in range(len(ItemName)) :
        print(i+1, '-', ItemName[i])

    print(end='>> ')
    try :
        n=int(input())
    except :
        n = -1

    if(n==1) :
        MP=np.mul(MP, MP)
        print('초초초초 상급 MP물약을 마십니다. . . 현재 MP :', MP)

    elif(n==2) :
        MP=np.mul(MP, 2)
        print('MP 물약을 마십니다. . . 현재 MP :', MP)

    elif(n==3) : 
        print('   남\n서    동\n   북')

    elif(n==4) : 
        print('이미 다 아는 마법이다..')

    elif(n==5) : 
        print('똑같은 모자이지만 좀 더 깨끗하다')

    elif(n==6) : 
        print('똑같은 옷이지만 좀 더 깨끗하다')

    elif(n==7) : 
        print('동화 1닢 밖에 없다..')

    else :
        print('뒤적뒤적')
        print('\n없는것 같다.')
        return 0

    if MP > MAX_SIZE :
        print("Integer Overflow detected")
        exit()

def Attack() :
    global MP, HP_Dragon
    can_double_cast = 0
    double_cast = 0
    magic = 0
    print('사용가능 마법 ▼')

    for i in range(len(MagicName)) :
        if not np.checkmana(MP, int(ManaCost[double_cast][i])) :
            continue
        print(f"{i+1} - {MagicName[i]:<15} (Cost : {ManaCost[double_cast][i]:>10}, Damage : {Damage[double_cast][i]:>15})")
        if not np.checkmana(MP, int(ManaCost[1][i])) :
            continue
        print(f"    >> Double Cast  (Cost : {ManaCost[1][i]:>10}, Damage : {Damage[1][i]:>15})")
        can_double_cast = i


    print("사용할 마법을 입력 해주세요 ", end='>> ')
    try :
        magic = int(input())
        if magic > 6 :
            print("존재하지 않는 마법이다.")
            return 0
    except :
        print("존재하지 않는 마법이다.")
        return 0

    if magic <= can_double_cast + 1 :
        print("\n\n이중영창을 사용하시겠습니까?(추가 마나가 발생합니다)(y/n)")
        print(end='>> ')
        n = input()
        if(n == 'y') : double_cast = 1

    if not np.checkmana(MP, int(ManaCost[double_cast][magic-1])) :
        print("마나가 부족해서 사용할 수 없다.")
        return 0
    
    print("주문을 입력해주세요")

    code = ''
    chars = set(string.printable) - set('{};')
    assert set(chars) | set(code) == set(chars)
    while True: 
        data = input()
        if data == 'end': break
        code += data + '\n'

    uniqueId = hashlib.sha256(uuid.uuid4().hex.encode()).hexdigest()
    tmp_path = f'spell/{uniqueId}'
    with open(tmp_path, 'w') as fp:
        fp.write(code)
        os.chmod(tmp_path, 0o755)

    #! usage : (isErr, data)
    magic1 = python_exec(tmp_path)
    magic2 = node_exec(tmp_path)
    spell1 = b''
    spell2 = b''

    if magic1[0][-1:] == b'\n' :
        spell1 = magic1[0][:-1]
    else :
        spell1 = magic1[0]
    
    if magic2[0][-1:] == b'\n' :
        spell2 = magic2[0][:-1]
    else :
        spell2 = magic2[0]

    if double_cast and (magic1 == magic2) and spell1 == SPELL[str(magic)].encode():
        print(f"<마법> {MagicName[magic-1]}을 이중영창 합니다")
        pass
    elif not double_cast and SPELL[str(magic)].encode() in [spell1, spell2]:
        print(f"<마법> {MagicName[magic-1]}을 영창합니다")
        pass
    else:
        print(f"<마법> {MagicName[magic-1]} 사용에 실패했습니다!")
        return 0
    
    if double_cast :
        {
            "1": (lambda: {
                print(f"{SPELL[str(magic)]}")
            }),
            "2": (lambda: {
                print(f"{SPELL[str(magic)]}")
            }),
            "3": (lambda: {
                print(f"{SPELL[str(magic)]}")
            }),
            "4": (lambda: {
                print(f"{SPELL[str(magic)]}")
            }),
            "5": (lambda: {
                print(f"{SPELL[str(magic)]}")
            }),
            "6": (lambda: {
                print(f"{SPELL[str(magic)]}")
            })
        }[f"{magic}"]()
        print(f"우로보로스에게 {Damage[double_cast][magic-1]} 의 데미지를 입혔다!")
        print(f"{ManaCost[double_cast][magic-1]} 의 마나를 사용했습니다")

    else :
        {
            "1": (lambda: {
                    print(f"{SPELL[str(magic)]}")
            }),
            "2": (lambda: {
                    print(f"{SPELL[str(magic)]}")
            }),
            "3": (lambda: {
                print(f"{SPELL[str(magic)]}")
            }),
            "4": (lambda: {
                print(f"{SPELL[str(magic)]}")
            }),
            "5": (lambda: {
                print(f"{SPELL[str(magic)]}")
            }),
            "6": (lambda: {
                print(f"{SPELL[str(magic)]}")
            })
        }[f"{magic}"]()
        sleep(.5)
        print(f"우로보로스에게 {Damage[double_cast][magic-1]} 의 데미지를 입혔다!")
        sleep(.5)
        print(f"{ManaCost[double_cast][magic-1]} 의 마나를 사용했습니다")

    MP=np.sub(MP, int(ManaCost[double_cast][magic-1]))
    HP_Dragon-=int(Damage[double_cast][magic-1])

def Run() :
    print('무사히 도망쳤다!')
    exit()

def intro() :
    dragon.strong_dragon()
    dragon.logo()

def main() :
    global HP
    intro()

    while(1) :
        if(HP_Dragon<=0) :
            print('The flag is : hsapce2024{EXPL0S1ON_i5_Alw4ys_B3autiful}')
            exit()
        if MP == 0 :
            print("마나 결핍..")    
            exit()
        if HP <= 0 :
            print('HP가 0입니다!!!!!!!!! . . . . . . 사망')
            exit()
        n=Menu()
        if(n==1) : Bag()
        elif(n==2) : Attack()
        elif(n==3) : Run()
        else : print("잘못된 입력입니다.")

        print('Dragon에게 공격받았다! . . . HP가 1000 감소했다')
        HP-=1000

if __name__ == '__main__' :
    main()
