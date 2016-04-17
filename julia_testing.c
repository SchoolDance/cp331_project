#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void display(void);
void initialize(void);

// Struct for colour of every pixel
typedef struct {
  int r;
  int g;
  int b;
} Colour;
Colour *colours = NULL;     // Globally initialize colour array

int main(int argc, char ** argv) {
  colours = malloc((atoi(argv[1]) - 1) * sizeof(*colours));      // Allocate array of random colours

  srand((unsigned) time(NULL));
  for (int i = 0; i < atoi(argv[1]); i++) {
    colours[i].r = rand() % 255;
    colours[i].g = rand() % 255;
    colours[i].b = rand() % 255;
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(1000, 1000);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("High Definition Julia Set");
  glutDisplayFunc(display);
  initialize();
  glutMainLoop();

  free(colours);
  return 0;
}

// void defineColourArray(Colour **array, int mag) {
//   *array = malloc((mag - 1) * sizeof(*array));
// }

// Sets up some OpenGL nonsense
void initialize(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1000.0, 0.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

// This fn is called by glutDisplayFunc and is what's used to send output to OpenGL shizan
void display(void) {
  typedef GLfloat point2[2];
  int j = 0;
  Colour current;
  point2 p = {0.0, 0.0};

  char *piece; char line[20]; char *array[3];
  FILE *file = fopen("nope.txt", "r");

  // Loops for the length of the file, plotting the points with the respective colours
  while (fgets(line, sizeof(line), file)) {
    piece = strtok(line, " ");
    while (piece != NULL) {
      array[j++] = piece;
      piece = strtok(NULL, " ");
    }
    j = 0;

    p[0] = atof(array[1]);
    p[1] = atof(array[0]);
    current = colours[atoi(array[2])];


    glBegin(GL_POINTS);
    glColor3ub(current.r, current.g, current.b);
    glVertex2fv(p);
    glEnd();
  }
  glFlush();
}
