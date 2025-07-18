# 민간 포렌식 기업 퇴사 사건

# Usage

# Concept
- Steganography
- Memory Analysis

# Writeup
민간 포렌식 기업에 다니던 최모씨는 퇴사하면서 자신이 사용하던 컴퓨터속 BitLocker로 암호화된 디스크의 키를 의도적으로 와이핑한 후 퇴사하였다.
회사는 이를 빠르게 파악하여 삭제한 직후 해당 컴퓨터의 메모리를 덤프하여 확보하였고, 해당 퇴사자와는 연락이 닿지 않는 상황이다.
당신은 암호화된 디스크속 자료를 찾아 플래그를 획득해야한다.


제공된 가상머신 속 메모리 덤프 파일, Outlook 설치사실, 웹 히스토리를 먼저 파악한다.
Outlook을 사용한다는 것을 기반으로 메모리 덤프의 filescan을 통해 outlookbackup.zip파일을 확인한다.
해당 파일을 추출하여 확인하면 바탕화면에 키파일이 존재하였다는 사실을 확인한다.
"Desktop"을 기반으로 메모리의 파일들에서 grep하면 수상한 Present.jpg를 찾을 수 있다.
해당 파일을 분석하여 png라는 사실과 시작부분의 청크가 훼손됨을 확인한다.
고정된 값의 헤더를 복구해준 뒤 CRC기반의 높이 너비 계산 코드를 사용하여 높이 및 너비를 계산한다.

'''Python
import binascii
import struct

target = open("file_path","rb").read() #계산할 png파일의 경로

widths = [struct.pack('>i', i) for i in range(2000)]
heights = [struct.pack('>i', j) for j in range(2000)]

for i in range(2000):
	for j in range(2000):
		data = target[12:16] + widths[i] + heights[j] + target[24:29]
		crc32 = binascii.crc32(data) & 0xffffffff
		if crc32 == 0x7d46d4a0: # PNG파일의 CRC값
			print("Width : {}, Height : {}".format(hex(i), hex(j)))
'''

이후 colortype에 맞는 8,16 두가지 bit depths값중 하나를 넣어주면 파일이 복구 된다.

청크를 복구하여 이미지를 열면 2bpp lsb stego라는 힌트가 주어진다.
해당 내용과 웹히스토리, 문서 디렉토리속 xor based stego에 관한 사진을 조합하여 한픽셀의 lsb값들을 xor하여 해당 이미지속 숨겨진 png파일을 추출한다.

'''python
from PIL import Image

img = Image.open("target.png")#데이터가 은닉된 png파일의 경로
pixels = img.getdata()
cnt = 0
ans = 0
msg  = ""
for pixel in pixels:
    if cnt == 8:
        msg += ' '
        cnt = 0
        ans += 1
    r_lsb = pixel[0] & 1
    g_lsb = pixel[1] & 1
    b_lsb = pixel[2] & 1
    msg += str(r_lsb ^ g_lsb) + str(r_lsb ^ b_lsb) 
    cnt += 2

with open('output.txt', 'w') as f:#결과값을 저장할 파일명
    f.write(msg)

'''
추출한 png파일은 maxi코드라는 2d 바코드 이미지이다.

이미지를 디코딩 하면 Bitlocker로 암호화된 파티션의 키가 제공된다.
(5p4c3w42 15 50 much FUN H4H4H4!!!)

암호화를 해제하여 D-Drive의 존재하는 플래그 파일을 확인한다.
SPACEWAR{f0r3n51c5 15 v3ry d1v3r53!!!}