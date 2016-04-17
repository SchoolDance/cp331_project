#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generateColours(int);

struct Colour {
  int r;
  int g;
  int b;
};

struct Colour colours[49];

int main(int argc, char **argv) {
  char const* const filename = argv[1];
  FILE *file = fopen(filename, "r");
  char line[20];
  char *piece;
  char *array[3];
  int j = 0;
  srand((unsigned) time(NULL));

  generateColours(atoi(argv[2]));

  // while (fgets(line, sizeof(line), file)) {
  //   piece = strtok(line, " ");
  //   while (piece != NULL) {
  //     array[j++] = piece;
  //     piece = strtok(NULL, " ");
  //   }
  //
  //   printf("%f \t %f \t %f \n", atof(array[0]), atof(array[1]), atof(array[2]));
  //
  // }

  fclose(file);
  return 0;
}

void generateColours(int numColours) {
  for (int i = 0; i < numColours; i++) {
    colours[i].r = rand() % 255;
    colours[i].g = rand() % 255;
    colours[i].b = rand() % 255;
  }
  printf("%d ", colours[10].r);
}
