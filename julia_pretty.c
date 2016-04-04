/*
This code is used to generate the "high" definition image of the Julia sets.

It will use OpenGL and read from an output file of points.
*/

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

// The struct that each pixel will have (colouring)
struct ColourRGB {
  float r;
  float g;
  float b;
};

struct ColourRGB pixels[1000*1000], pattern[999];

void Init() {
  // A bunch more OpenGL init?
  // TODO: Find out what this shit does
  glViewport(0, 0, 1000, 1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0, 1000, 0, 1000);

  int i;
  int r, g, b;

  // TODO: Do the colour thing
  // - how many colours?!
  // - what shall I do?!


}

void julia() {
  // TODO: This is going read points from the file
  // Ideally, I should have a function that reads the file and returns an array [of structs?]

}

void onDisplay() {

  // Clears the buffer before creating image
  glClearColor(1, 1, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  // So, this actually does what I'm looking for? Ok.
  // TODO: Figure out what this does.
  glDrawPixels(1000, 1000, GL_RGB, GL_FLOAT, ...);
  glutSwapBuffers();
}

int main(int argc, char **argv) {

  // Initialize OpenGL
  glutInit(&argc, argv);
  glutInitWindowSize(1000, 1000);         // Width x height
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutCreateWindow('< Julia sets by Scott >');

  Init();

  // Callback for display window
  glutDisplayFunc(...);

  glutMainLoop();           // Actually runs the OpenGL stuff
  return 0;
}
