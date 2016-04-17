/*
This code is used to generate the "high" quality images of the Julia sets.

It will use OpenGL and read from an output file of points.
*/

#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

void init();
void julia();

// The struct that each pixel will have (colouring)
struct ColourRGB {
  float r;
  float g;
  float b;
};

struct ColourRGB pixels[1000*1000], pattern[999];

void init() {
  // TODO: Find out what this shit does
  // glClearColor(1.0, 1.0, 1.0, 1.0);
  // glViewport(0, 0, 1000, 1000);
  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
  // glMatrixMode(GL_PROJECTION);
  // gluOrtho2D(0, 1000, 0, 1000);

  int i;
  int r, g, b;

  // TODO: Do the colour thing
  // Stick the same amount for now?

  // This loops defaults each pixel colour to white
  for (i = 0; i < 1000*1000; i++) {
    pixels[i].r = 1;
    pixels[i].g = 1;
    pixels[i].b = 1;
  }

  // Initializes all the colours, this only gives us 729
  for (r = 0.1; r <= 0.9; r = r + 0.1) {
    for (g = 0.1; g <= 0.9; g = g + 0.1) {
      for (b = 0.1; b <= 0.9; b = b + 0.1) {
        // pattern[i].r = b;
        // pattern[i].g = r;
        // pattern[i].b = g;
      }
    }
  }

  // Sets the rest
  // for (i = 0; i <= 999; i++) {
  //   pattern[i].r = 1;
  //   pattern[i].g = 1;
  //   pattern[i].b = 1;
  // }
}

void julia() {
  // TODO: This is going read points from the file and plots them

  typedef GLfloat point2[2];
  int j = 0, k, w = 0;
  point2 p = {0.0, 0.0};


  // char const* const filename = argv[1];       // Grab filename from input
  FILE *file = fopen("nope.txt", "r");
  char line[10];
  char *entry;
  char *points[3];

  // glClear(CL_COLOR_BUFFER_BIT);
  while (fgets(line, sizeof(line), file) && j != 1000) {
    entry = strtok(line, " ");
    while (entry != NULL) {
      points[w++] = entry;
      entry = strtok(NULL, " ");
    }

    p[0] = atof(points[1]);
    p[1] = atof(points[2]);
    w = 0;
    j++;
    printf("%f\t%f\n", p[0], p[1]);
    glBegin(GL_POINTS);
    glColor3f(2.0f, 0.5f, 1.0f);
    glVertex2fv(p);
    glEnd();
  }
  glFlush();
}

void myinit(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1000.0, 0.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {

  // Initialize OpenGL
  glutInit(&argc, argv);
  glutInitWindowSize(1000, 1000);         // Width x height
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  // init();
  glutCreateWindow("Julia Sets by Scott");
  glutDisplayFunc(julia);     // This function plots points and shit
  myinit();
  glutMainLoop();           // Actually runs the OpenGL stuff
  return 0;
}
