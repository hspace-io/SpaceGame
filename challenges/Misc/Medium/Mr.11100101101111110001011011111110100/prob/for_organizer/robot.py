import hashlib
import struct
import os

COMMANDS = {
    '0x10': 'forward',
    '0x20': 'backward',
    '0x30': 'left',
    '0x40': 'right'
}
COUNTER = 1

def to_hex_byte(value):
    byte_value = struct.pack(">I", value)[-1:]
    hex_value = byte_value.hex()
    return hex_value

def manchester_decode(data: bytes) -> bytes:
    decoded = []
    current_byte = 0
    bit_count = 0

    for byte in data:
        for i in range(0, 8, 2):
            bit_pair = (byte >> (6 - i)) & 0b11
            if bit_pair == 0b01:
                bit = 1
            elif bit_pair == 0b10:
                bit = 0
            else:
                print(f"Invalid signal: byte {byte:08b}")
                return None
            current_byte = (current_byte << 1) | bit
            bit_count += 1
            if bit_count == 8:
                decoded.append(current_byte)
                current_byte = 0
                bit_count = 0

    return bytes(decoded)

def hash_check(command, counter, robot_id, md5_hash):
    hash = str(command) + str(counter) + robot_id
    counter = counter
    hash = hashlib.md5(hash[:].encode('utf-8')).digest()
    if md5_hash == hash:
        return True
    else:
        raise ValueError("Invalid hash")

def parse_signal(signal: bytes, md5_hash: bytes):
    command_code = hex(ord(signal[0:1]))
    if command_code not in COMMANDS:
        raise ValueError("Invalid command")

    command = COMMANDS[command_code]
    counter = to_hex_byte(int.from_bytes(signal[1:2], "big"))
    
    robot_hash = signal[2:6].hex()
    hash_check(command_code[2:], counter, robot_hash, md5_hash)
    return command, counter, robot_hash

def display_grid(robot_pos):
    for y in range(8):
        for x in range(8):
            if [x, y] == robot_pos:
                print('*', end=' ')
            elif [x, y] == [7, 7]:
                print('x', end=' ')
            else:
                print('.', end=' ')
        print()
    print()

def binary_string_to_bytes(binary_string: str) -> bytes:
    byte_array = bytearray()
    for i in range(0, len(binary_string), 8):
        byte = binary_string[i:i+8]
        byte_array.append(int(byte, 2))
    return bytes(byte_array)

def process_signal(encoded_signal: bytes, robot_pos):
    global COUNTER
    decoded_signal = manchester_decode(encoded_signal)
    try:
        signal = decoded_signal[:6]
        md5_hash = decoded_signal[6:]
    except:
        return None

    if decoded_signal is None:
        return None

    try:
        command, counter, robot_hash = parse_signal(signal, md5_hash)
        
    except ValueError as e:
        print(f"Parsing error: {e}")
        return None
    counter = '0x' + counter
    if int(counter, 16) != (COUNTER & 0xFF):
        print("Wrong Counter")
        return None
    else:
        COUNTER += 1
        #COUNTER = COUNTER


    if command == 'forward' and robot_pos[1] > 0:
        robot_pos[1] -= 1
    elif command == 'backward' and robot_pos[1] < 8:
        robot_pos[1] += 1
    elif command == 'left' and robot_pos[0] > 0:
        robot_pos[0] -= 1
    elif command == 'right' and robot_pos[0] < 8:
        robot_pos[0] += 1
    else:
        print("Invalid move detected, terminating program.")
        return None

    #os.system('clear')
    display_grid(robot_pos)

    if robot_pos == [7, 7]:
        print("Robot has reached the target position. Game Over!")
        if int(counter, 16) < 10:
            print("HSPACE{n0w_y0u_c4n_4n41yz3_th3_wir313ss_sign41!}")
            return None
        else:
            print("Your Counter is over 10!")
        return None

    return robot_pos

# Example usage
if __name__ == "__main__":
    # Initial robot position
    robot_pos = [0, 0]
    robot_identifier = "Robot123"
    robot_hash = hashlib.sha256(robot_identifier.encode()).hexdigest()[:8]

    # Display initial grid
    os.system('clear')
    display_grid(robot_pos)

    while True:
        encoded_signal_input = input("Enter the encoded signal as a binary string: ")
        if len(encoded_signal_input) % 8 != 0:
            print("Invalid input length. The binary string length must be a multiple of 8.")
            continue
        try:
            encoded_signal = binary_string_to_bytes(encoded_signal_input)
            new_robot_pos = process_signal(encoded_signal, robot_pos)
            
            if new_robot_pos is None:
                print("Terminating program.")
                break
            robot_pos = new_robot_pos
        except ValueError as e:
            print(f"Invalid input. Please enter a valid binary string. {e}")