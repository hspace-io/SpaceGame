import os
import binascii
import random
import subprocess

def create_and_move_to_chall_dir():
    chall_path = binascii.hexlify(os.urandom(16)).decode()

    try:
        os.mkdir(chall_path)
    except FileExistsError:
        print(f"Something's wrong. retry!")
        exit(-1)
    except OSError as e:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)

    try:
        os.chdir(chall_path)
    except FileNotFoundError:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)
    except PermissionError:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)

    return chall_path

def gen_flag():
    flag_dir_idx = random.randint(0, 0xf)

    for i in range(0x10):
        path = binascii.hexlify(os.urandom(16)).decode()
        if flag_dir_idx == i:
            os.mkdir(path)
            flag_dir = path
            continue
        with open(path, 'w') as f:
            f.write('hmm... nice try... but... it\'s not flag :(')

    flag_path = f'{flag_dir}/{binascii.hexlify(os.urandom(16)).decode()}'

    with open(flag_path, 'w') as f:
        f.write(r'hspace{S05ry_t0_b0th3r_y0u..f0rg1v3_m3_pl3as3}')

def run(chall_path):
    try:
        subprocess.run('../prob', shell=True, timeout=45)
    except subprocess.TimeoutExpired:
        pass
    except:
        print(f"Please contact admins. this should be a critical issue.")
    print("[?] done?")

    try:
        os.chdir('../')
    except FileNotFoundError:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)
    except PermissionError:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)

    os.system(f"rm -rf {chall_path}")

def main():
    chall_path = create_and_move_to_chall_dir()
    gen_flag()
    run(chall_path)

if __name__ == '__main__':
    main()
