import random
import time
import os

def print_uart_art():
    text_art1 = r"""
                         ('-.     _  .-')   .-') _    ,------.  
                        ( OO ).-.( \( -O ) (  OO) )  '  .--.  '  """
    text_art2 = """ 
    ,--. ,--.           / . --. / ,------. /     '._ |  |  |  | 
    |  | |  |           | \\-.  \\  |   /`. '|'--...__)'--'  |  | """
    text_art3 = """
    |  | | .-')       .-'-'  |  | |  /  | |'--.  .--'    __.  | 
    |  |_|( OO )       \\| |_.'  | |  |_.' |   |  |      |   .'  """
    text_art4 = """
    |  | | `-' /        |  .-.  | |  .  '.'   |  |      |___|   
    ('  '-'(_.-'        |  | |  | |  |\\  \\    |  |      .---.   """
    text_art5 = """
     `-----'            `--' `--' `--' '--'   `--'      '---'   """
 

    print(text_art1, end='')
    time.sleep(0.5)
    print(text_art2, end='')
    time.sleep(0.5)
    print(text_art3, end='')
    time.sleep(0.5)
    print(text_art4, end='')
    time.sleep(0.5)
    print(text_art5, end='')
    time.sleep(0.5)
    time.sleep(2)



# 화면을 지우는 함수
def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

# 멀티미터 기능 구현 클래스
class Multimeter:
    def __init__(self):
        self.mode = None

    def set_mode(self, mode):
        if mode in ['DCV']:
            self.mode = mode
        else:
            print("지원하지 않는 모드입니다. DCV 모드를 선택하십시오.")
            self.mode = None

    def measure(self, black_pin, red_pin):
        if self.mode == 'DCV':
            return self.dcv_measure(black_pin, red_pin)
        #elif self.mode == 'DCA':
            #return self.dca_measure(black_pin, red_pin)
        else:
            print("모드를 먼저 설정하십시오.")
            return None

    def dcv_measure(self, black_pin, red_pin):
        if black_pin == 'GND':
            if red_pin == 'GND':
                return 0.0
            elif red_pin == 'VCC':
                return 3.3
            elif red_pin == 'RX':
                return 0.0
            elif red_pin == 'TX':
                return round(random.uniform(2.4, 3.3), 2)  # TX 핀의 경우 2.4V ~ 3.3V 사이의 전압 값
        return 0.0  # 측정할 수 없는 경우 0으로 반환

    '''
    def dca_measure(self, black_pin, red_pin):
        if black_pin == 'GND':
            if red_pin == 'GND':
                return 0.0
            elif red_pin == 'VCC':
                return 0.0
            elif red_pin == 'RX':
                return 0.0
            elif red_pin == 'TX':
                return round(random.uniform(23.0, 44.0), 1)  # TX 핀의 경우 23mA ~ 44mA 사이의 전류 값
        return 0.0  # 측정할 수 없는 경우 0으로 반환
    '''

# UART 부팅 로그를 인터페이스에 따라 다르게 표시하는 함수
def display_boot_log(interface_number):
    
    boot_logs = {
        1: """
NET: Registered protocol family 17
Ebtables v2.0 registered

802.19 VLAN Support v1.8 Ben Greear <greearb@candelatech.com>
All bugs added by David S. Miller <davem@redhat.com>

VFS: Mounted root (squashfs filesystem) readonly on device 31:5.
Freeing unused kernel memory: 144k freed

--> Mount devtmpfs [ Success ]
check_and_install_mtk_eeprom:Check EEPROM (wl_mode:0) -> [ OK ]

Checking System Requirements *******************

DRAM SIZE : Requirement->32 Mbytes / Mounted->32 Mbytes -- [OK]
FLASH SIZE : Requirement->4 Mbytes / Mounted->4 Mbytes -- [OK]

Start inittime
Product ID:n604se Version:14.198
--> init_sys
-> Check if boot in flash ... [ OK ]
---> gpio_board_init
---> gpio.init end
> Load sysctl.d
> init_restore





hspace{uuuuuu_uart!!_fuuuuuuun_hardware-_^}
        """

    }
    
    log = boot_logs.get(interface_number, "No log available for this interface.")
    
    clear_screen()
    for line in log.splitlines():
        print(line)
        time.sleep(1)
        
    
    
    print(boot_logs.get(interface_number, "Invalid interface number"))

# 깨진 글자나 아무 것도 출력되지 않게 만드는 함수 (TX/RX 핀 또는 Baudrate가 틀렸을 때 출력)
def display_garbled_output():
    garbled_output = ''.join(random.choice(['$', '%', '@', '#', '&', '!', '*', '?',' ', '=', '.', 'A', 'h']) for _ in range(1500))
    print(garbled_output)

# UART 통신 검증 (TX/RX 핀과 Baudrate가 모두 맞으면 True, 그렇지 않으면 False)
def validate_uart_connection(tx_pin_input, rx_pin_input, correct_tx, correct_rx, selected_baudrate, correct_baudrate):
    return tx_pin_input == correct_tx and rx_pin_input == correct_rx and selected_baudrate == correct_baudrate


def display_multimeter_ui(measurement_str, mode='DCV', black_pin_number=None, red_pin_number=None, interface_number=None):
    clear_screen()
    print(f"""
        
        UART Interface {interface_number} :

        ┌───────────────────────────────────────────┐
        │                 MULTIMETER                │
        ├───────────────────────────────────────────┤
        │ Mode: {mode:<4}          Battery: [██████]     │
        ├───────────────────────────────────────────┤
        │                                           │
        │                {measurement_str:^9}                  │
        │                                           │
        │                                           │
        ├───────────────────────────────────────────┤
        │  Black Lead: Pin {black_pin_number:<2} │  Red Lead: Pin {red_pin_number:<2}   │
        └───────────────────────────────────────────┘
    """)

# UART 인터페이스 선택 함수
def uart_interface_selection():
    interface_number = input("UART 인터페이스를 선택하십시오 (1): ").strip()
    if interface_number not in ['1']:
        print("올바른 인터페이스 번호를 입력하십시오.")
        return None
    print(f"\nUART Interface {interface_number}가 선택되었습니다.")
    return int(interface_number)

# 리드선 연결 함수
def connect_probes(interface_number):
    if interface_number is None:
        print("먼저 UART 인터페이스를 선택하십시오.")
        return None, None
    black_pin_number = input("검정선(-) 연결할 핀 번호를 선택하십시오 (1-6): ").strip()
    red_pin_number = input("빨강선(+) 연결할 핀 번호를 선택하십시오 (1-6): ").strip()
    
    if not black_pin_number.isdigit() or not red_pin_number.isdigit():
        print("올바른 핀 번호를 입력하십시오.")
        return None, None
    
    black_pin_number = int(black_pin_number)
    red_pin_number = int(red_pin_number)

    # 핀 번호가 1~6 범위 내인지 확인
    if black_pin_number < 1 or black_pin_number > 6 or red_pin_number < 1 or red_pin_number > 6:
        print("잘못된 핀 번호입니다.")
        return None, None

    return black_pin_number, red_pin_number

# 멀티미터 모드 선택 함수
def choose_measurement_mode(multimeter):
    mode = input("멀티미터 모드를 선택하십시오 (DCV): ").strip().upper()
    multimeter.set_mode(mode)


# 핀 연결이 적합한지 확인 후 결과 출력
def display_measurement(multimeter, black_pin_number, red_pin_number, interface_number):
    if black_pin_number is None or red_pin_number is None:
        print("검정선과 빨강선이 연결되지 않았습니다.")
        return
    
    black_pin_role = pin_roles[interface_number][black_pin_number]
    red_pin_role = pin_roles[interface_number][red_pin_number]

    # TX 핀일 경우 값을 주기적으로 갱신하며 표시
    while True:
        result = multimeter.measure(black_pin_role, red_pin_role)
        measurement_str = ""
        if multimeter.mode == 'DCV':
            measurement_str = f"{result} V"
        '''elif multimeter.mode == 'DCA':
            measurement_str = f"{result} mA" '''
        
        # TX 핀일 경우 값이 변동되도록 구현
        display_multimeter_ui(measurement_str, multimeter.mode, black_pin_number, red_pin_number, interface_number)
        
        # TX 핀 값 변동
        
        break  # TX 핀이 아닐 경우 한 번만 표시 후 종료
        
        # 사용자가 메뉴로 돌아가려면 'q'를 누르도록 안내
        #if input("메뉴로 돌아가려면 'q'를 입력하십시오: ").strip().lower() == 'q':
        #   break

pin_roles = {
    #1: {1: 'GND', 2: 'VCC', 3: 'RX', 4: 'TX', 5: 'VCC', 6: 'GND'},  
    #2: {1: 'GND', 2: 'VCC', 3: 'TX', 4: 'RX', 5: 'GND', 6: 'VCC'},  
    1: {1: 'TX', 2: 'RX', 3: 'VCC', 4: 'GND', 5: 'VCC', 6: 'GND'}   
}

# UART Baudrate 입력 함수 (인터페이스마다 독립적으로 설정)
def enter_baudrate(interface_number, baudrates):
    baudrate = {}

    if interface_number is None:
        print("먼저 UART 인터페이스를 선택하십시오.")
        return baudrates

    baudrate = input(f"UART Interface {interface_number}의 Baudrate를 입력하십시오: ").strip()
    baudrates[interface_number] = int(baudrate)
    print(f"UART Interface {interface_number}의 Baudrate {baudrate}가 설정되었습니다.")
    return baudrates

# UART 통신 확인 및 부팅 로그 출력 함수
def check_uart_connection(interface_number, baudrates):
    if interface_number is None:
        print("UART 인터페이스가 설정되지 않았습니다. 먼저 인터페이스를 선택하십시오.")
        return

    correct_pins = {
        #1: {'TX': 4, 'RX': 3},
        #2: {'TX': 3, 'RX': 4},
        1: {'TX': 1, 'RX': 2}
    }   

    # 각 인터페이스에 맞는 올바른 Baudrate 값을 설정
    correct_baudrates = {
        #1: 115200,
        #2: 9600,
        1: 57600
    }
    
    selected_baudrate = baudrates.get(interface_number, None)

    if selected_baudrate is None:
        print(f"UART Interface {interface_number}의 Baudrate가 설정되지 않았습니다.")
        return

    print(f"현재 선택된 UART Interface: {interface_number}")
    
    tx_pin = int(input("TX 핀 번호를 입력하십시오: ").strip())
    time.sleep(1)
    rx_pin = int(input("RX 핀 번호를 입력하십시오: ").strip())
    time.sleep(1)

    if validate_uart_connection(tx_pin, rx_pin, correct_pins[interface_number]['TX'], correct_pins[interface_number]['RX'], selected_baudrate, correct_baudrates[interface_number]):
    	
        display_boot_log(interface_number)
    else:
        display_garbled_output()

# 메인 메뉴 출력 함수
def print_main_menu(interface_number, black_pin, red_pin, mode, baudrate):
    clear_screen()
    
    print("================================")
    print("             M E N U            ")
    print("================================")
    print("1. UART Interface 선택")
    print("2. Multimeter Mode(DCV) 선택")
    print("3. 리드선 연결")
    print("4. Multimeter 확인")
    print("5. Baudrate 값 입력")
    print("6. Check UART")
    print("7. Exit")
    print("================================")

    # 선택된 값들을 메뉴 하단에 표시
    print("\n현재 선택된 값들:")
    print(f"UART Interface: {interface_number if interface_number else 'None'}")
    print(f"Multimeter Mode: {mode if mode else 'None'}")
    print(f"Black : {black_pin if black_pin else 'None'}, Red : {red_pin if red_pin else 'None'}")
    print(f"Baudrate: {baudrate if baudrate else 'None'}")
    print("================================\n")



def print_intro():
    print_uart_art()

    text = ''' 

    '''

# 메인 메뉴에서 사용자 선택을 처리하는 함수
def handle_menu_choice():
    multimeter = Multimeter()
    interface_number = None
    black_pin = None
    red_pin = None
    baudrate = {}  # 빈 딕셔너리로 초기화
    mode = None
    
    while True:
        print_main_menu(interface_number, black_pin, red_pin, mode, baudrate)
        choice = input("원하는 작업을 선택하십시오: ").strip()

        if choice == '1':
            interface_number = uart_interface_selection()  # UART 인터페이스 선택
            input("메인 메뉴로 돌아가려면 Enter를 누르십시오.")
        elif choice == '2':
            choose_measurement_mode(multimeter)  # 멀티미터 모드 선택
            mode = multimeter.mode
            input("메인 메뉴로 돌아가려면 Enter를 누르십시오.")
        elif choice == '3':
            black_pin, red_pin = connect_probes(interface_number)  # 리드선 연결
            input("메인 메뉴로 돌아가려면 Enter를 누르십시오.")
        elif choice == '4':
            if black_pin is None or red_pin is None or interface_number is None or multimeter.mode is None:
                print("먼저 모드 설정, 리드선 연결, 그리고 인터페이스를 설정하십시오.")
            else:
                display_measurement(multimeter, black_pin, red_pin, interface_number)  # 측정값 확인
            input("메인 메뉴로 돌아가려면 Enter를 누르십시오.")
        elif choice == '5':
            baudrate = enter_baudrate(interface_number, baudrate)  # Baudrate 입력
            input("메인 메뉴로 돌아가려면 Enter를 누르십시오.")
        elif choice == '6':
            if interface_number is not None:
                check_uart_connection(interface_number, baudrate)
                
                #input("메인 메뉴로 돌아가려면 Enter를 누르십시오.")
            else:
                print("먼저 인터페이스를 선택하십시오.")
            input("메인 메뉴로 돌아가려면 Enter를 누르십시오.")
        elif choice == '7':
            print("프로그램을 종료합니다.")
            break
        else:
            print("올바른 옵션을 선택하십시오.")
            input("메인 메뉴로 돌아가려면 Enter를 누르십시오.")

if __name__ == "__main__":
    print_uart_art()
    handle_menu_choice()

