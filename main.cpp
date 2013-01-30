#include <math.h>
#include <iostream>
#include <vector>

#include "./common.h"
#include "./command.h"

#define PI 3.14159265

using namespace std;

// the main point structure
struct Point {
  double pos[2];
  int color;
};

// the current angle in polar coordinates
double angle;

// true if the pen is up (off) false if the pen is down (on)
bool penUp;

// the current point that the pen (up or down) is located
Point current_point;

// all of the points which we are keeping track of
vector<vector<Point> > points;

void Init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // TODO: Change this call so that the drawing methods work on a canvas
  // that is 200x200 units with the origin (0, 0) at the center of the
  // canvas.
  // DONE
  gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  // TODO: Change to draw according to turtle commands given
  // Draw a red triangle.main.cpp:16:1:

  for (int i = 0; i < points.size(); ++i) {
    // convert from color enum to RGB value
    if (points[i].size() > 0) {
      int color = points[i][0].color;
      switch (color) {
        case 0:
          glColor3f(0.0, 0.0, 0.0);
          break;
        case 1:
          glColor3f(1.0, 0.0, 0.0);
          break;
        case 2:
          glColor3f(0.0, 1.0, 0.0);
          break;
        case 3:
          glColor3f(0.0, 0.0, 1.0);
          break;
      }
    }

    glBegin(GL_LINE_STRIP);

    for (int j = 0; j < points[i].size(); ++j) {
      // put down an actual point
      glVertex2f(points[i][j].pos[0], points[i][j].pos[1]);
    }
    glEnd();
  }
  glFlush();
}

void Interpret(const vector<Command>& commands) {
  // TODO: Interpret each command one-by-one.
  // TODO: Remove output statements as you implement them.
  for (int i = 0; i < commands.size(); ++i) {
    const Command& c = commands[i];
    switch (c.name()) {
    case FORWARD:
      // -------------- FORWARD -----------------------------------------------

      // figure out the position of the next point after this forward
      current_point.pos[0] = current_point.pos[0] +
        c.arg() * cos((angle * PI) / 180);
      current_point.pos[1] = current_point.pos[1] +
        c.arg() * sin((angle * PI) / 180);

      // only if the pen is down, we display this point
      if (!penUp) {
        points.back().push_back(current_point);
      }

       // end
      break;
    case RIGHT:

    // ------------ RIGHT  ----------------------------------------------------
      angle = angle - c.arg();

      if (angle < 0) {
        angle += 360;
      }
      break;
    case LEFT:
    // ----------- LEFT     ---------------------------------------------------

	  angle = angle + c.arg();

	  if (angle > 360) {
		angle -= 360;
	  }

		  break;
    case PEN_UP: {
      penUp = true;
      vector<Point> line1;
      points.push_back(line1);
      break;
    }
    case PEN_DOWN: {
      penUp = false;

      // place a point since the pen is down for a new line
      points.back().push_back(current_point);
      break;
    }
    case COLOR: {
      if (!penUp) {
        points.back().push_back(current_point);
      }

      vector<Point> line2;
      points.push_back(line2);
      if (c.arg() == 0)
         current_point.color = 0;
      if (c.arg() == 1)
         current_point.color = 1;
      if (c.arg() == 2)
         current_point.color = 2;
      if (c.arg() == 3)
         current_point.color = 3;

      if (!penUp) {
        points.back().push_back(current_point);
      }
    }
    break;
    case ORIGIN: {
      current_point.pos[0] = 0.0;
      current_point.pos[1] = 0.0;
      angle = 90;

      // cut continuous line
      vector<Point> line2;
      points.push_back(line2);

      if (!penUp) {
        points.back().push_back(current_point);
      }
    }
    break;
    }
  }
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
      exit(EXIT_SUCCESS);
    break;
  }
}

int main(int argc, char** argv) {
  // added by Hernaldo 1/21/13
  current_point.color = 0;
  current_point.pos[0] = 0;
  current_point.pos[1] = 0;
  // first position is X, second one is Y, using cartesian coords.

  angle = 90;

  penUp = false;

  vector<Point> line;
  points.push_back(line);

  points.back().push_back(current_point);
  // end

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  // DO NOT change the window title.  It is necessary for the screen capture.
  glutCreateWindow("Turtle graphics");
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  Init();

  if (argc < 2) {
    cout << "Usage: turtle filename" << endl;
    exit(1);
  }
  vector<Command> commands = Parse(argv[1]);
  Interpret(commands);

  glutMainLoop();
}
