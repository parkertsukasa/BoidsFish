#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "platform.h"

#include "util.h"


/*--------
 * 
 */
float Random( float rmin, float rmax ){
    double r;
    r = (double)rand()/(double)RAND_MAX;
    r = r * ( rmax - rmin ) + rmin;
    return r;
}

float Gaussian(float min, float max)
{
  float gauss = 0.0;

  for(int i = 0; i < 12; i++)
  {
    gauss += Random(min, max);
  }

  return gauss / 12;
}

//end of file
