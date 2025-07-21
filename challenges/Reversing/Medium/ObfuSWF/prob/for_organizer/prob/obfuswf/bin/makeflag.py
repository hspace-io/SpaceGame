BASE32_ALPHABET = "ervngpacwidfbuzhklmhso_954207381"
key = 'spaceWar'

def decode(encoded_str):
    encoded_str = encoded_str.rstrip('=')
    bits = 0
    value = 0
    output = []
    
    for char in encoded_str:
        if char not in BASE32_ALPHABET:
            raise ValueError(f"Invalid Base32 character: {char}")
        value = (value << 5) | BASE32_ALPHABET.index(char)
        bits += 5
        while bits >= 8:
            output.append((value >> (bits - 8)) & 0xFF)
            bits -= 8
    
    return bytes(output)

def encode(data):
    bits = 0
    value = 0
    output = []

    for byte in data:
        value = (value << 8) | byte
        bits += 8
        while bits >= 5:
            output.append(BASE32_ALPHABET[(value >> (bits - 5)) & 0b11111])
            bits -= 5
    if bits > 0:
        output.append(BASE32_ALPHABET[(value << (5 - bits)) & 0b11111])
    
    while len(output) % 8 != 0:
        output.append("=")
    return "".join(output)

def bitswap(data):
    out = []
    for b in data:
        result = 0
        for i in range(8):
            result = (result << 1) | ((b >> i) & 1)
        out.append(result)
    return out

def xor(a, b):
    result = []
    for i in range(len(a)):
        result.append(a[i] ^ ord(b[i % len(b)]))
    return result

if __name__ == "__main__":
    flag = 'r3v3r51ng_7h3_5wf_0bfu5c4710n_32'
    flag = decode(flag)
    flag = bitswap(flag)
    flag = xor(flag, key)

    print(flag)

    # flag = [131, 210, 106, 164, 162, 243, 124, 13, 168, 96, 219, 14, 198, 186, 129, 129, 143, 253, 186, 62]
    # flag = xor(flag, key)
    # flag = bitswap(flag)
    # flag = encode(flag)

    # print(f'hspace{{{flag}}}')