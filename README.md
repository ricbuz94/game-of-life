# Conway's game of life

My simple implementation of Conway's game of life in C.

### Rules

1.  Any live cell with fewer than two live neighbours dies, as if by
    underpopulation. LIVE -> DEAD
2.  Any live cell with two or three live neighbours lives on to the next
    generation. LIVE -> LIVE
3.  Any live cell with more than three live neighbours dies, as if by
    overpopulation. LIVE -> DEAD
4.  Any dead cell with exactly three live neighbours becomes a live cell, as
    if by reproduction. DEAD -> LIVE

...or more concise:

1.  Any live cell with two or three live neighbours survives. LIVE -> LIVE
2.  Any dead cell with three live neighbours becomes a live cell. DEAD -> LIVE
3.  All other live cells die in the next generation. Similarly, all other dead
    cells stay dead. LIVE,DEAD -> DEAD

DEAD -> □
LIVE -> ■

### Run it in the terminal (clang example on macOS)

```
clang main.cc -O3 -o main && ./main
```
