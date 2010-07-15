/*  */

/* Win32 calling conventions. */
#ifndef CALLBACK
#define CALLBACK
#endif


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

GLfloat angle, angle2;

#define OVERTS 90
#define IVERTS 30
GLdouble outervertices[OVERTS][3];
GLdouble hole1vertices[IVERTS][3];
GLdouble hole2vertices[IVERTS][3];
#define ORADIUS 20.0
#define HOLE1RADIUS 2.5
#define HOLE2RADIUS 4.5

GLuint dlist;

/* tessellator callbacks, from tess.c in the redbook examples ---- */
void CALLBACK beginCallback(GLenum which)
{
   glBegin(which);
}

void CALLBACK errorCallback(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf(stderr, "Tessellation Error: %s\n", (char *) estring);
   exit(0);
}

void CALLBACK endCallback(void)
{
   glEnd();
}

void CALLBACK vertexCallback(GLvoid *vertex)
{
   const GLdouble *pointer;

   pointer = (GLdouble *) vertex;
   glColor3dv(pointer+3);
   glVertex3dv(pointer);
}

void CALLBACK combineCallback(GLdouble coords[3],
                     GLdouble *vertex_data[4],
                     GLfloat weight[4], GLdouble **dataOut )
{
   GLdouble *vertex;
   int i;

   printf("\nNew vertex: %f, %f, %f\n", coords[0], coords[1], coords[2]);
/*    printf ("Vertex data:                       Weight:\n"); */
/*    for(i = 0; i < 4; i++) */
/*      { */
/*        printf ("%f, %f, %f -- %f\n", vertex_data[i][0], vertex_data[i][1], */
/*                vertex_data[i][2], vertex_data[i][3], weight[i]); */
/*      } */

/*    vertex = (GLdouble *) malloc(6 * sizeof(GLdouble)); */
   vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));

   vertex[0] = coords[0];
   vertex[1] = coords[1];
   vertex[2] = coords[2];
/*    for (i = 3; i < 6; i++) */
/*       vertex[i] = weight[0] * vertex_data[0][i] */
/*                   + weight[1] * vertex_data[1][i] */
/*                   + weight[2] * vertex_data[2][i] */
/*                   + weight[3] * vertex_data[3][i]; */
   *dataOut = vertex;
}


/* --------------------------------------------------------------- */

void calculate_vertices(void)
{
  GLfloat theta, thetaincrement;
  int Count;
  float hole1xoffset = 2.0;
  float hole1yoffset = -1.5;
  float hole2xoffset = -3.0;
  float hole2yoffset = 2.5;
  double groundlevel = -8.0;

  thetaincrement = 2. * M_PI / (float)OVERTS;
 for(Count=0; Count<OVERTS; Count++)
    {
      theta = Count * thetaincrement;
      outervertices[Count][0] = ORADIUS * cos(theta);
      outervertices[Count][1] = groundlevel;
      outervertices[Count][2] = ORADIUS * sin(theta);
    }
  thetaincrement = 2. * M_PI / (float)IVERTS;
 for (Count = 0; Count < IVERTS; Count++)
	{
	  /* holes wind clockwise, so that
	   * their winding number is still less than 1 where they intersect
	   */
	  theta = (IVERTS - Count) * thetaincrement;
      hole1vertices[Count][0] = HOLE1RADIUS * cos(theta) + hole1xoffset;
      hole1vertices[Count][1] = groundlevel;
      hole1vertices[Count][2] = HOLE1RADIUS * sin(theta) + hole1yoffset;
      hole2vertices[Count][0] = HOLE2RADIUS * cos(theta) + hole2xoffset;
      hole2vertices[Count][1] = groundlevel;
      hole2vertices[Count][2] = HOLE2RADIUS * sin(theta) + hole2yoffset;

	}
}

GLvoid init(GLvoid)
{
   GLUtesselator *tobj;
   GLfloat light_position[] = {0.0, -20., 20.0, 0.0};
  GLfloat specular[] = {1., 1., 1., 1.};
      GLfloat color[4] = {1., 1., 1., 1.};
      int i;

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

       glClearColor( 0.0, 0.0, 0.0, 1.0);

       angle = angle2 = 0.0;
 
 /*      glEnable(GL_NORMALIZE); */
/*       glCullFace(GL_BACK); */
/*       glFrontFace(GL_CCW); */
/*       glEnable(GL_CULL_FACE); */
      glEnable(GL_DEPTH_TEST);
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
/*    glEnable(GL_COLOR_MATERIAL); */
/*    glShadeModel(GL_SMOOTH); */

      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
/*    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); */
      glColor4fv(color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 0.7 * 128.0);


  calculate_vertices();

 tobj = gluNewTess();
 gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);

/*  gluTessCallback(tobj, GLU_TESS_VERTEX, */
/*                    (GLvoid (CALLBACK*) ()) &vertexCallback); */
   gluTessCallback(tobj, GLU_TESS_VERTEX,
                   (GLvoid (CALLBACK*) ()) &glVertex3dv);
   gluTessCallback(tobj, GLU_TESS_BEGIN,
                   (GLvoid (CALLBACK*) ()) &beginCallback);
   gluTessCallback(tobj, GLU_TESS_END,
                   (GLvoid (CALLBACK*) ()) &endCallback);
   gluTessCallback(tobj, GLU_TESS_ERROR,
                   (GLvoid (CALLBACK*) ()) &errorCallback);
   gluTessCallback(tobj, GLU_TESS_COMBINE,
                   (GLvoid (CALLBACK*) ()) &combineCallback);



  dlist = glGenLists(1);
  glNewList(dlist, GL_COMPILE);
  gluTessBeginPolygon(tobj, NULL);
  gluTessBeginContour(tobj);
  for (i = 0; i < OVERTS; i++)
    {
/*      glNormal3f(0., 0., 1.); */
      gluTessVertex(tobj, outervertices[i], outervertices[i]);
    }
  gluTessEndContour(tobj);
  gluTessBeginContour(tobj);
  for (i = 0; i < IVERTS; i++)
    {
/*      glNormal3f(0., 0., 1.); */
      gluTessVertex(tobj, hole1vertices[i], hole1vertices[i]);
    }
  gluTessEndContour(tobj);
  gluTessBeginContour(tobj);
  for (i = 0; i < IVERTS; i++)
    {
/*      glNormal3f(0., 0., 1.); */
      gluTessVertex(tobj, hole2vertices[i], hole2vertices[i]);
    }
  gluTessEndContour(tobj);
  gluTessEndPolygon(tobj);

  glEndList();
  gluDeleteTess(tobj);

/*   glLightfv(GL_LIGHT0, GL_POSITION, light_position); */
  glTranslatef(0., 0., -35.);

}


void reshape(int width, int height)
{
/*   glLoadIdentity(); */
/*     GLfloat light_position[] = {0.0, 2., 1.0, 0.0}; */
/*   glLightfv(GL_LIGHT0, GL_POSITION, light_position); */
/*   glTranslatef(0., -0.5, -2.); */

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      gluPerspective(45.0, (GLdouble)width/(GLdouble)height, 1., 500.0);
      glMatrixMode( GL_MODELVIEW );

}



GLvoid display(GLvoid)
{
  int i;
  GLfloat coloroffset;
  GLfloat light_position[] = {0.0, 2., 1.0, 0.0};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*   glLoadIdentity(); */

  glPushMatrix();
            glRotatef(angle2, 1., 0., 0.);
            glRotatef(angle, 0., 1., 0.);
/*   glLightfv(GL_LIGHT0, GL_POSITION, light_position); */
 
  glCallList(dlist);


 glPopMatrix();
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
      if (angle2 >= 360.0) angle2 = 0.;
      glutPostRedisplay();
      break;
    case 'd':
      angle2 -= 5.0;
      if (angle2 <= 0.0) angle2 = 360.;
      glutPostRedisplay();
      break;

    case 27:   /* exit when escape key is pressed */
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

