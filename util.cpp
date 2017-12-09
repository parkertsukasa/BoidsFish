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

