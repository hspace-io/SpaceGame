import struct
import time
import string
import random
from unicorn import *
from unicorn.x86_const import *
from elftools.elf.elffile import ELFFile
from capstone import *

# Instruction Counting
INS_COUNT = 0
IS_COUNTING = 0
IS_CORRECT = 0

# 파일 이름
BINARY = "guess"

# 페이지 크기
PAGE_SIZE = 0x1000

md = Cs(CS_ARCH_X86, CS_MODE_64)
md.detail = True

def start_print():
    print("ARE YOU A GUESSING MASTER?\nPlease guess the 4 length password!\nYou only have 6 chance to guess the letter inside the password. But if it's correct, it doesn't count.\nI'm not sure if it is helping but I will also give you a [hint]...\n")

def calculate_print():
    print("Comparing Password... ", end="", flush=True)
    time.sleep(1)

def check_correct():
    global IS_CORRECT
    if IS_CORRECT == 1:
        print("Correct!!")
        IS_CORRECT = 0
    else:
        print("Wrong!!")

def print_flag():
    with open("./flag", "r") as f:
        flag = f.read()
        print(flag)

def generate_random_string(length=4):
    characters = string.ascii_lowercase + string.digits
    random_string = ''.join(random.sample(characters, length))
    return random_string

def align_up(addr, size):
    return (addr + size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1)

def align_down(addr):
    return addr & ~(PAGE_SIZE - 1)

def hook_code(uc, address, size, user_data):
    global IS_COUNTING
    global INS_COUNT
    global IS_CORRECT
    if IS_COUNTING == 1:
        INS_COUNT += 1

    #print(f"Executing at 0x{address:x}, instruction size = {size}")
    instruction = uc.mem_read(address, size)
    for i in md.disasm(instruction, address):
        #print(f"0x{i.address:x}:\t{i.mnemonic}\t{i.op_str}")
        #print(f"Instruction: {instruction.hex()}")
        if i.mnemonic == "syscall" :
            edi_value = uc.reg_read(UC_X86_REG_RDI)
            if edi_value == 0:
                # Read Syscall
                rsi = uc.reg_read(UC_X86_REG_RSI)
                rdx = uc.reg_read(UC_X86_REG_RDX)

                # Get input from user
                input_data = input()[:rdx]

                # Write the input data to memory at RSI address
                uc.mem_write(rsi, input_data.encode('utf-8'))

            if edi_value == 1:
                # Write Syscall
                rsi = uc.reg_read(UC_X86_REG_RSI)
                rdx = uc.reg_read(UC_X86_REG_RDX)
               
                # Read and print the memory at RSI address with RDX length
                data = uc.mem_read(rsi, rdx)
                print(data.decode('utf-8'), end="")

        if i.op_str == "edi, 0x3c" :
            uc.emu_stop()

        if i.mnemonic == "mov" and i.op_str == "dword ptr [rbp - 0x38], 0" :
            IS_COUNTING = 1

        elif i.mnemonic == "lea" and i.op_str == "rax, [rbp - 0x59]":
            if IS_COUNTING == 1:
                calculate_print()
                check_correct()
                IS_COUNTING = 0
                print(f"HINT : Total Assembly Instruction used for comparing : {INS_COUNT}")
                INS_COUNT = 0

        if address == 0x401ae6 :
            value = generate_random_string()
            # print(value) 
           	# rbp-0x59 위치 계산
            rbp_value = uc.reg_read(UC_X86_REG_RBP)
            target_address = rbp_value - 0x59

            # 4바이트 값을 메모리에 쓰기
            uc.mem_write(target_address, value.encode('utf-8')) 

        if address == 0x401ce6 :
            print_flag()

        if address == 0x401c32 :
            IS_CORRECT = 1;

def read_elf(filename):
    f = open(filename, "rb")
    elffile = ELFFile(f)
    entry = elffile.header.e_entry
    segments = [(segment.header.p_vaddr, segment.data()) for segment in elffile.iter_segments() if segment.header.p_type == 'PT_LOAD']
    return entry, segments, elffile, f

def find_main_address(elffile):
    symtab = elffile.get_section_by_name('.symtab')
    if not symtab:
        raise Exception("No .symtab section found in ELF file.")
    for symbol in symtab.iter_symbols():
        if symbol.name == 'main':
            return symbol.entry.st_value
    raise Exception("No 'main' function found in ELF file.")

def main():
    entry, segments, elffile, elf_file = read_elf(BINARY)
    main_address = find_main_address(elffile)
    #print(f"main function address: 0x{main_address:x}")

    # Emulate X86_64
    mu = Uc(UC_ARCH_X86, UC_MODE_64)

    # 메모리 맵핑 및 코드 로드
    for vaddr, code in segments:
        mem_size = align_up(vaddr, len(code)) - align_down(vaddr)
        mu.mem_map(align_down(vaddr), mem_size)
        mu.mem_write(vaddr, code)
        #print(f"Mapped {len(code)} bytes at 0x{vaddr:x}, size: 0x{mem_size:x}")

    # 스택 설정
    STACK_SIZE = 8 * 1024 * 1024  # 8 MB
    STACK_TOP = 0x80000000  # 스택의 최상위 주소
    STACK_BASE = STACK_TOP - STACK_SIZE  # 스택의 시작 주소
    mu.mem_map(STACK_BASE, STACK_SIZE)
    mu.reg_write(UC_X86_REG_RSP, STACK_TOP - 1)

    # 힙 설정
    HEAP_SIZE = 8 * 1024 * 1024  # 8 MB
    HEAP_BASE = 0x90000000  # 힙의 시작 주소
    mu.mem_map(HEAP_BASE, HEAP_SIZE)

    # 실행 위치 설정
    mu.reg_write(UC_X86_REG_RIP, main_address)

    start_print()

    # 명령어 실행 시 콜백 훅 설정
    mu.hook_add(UC_HOOK_CODE, hook_code)

    # 실행
    mu.emu_start(main_address, main_address + PAGE_SIZE)

    # 파일 닫기
    elf_file.close()

if __name__ == "__main__":
    main()

