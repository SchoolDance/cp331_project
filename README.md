# Parallel Julia Set Generation

To compile:

```bash
➜  gcc julia_draw.c -o julia_draw -lglut -lGL -lGLU
➜  ./julia_draw 50
```

Right now I am just using `julia_draw.c` as a benchmark for the production code. Just want to make sure everything is ironed out before we merge.
