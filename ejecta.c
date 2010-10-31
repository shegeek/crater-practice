
/* WARNING be sure to change rand() to random() for the xscreensaver api */

#include "ejecta.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void reset_ejparticle(ejparticle * redux, GLdouble velocity[3],
		      double maxradius)
{
  GLdouble theta = (double)(rand() % 314) / 50.;
  redux->initpos[0] = theta;
  redux->initpos[1] = (double)(rand() % 100) / 100. * maxradius;
  redux->dircos[0] = cos(theta);
  redux->dircos[1] = sin(theta);
  redux->initvelocity[0] = velocity[0];
  redux->initvelocity[1] = velocity[1];
  redux->initvelocity[2] = velocity[2];
} 

/* initial positions aren't in yet--
 * haven't decided where to or if to add world coords
 */
bool advance_ejparticle(ejparticle * updated, double elapsed, double gconst)
{
  bool islive = true;
  updated->currpos[0] = updated->initvelocity[0] * elapsed * updated->dircos[0];
  updated->currpos[1] = (gconst * elapsed * elapsed) + updated->initvelocity[1] * elapsed;
  if (updated->currpos[1] < 0.) islive = false;
  updated->currpos[2] = updated->initvelocity[2] * elapsed * updated->dircos[1];
  return islive;
}

void reset_ejemitter(ejemitter * redux, GLdouble newlocus[3], 
			    GLdouble newradius)
{
  redux->tcounter = 0;
  redux->locus[0] = newlocus[0];
  redux->locus[1] = newlocus[1];
  redux->locus[2] = newlocus[2];
  redux->radius = newradius;
}

ejemitter * init_ejemitter(int count, double maxradius, 
			   double timestep, double gravconstant)
{
  ejemitter * newemitter;
  newemitter = malloc(sizeof(ejemitter));
  newemitter->ejectum = calloc (sizeof(ejparticle *));
  for (itor = 0; itor < count; itor++)
    {
      newemitter->ejectum[itor] = malloc(sizeof(ejparticle));
    }
  newemitter->numejparticles = count;
  newemitter->radius = maxradius;
  newemitter->tincrement = timestep;
  newemitter->grav = -0.5 * gravconstant;
  return newemitter;
}  

void render_ejparticles(ejparticle ** ejectoids, int numejectoids)
{
  int iterator;
  glBegin(GL_POINTS);
  for (iterator = 0; iterator < numejectoids; iterator++)
    {
    glVertex3f(ejectoids[iterator]->currpos[0],
	       ejectoids[iterator]->currpos[1],
	       ejectoids[iterator]->currpos[2]); 
    }
  glEnd();
}

bool spew_ejecta(ejemitter * ejspew)
{
 /* if any one particle is still live, set to true */
  /* possibly make an int to also tell caller when it's safe to draw mesh */
  /* or, set flag in emitter */
  bool islive = false;
  return islive;
}


void delete_ejemitter(ejemitter * doomed)
{
  int itor;
  for (itor = 0; itor < doomed->numejparticles; itor++)
    {
      free (doomed->ejectum[itor]);
    }
  free(doomed->ejectum);
  free(doomed);
}


/* =================================== */

/* left to do:
 *
 * decide what to do about world coords--
 *   leaning toward leaving them out & translating in caller,
 *   and storing xyz particle coords, keeping dircos and nixing r & theta
 *
 * properly assign initial velocities to particles 
 * so that they disperse properly
 *
 * decide how to send message to caller that it's safe to draw mesh
 *
 * flesh out spew()
 *
 * possibly put in another flag so that 
 *  emitter won't run if not properly reset
 */
