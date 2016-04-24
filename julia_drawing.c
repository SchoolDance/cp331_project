#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lodepng.h"

void encodeOneStep(const char *, const unsigned char *, unsigned, unsigned);

typedef struct {
  int r, g, b;
} Colour;

// The array for the colours
Colour *colours = NULL;

int main(int argc, char **argv) {
  unsigned width = 3000, height = 3000;
  unsigned char *image = malloc(width * height * 4);

  Colour current;
  int numColours = 50;
  colours = malloc(numColours * sizeof(*colours));

  srand((unsigned) time(NULL));
  for (int i = 0; i < numColours; i++) {
    colours[i].r = rand() % 255;
    colours[i].g = rand() % 255;
    colours[i].b = rand() % 255;
  }

  char *piece; char line[20]; char *array[3]; int j = 0; int row, col;
  FILE *file = fopen("testing.txt", "r");

  while (fgets(line, sizeof(line), file)) {
    piece = strtok(line, " ");
    while (piece != NULL) {
      array[j++] = piece;
      piece = strtok(NULL, " ");
    }
    j = 0;

    row = atoi(array[0]);
    col = atoi(array[1]);
    current = colours[atoi(array[2]) % 50];

    image[4 * width * row + 4 * col + 0] = current.r;
    image[4 * width * row + 4 * col + 1] = current.b;
    image[4 * width * row + 4 * col + 2] = current.g;
    image[4 * width * row + 4 * col + 3] = 255;
  }

  encodeOneStep("testing.png", image, width, height);
  free(image);
  return 0;
}

void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(filename, image, width, height);

  /*if there's an error, display it*/
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}
