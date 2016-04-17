#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Colour {
  int r;
  int g;
  int b;
};

struct Colour colours[49];

void myinit(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1000.0, 0.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

void display(void) {
  typedef GLfloat point2[2];
  int j = 0;
  point2 p = {0.0, 0.0};

  char *piece; char line[20]; char *array[3];
  FILE *file = fopen("nope.txt", "r");

  while (fgets(line, sizeof(line), file)) {
    piece = strtok(line, " ");
    while (piece != NULL) {
      array[j++] = piece;
      piece = strtok(NULL, " ");
    }

    j = 0;

    p[0] = atof(array[1]);
    p[1] = atof(array[0]);

    glBegin(GL_POINTS);
    glColor3ub(colours[atoi(array[2])].r, colours[atoi(array[2])].g, colours[atoi(array[2])].b);
    glVertex2fv(p);
    glEnd();
  }
  glFlush();
}

void main(int argc, char ** argv) {
  for (int i = 0; i < atoi(argv[1]); i++) {
    colours[i].r = rand() % 255;
    colours[i].g = rand() % 255;
    colours[i].b = rand() % 255;
  }

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize (1000, 1000);
  glutInitWindowPosition (0, 0);
  glutCreateWindow ("Stuff by scott");
  glutDisplayFunc (display);
  myinit();
  glutMainLoop();
}
