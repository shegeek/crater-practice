/* copied and pasted from OGL_27.pdf */

/* sweepvol.c */
/* Purpose: Construct a shape and sweep it around the y-axis */
#include <GL/glut.h>    /* includes gl.h */
#include <math.h>
#include <stdlib.h>
#define KEY_ESC 27
#define MAXOUTLINES 30.0 /* maximum number of
/* Global Definitions */
struct point3D{
      float x;
      float y;
      float z;
};

GLfloat angle, angle2;
#define SIN3DEG 0.052335956

/* This contains the vertex data for the lamp outline */
struct point3D v[30] = {
      {0.0, 0.0, 0.0},
      {0.1, 0.0, 0.0},
      {0.2, 0.0, 0.0},
      {0.3, 0.0, 0.0},
      {0.4, 0.0, 0.0},

      {0.5, 0.0, 0.0},
      {0.5, 0.3, 0.0},
      {0.5, 0.4, 0.0},
      {0.4, 0.55, 0.0}, 
      {0.3, 0.7, 0.0},

      {0.2, 0.8, 0.0},
      {0.2, 1.0, 0.0},
      {0.3, 1.0, 0.0},
      {0.4, 1.0, 0.0},
      {0.5, 1.0, 0.0},

      {0.6, 1.0, 0.0},
      {0.7, 1.0, 0.0},
      {0.8, 1.0, 0.0},
      {0.9, 1.0, 0.0},
      {1.0, 1.0, 0.0},

      {0.9, 1.05, 0.0},
      {0.8, 1.1, 0.0},
      {0.7, 1.15, 0.0},
      {0.6, 1.2, 0.0},
      {0.5, 1.25, 0.0},

      {0.4, 1.3, 0.0},
      {0.3, 1.35, 0.0},
      {0.2, 1.4, 0.0},
      {0.1, 1.45, 0.0},
      {0.0, 1.5, }
};

/* and this, the normals */
struct point3D n[29] = {
  {0., -1., 0.},
  {0., -1., 0.},
  {0., -1., 0.},
  {0., -1., 0.},
  {0., -1., 0.},

  {1., 0.,  2 * SIN3DEG},
  {1., 0., 2 * SIN3DEG},
  {0.5547, 0.83205, SIN3DEG},
  {0.5547, 0.83205, SIN3DEG},
  {1.414, 1.414, 1.4 * SIN3DEG},

  {1., 0., 2 * SIN3DEG},
  {0., -1., 0.},
  {0., -1., 0.},
  {0., -1., 0.},
  {0., -1., 0.},

  {0., -1., 0.},
  {0., -1., 0.},
  {0., -1., 0.},
  {0., -1., 0.},
  {0.44721, 0.89441, SIN3DEG},

  {0.44721, 0.89441, SIN3DEG},
  {0.44721, 0.89441, SIN3DEG},
  {0.44721, 0.89441, SIN3DEG},
  {0.44721, 0.89441, SIN3DEG},
  {0.44721, 0.89441, SIN3DEG},

  {0.44721, 0.89441, SIN3DEG},
  {0.44721, 0.89441, SIN3DEG},
  {0.44721, 0.89441, SIN3DEG},
  {0.44721, 0.89441, SIN3DEG}  
};


/* draw outline of lamp */
GLvoid drawOutline( GLvoid )
{
      int loop;
      /* make the front and back sweep of the slice a little wider
       * to minimize rasterization artifacts
       */
      GLfloat sweep = sin((6.05 * M_PI) / 180.0);
      /* each stack needs its own z component according to its angle
       * that is now accounted for in the normal array
       * don't know why three degrees works for the shade,
       * it was the result of a wild guess
       */
/*       GLfloat sway = sin((3.00 * M_PI) / 180.); */


/*       glBegin(GL_LINE_STRIP); */
/*       for( loop=0; loop<30; ++loop) */
/*       { */
/*             glVertex3f( v[loop].x, v[loop].y, v[loop].z ); */
/*       } */
/*       glEnd(); */

      for( loop=0; loop<29; ++loop)
      {
      glBegin(GL_POLYGON);
      glNormal3f(n[loop].x, n[loop].y, n[loop].z);
/*       glNormal3f(n[loop].x, n[loop].y, 0.); */
            glVertex3f( v[loop].x, v[loop].y, v[loop].x * sweep);
            glVertex3f( v[loop + 1].x, v[loop + 1].y, v[loop+1].x * sweep);
	    glNormal3f(n[loop].x, n[loop].y, -n[loop].z);
            glVertex3f( v[loop + 1].x, v[loop + 1].y, -v[loop+1].x * sweep );
            glVertex3f( v[loop].x, v[loop].y, -v[loop].x * sweep);
      glEnd();
      }

}
/* Sweep the outline around the y axis */
GLvoid sweepOutline( GLvoid )
{
      GLint loop;
      GLfloat color[4] = {1., 1., 0., 1.};
/*       color[1] = color[3] = 0.; */

      glLoadIdentity();
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
/*       glColor3f(0., 1., 0.); */
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
      for( loop = 0; loop < 30 ; ++loop )
      {
           float frac = (float)loop / MAXOUTLINES; /* frac will run
from 0 to a bit less than 1 */
            glLoadIdentity();
	    glPushMatrix();
	    glTranslatef(0., -0.75, -4.0);
	    glRotatef(angle2, 1., 0., 0.);
	    glRotatef(angle, 0., 1., 0.);
            glRotatef( 12 * loop, 0.0, 1.0, 0.0 );
/*             glColor3f(1.0 - frac, 0.0, frac); */ /* change the colour to
show up the separate outlines */
/* 	    color[0] = 1.0 - frac; */
/* 	    color[2] = frac; */
/*    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color); */
            drawOutline();
	    glPopMatrix();
      }
      glFlush();
}

void drawIcosahedron(void)
{
  GLfloat color[] = {0., 1., 0., 1.};
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glPushMatrix();
  glTranslatef(0., 0., -4.);
  glRotatef(angle, 0., 1.414 / 2, 1.414 / 2);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  glutSolidTorus(0.25, 0.5, 8, 30);
  glPopMatrix();

}

void display(void)
{
  sweepOutline();
/*   drawIcosahedron(); */
}

void reshape(int width, int height)
{
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
/*       glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 6.0); */
      gluPerspective(45.0, (GLdouble)width/(GLdouble)height, -5.0, 2.0);
      glMatrixMode( GL_MODELVIEW );

}

GLvoid init( GLvoid )
{
  angle = angle2 = 0.;
/*   GLfloat light_color[] = {1.0, 1.0, 1.0, 1.0}; */
  GLfloat light_position[] = {0.0, 0.0, -1.0, 0.0};
  GLfloat specular[] = {1., 1., 1., 1.};

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

       glClearColor( 1.0, 1.0, 1.0, 1.0);
      glLineWidth(3.0);

      glEnable(GL_NORMALIZE);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      glEnable(GL_DEPTH_TEST);
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);

   glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 0.7 * 128.0);

 }


GLvoid keyboard( GLubyte key, GLint x, GLint y)
{
    switch (key) {

    case 'l':
      angle += 5.0;
      if (angle >= 360.0) angle = 0.;
      glutPostRedisplay();
      break;
    case 'r':
      angle -= 5.0;
      if (angle <= 0.0) angle = 360.;
      glutPostRedisplay();
      break;

    case 'u':
      angle2 += 5.0;
      if (angle2 >= 360.0) angle = 0.;
      glutPostRedisplay();
      break;
    case 'd':
      angle2 -= 5.0;
      if (angle2 <= 0.0) angle = 360.;
      glutPostRedisplay();
      break;

    case KEY_ESC:   /* exit when escape key is pressed */
         exit(0);
    }
}
int main( int argc, char *argv[] )
{
      glutInit( &argc, argv );
      glutInitWindowSize(500, 500);
      glutCreateWindow( argv[0] );
      init();
   glutReshapeFunc(reshape);
    glutKeyboardFunc( keyboard );
      glutDisplayFunc( display );
      glutMainLoop();
  return 0;
}
