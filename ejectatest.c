
/*
 * 3-D gear wheels.  This program is in the public domain.
 *
 * Command line options:
 *    -info      print GL implementation information
 *    -exit      automatically exit after 30 seconds
 *
 *
 * Brian Paul
 */

/* Conversion to GLUT by Mark J. Kilgard */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include "ejecta.h"
#include <unistd.h>  /* for usleep() */
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265
#endif

static GLint ground;

static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static ejemitter *eruption = NULL;
static bool erupting;


restart_explosion()
{
  reset_ejemitter(eruption, 1.);
  erupting = true;
}

static void
draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
    glRotatef(view_rotx, 1.0, 0.0, 0.0);
    glRotatef(view_roty, 0.0, 1.0, 0.0);
    glRotatef(view_rotz, 0.0, 0.0, 1.0);

      glCallList(ground);

      glTranslatef(-3.0, -2.0, -2.0);

  if (erupting)
    {
      erupting = spew_ejecta(eruption);
    }
  else
    {
      restart_explosion();
    }

  glPopMatrix();

  glFinish();
  glutSwapBuffers();
}


static void
idle(void)
{
  usleep(30000);
  glutPostRedisplay();
}

/* change view angle, exit upon ESC */
/* ARGSUSED1 */
static void
key(unsigned char k, int x, int y)
{
  switch (k) {
  case 'z':
    view_rotz += 5.0;
    break;
  case 'Z':
    view_rotz -= 5.0;
    break;
  case 27:  /* Escape */
    exit(0);
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

/* change view angle */
/* ARGSUSED1 */
static void
special(int k, int x, int y)
{
  switch (k) {
  case GLUT_KEY_UP:
    view_rotx += 5.0;
    break;
  case GLUT_KEY_DOWN:
    view_rotx -= 5.0;
    break;
  case GLUT_KEY_LEFT:
    view_roty += 5.0;
    break;
  case GLUT_KEY_RIGHT:
    view_roty -= 5.0;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

/* new window size or exposure */
static void
reshape(int width, int height)
{
  GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
/*   glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0); */
  gluPerspective(20., 1/h, 5., 60.);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -30.0);
}

static void
init(int argc, char *argv[])
{
  static GLfloat pos[4] = {5.0, 15.0, 10.0, 0.0};
/*   static GLfloat pos[4] = {0.0, 15.0, 0.0, 0.0}; */
  static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0};
  static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0};
  static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0};
    GLfloat fullon[] = {1., 1., 1., 1.};
   static GLfloat shininess[] = {128.};
   GLint i;



  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, fullon);
  glLightfv(GL_LIGHT0, GL_SPECULAR, fullon);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
   glMaterialfv(GL_FRONT, GL_SPECULAR, fullon);
  glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

      glEnable(GL_POINT_SMOOTH);
      glPointSize(2.0);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  

  /* make the gears */
  ground = glGenLists(1);
  glNewList(ground, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
/*     glFrontFace(GL_CW); */
    glBegin(GL_QUADS);
    glVertex3f(-10., 0., -10.);
    glVertex3f(-10., 0., 10.);
    glVertex3f(10., 0., 10.);
    glVertex3f(10., 0., -10.);
    glEnd();
/*     glFrontFace(GL_CCW); */
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
  glEndList();


  eruption = init_ejemitter(500, 0.01, 10.);
  restart_explosion();
  glEnable(GL_NORMALIZE);
}

static void 
visible(int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc(idle);
  else
    glutIdleFunc(NULL);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Test for Craters Particle System");
  init(argc, argv);

  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutVisibilityFunc(visible);

  glutMainLoop();
  return 0;        
}
