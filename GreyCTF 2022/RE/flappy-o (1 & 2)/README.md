# CHALLENGE DESCRIPTION

### flappy-o
> I know you cheated in flappy-js. This time the game is written in C, I don't think you can cheat so easily. Or can you?
> 
> Show me your skills by getting a score of at least 64.
> Author: daniellimws
> 
> [flappybird](flappybird)

### flappy-o2
> This challenge uses the same binary as `flappy-o`.
> 
> Every 10000 points you reach, you unlock a character of the bonus flag. This is the real test of your skills.
> Author: daniellimws

# INITIAL ANALYSIS

![](flappy1.png)
![](flappy2.png)

Running the program, we are presented with a flappybird like terminal game. 

Everytime the bird passes through the pipes, the score is incremented by 1 and a letter of the flag (for part 1) is printed.

![](flappy3.png)

From what the challenge description mentions, it seems that the flag for part 1 will be presented when we get a score of at least 64, and the flag for part 2 will be presented at every 10000 points we get in the game.

Clearly, it will be quite difficult to manually play this game, so let's start reversing it and see what we can do.

# REVERSING THE BINARY

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  unsigned int v3; // eax
  int v4; // eax
  int v5; // eax

  v3 = time(0LL);
  srand(v3);
  readBest(&bestScore, bestPlayerName);
  initscr();
  curs_set(0);
  keypad(stdscr__NCURSES6_TINFO_5_0_19991023, 1);
  setupColors();
  if ( stdscr__NCURSES6_TINFO_5_0_19991023 )
    v4 = stdscr__NCURSES6_TINFO_5_0_19991023->_maxy + 1;
  else
    v4 = -1;
  row = v4;
  if ( stdscr__NCURSES6_TINFO_5_0_19991023 )
    v5 = stdscr__NCURSES6_TINFO_5_0_19991023->_maxx + 1;
  else
    v5 = -1;
  col = v5;
  if ( row <= 23 || col <= 74 )
    wait_duration = 140000;
  writeInfo(row, col);
  wgetnstr(stdscr__NCURSES6_TINFO_5_0_19991023, playerName, -1);
  drawStarting(row, col);
  refresh();
  noecho();
  wtimeout(stdscr__NCURSES6_TINFO_5_0_19991023, 1);
  birdRow = row / 2;
  birdCol = col / 4;
  pipeCol1 = col;
  pipeCol2 = col;
  pipeCounter = col;
  getNewPipeValue(&crackStart1, &crackFinish1, row);
  getNewPipeValue(&crackStart2, &crackFinish2, row);
  gameLoop(); // <-------- the relevant game code
  writeBest(bestScore, bestPlayerName);
  ggs();
  return 0;
}
```

If we look at the main function, we realize that there is nothing interested as most of the code here merely initializes the pre-game setup and the interface for the game itself.

The only interesting function called is `gameLoop()` which is the part of the code involving the game itself.

```c
__int64 gameLoop(void)
{
  __int64 result; // rax

  while ( 1 )
  {
    result = (unsigned int)isOver;
    if ( isOver )
      break;
    clear();
    processInput(); // take in user input
    drawBird(); // draw bird on screen
    drawStats(); // draw score, bestscore etc
    drawAndUpdatePipes(); // draw pipes
    drawGrass(); // draw grass
    checkAndHandleCollision(); // modify score
    updatePipeIfNeeded(); // redraw pipes
    updateAndDrawFlag(); // draw flag
    ++isScore;
    --pipeCounter;
    refresh();
    usleep(wait_duration); // irrelevant sleep!!
  }
  return result;
}
```

From the `gameLoop` code, we can see the sequence of the code being run continuously.

As of now, our goal looks like this

` ++ score -> score == 64 -> flag1 -> score == 10000*n -> flag2`

so we would want to patch the binary in a way that the score increases indefinitely so we get our flags.

```c
__int64 updateAndDrawFlag(void)
{
  __int64 result; // rax
  int v1; // [rsp+Ch] [rbp-4h]

  mvprintw(3, col / 2 - 20, "%s", flag1);
  mvprintw(4, col / 2 - 20, "%s", (const char *)&flag2);
  v1 = score / 8;
  result = (unsigned int)prevScore;
  if ( score / 8 != prevScore )
  {
    if ( v1 - prevScore != 1 )
      reportCheater();
    if ( (unsigned __int64)v1 <= 0x3F )
      flag1[v1 - 1] = genFlag1(v1 - 1);
    lfsr2(1);
    if ( !(v1 % 10000) && (unsigned __int64)(v1 / 10000) <= 0x19 )
      *((_DWORD *)&flag2 + v1 / 10000 - 1) = genFlag2(v1 - 10000);
    result = (unsigned int)v1;
    prevScore = v1;
  }
  return result;
}
```

The `updateAndDrawFlag` function merely decodes the flag when you reach certain scores, and isn't that interesting as it would be more tedious to reverse this than patch the binary.

```c
unsigned __int64 checkAndHandleCollision(void)
{
  unsigned __int64 result; // rax

  collision = checkCollision(pipeCol1, birdCol, birdRow, crackStart1, crackFinish1);
  result = (unsigned int)collision;
  if ( collision )
  {
    result = (unsigned int)collision;
    if ( collision == 2 )
    {
      isOver = 1;
    }
    else
    {
      result = (unsigned int)isScore;
      if ( isScore )
      {
        ++score;
        result = (unsigned int)bestScore;
        if ( score / 8 > bestScore )
        {
          bestScore = score / 8;
          return (unsigned __int64)strcpy(bestPlayerName, playerName);
        }
      }
    }
  }
  return result;
}
```

If we look inside `checkAndHandleCollision`, this is the function that increments the score.

We could patch this function to remove the checks so as to keep the score increasing and prevent the game from ending.

# PATCHING THE BINARY

Let's identify what we would want to patch out in our `checkAndHandleCollision` function:

```c
unsigned __int64 checkAndHandleCollision(void)
{
  unsigned __int64 result; // rax

  collision = checkCollision(pipeCol1, birdCol, birdRow, crackStart1, crackFinish1);
  result = (unsigned int)collision; 
  if ( collision ) // <-- patch out the if condition
  {
    result = (unsigned int)collision;
    if ( collision == 2 ) // <-- patch out to prevent game from ending
    { 
      isOver = 1; // <-- patch out to prevent game from ending
    }
    else
    {
      result = (unsigned int)isScore;
      if ( isScore )
      {
        ++score;
        result = (unsigned int)bestScore;
        if ( score / 8 > bestScore )
        {
          bestScore = score / 8;
          return (unsigned __int64)strcpy(bestPlayerName, playerName);
        }
      }
    }
  }
  return result;
}
```

Ideally we would want our binary to look like this:

```c
unsigned __int64 checkAndHandleCollision(void)
{
  unsigned __int64 result; // rax

  if ( isScore )
  {
    ++score;
    result = (unsigned int)bestScore;
    if ( score / 8 > bestScore )
    {
        bestScore = score / 8;
        return (unsigned __int64)strcpy(bestPlayerName, playerName);
    }
  }

  return result;
}
```

so that our score will always increase and the game won't end even if we collide with the pipes.

<br>

![](ida1.png)

Looking at the binary in IDA, we would want to patch out the jump at 0x252D by writing the bytes at 0x252D from `74 77` to `90 90`.

It would look something like this:

![](ida3.png)

Next, we will also nop out the 2 instructions at `0x253A` which is the equivelant to `isOver = 1` which would end the program and jump to the end of the function (prevents our score from being incremented).

![](ida4.png)

Lastly we want to patch out the 2 instructions at `0x254C` and `0x254E` to skip the check and just go straight to incrementing our score.

**Essentially you get the idea, patch out everything that prevents us from increasing our score.**

```c
__int64 gameLoop(void)
{
  while ( 1 )
  {
    ...
    if ( isOver )
      break;
	...
  }
  return result;
}
```

We will also patch `break` out from `gameLoop` so our program doesn't end.

![](ida5.png)

nop these 2 instructions and we are done!

# TESTING OUR PATCHES

![](flappy4.png)

If you try to run the program now, you find that the score increases indefinitely as we would like it to. 

However, it takes foreveeeeeeeeeeeer and is unfeasible in getting our flags.

Let's try to patch it to be faster.

# PATCHING PT 2.

```c
void __noreturn gameLoop(void)
{
  while ( 1 )
  {
    clear();
    processInput();
    drawBird();
    drawStats();
    drawAndUpdatePipes();
    drawGrass();
    checkAndHandleCollision(); // increases score
    updatePipeIfNeeded();
    updateAndDrawFlag(); // draw flag if certain score reached
    ++isScore;
    --pipeCounter;
    refresh();
    usleep(wait_duration);
  }
}
```

We look at `gameLoop` once again, there are actually only 2 relevant functions in this entire loop.

In order to make our game print the flag faster, we can patch everything else out!

Let's nop out all the other functions and it will look like this

```c
void __noreturn gameLoop(void)
{
  while ( 1 )
  {
    checkAndHandleCollision();
    updateAndDrawFlag();
    ++isScore;
    --pipeCounter;
  }
}
```

# GETTING OUR FLAG

Now if we run the program, we find that we will actually be stuck at the **Get Ready!!!** screen.

![](flappy5.png)

This is because we actually patched out all the functions that draws the screens.

However, fret not! In the background, our score is increasing continuously and our flag is probably already fully decoded. 

Let's retrieve it in GDB!

![](flag.png)

### FLAG1: grey{y0u_4r3_pr0_4t_7h1s_g4m3_b6d8745a1cc8d51effb86690bf4b27c9}
### FLAG2: grey{y0u_4r3_v3ry_g00d_4t_7h1s_g4m3_c4n_y0u_t34ch_m3_h0w_t0_b3_g00d_ef4bd282d7a2ab1ebdcc3616dbe7afb}

_p.s. you should be able to get the flag almost instantly without having to wait_
