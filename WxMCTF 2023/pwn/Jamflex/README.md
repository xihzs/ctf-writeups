# Jamflex

> jamflex made this login system for mgci kids l0l good luck getting in
>
> 🦀$11🦀
>
> NOTE: the solution is not brute force :)

## Static Analysis

This challenge contains seemingly logical (but broken) code. 

The code seems to check try to open a file `~/password.txt`, however since it passes in a relative path which is not supported, it will always return -1. 

This sets `fd=0` which is used in the call to the `read` function, resulting in the program unintentionally taking user input as pw1.

Since the flag is printed when the value of `pw2` and `pw1` is identical, we can simply provide any string of our choice for `pw1`, and the numerical counterpart by adding the character code for all 10 chracters of `pw1` as `pw2`.

```
i.e.
pw1 = "A"*10       = AAAAAAAAAA
pw2 = ord("A")*10  = 650
```

## Solution

```python
p = process("./jamflex")
p.sendline(b"AAAAAAAAAA")
p.sendline(b"username: ", b"hello")
p.sendline(b"password: ", b"1234567890")
p.interactive()
```

_remark: wut da fuq?_
