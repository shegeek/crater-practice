


#ifndef _EJECTA_H_
#define _EJECTA_H_

#include <stdbool.h>
#include <GL/glut.h>

typedef struct {
/* intial position stored as polar coordinates relative to crater center */
  GLdouble initpos[2]; /* need to store this, or are dircos enough? as xyz? */
  /* store radius as percent? */
  GLdouble dircos[2];
  GLdouble initvelocity[3];
  GLdouble currpos[3];
} ejparticle;
  

typedef struct {
  ejparticle ** ejectum;
  int numejparticles;
  GLdouble locus[3];  /* needed, or translate there with crater placement? */
  double radius;
  double tcounter;
  double tincrement;
  double grav;
} ejemitter;





#endif
