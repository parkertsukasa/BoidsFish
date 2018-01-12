#include <stdlib.h>
#include <stdio.h>

#include "platform.h"

#include "common.h"

#include "sim.h"

extern SimDataT simdata;
extern CamDataT cam;
extern WindowDataT window;

/*---------------------------------------------------------------- Viewing
 * Viewing:
 *--------*/
void Viewing( void )
{
  if(cam.up)
  {
      cam.pos.x = 75.0;
      cam.pos.y = 100.0;
      cam.pos.z = 90.0;
      cam.rot.x = 0.0;
      cam.rot.y = 0.0;
      cam.rot.z = 0.0;
  }
  else
  {      
      cam.pos.x = 0.0;
      cam.pos.y = 175.0;
      cam.pos.z = 10.0;
      cam.rot.x = 0.0;
      cam.rot.y = 0.0;
      cam.rot.z = 0.0;
  }

    gluLookAt( cam.pos.x,  cam.pos.y,  cam.pos.z, cam.rot.x, cam.rot.y, cam.rot.z, 0.0, 1.0, 0.0 );
}
/*---------------------------------------------------------------- Projection
 * Projection:
 *--------*/
void Projection( void )
{
	//---- begin projection transformation
    gluPerspective( 45.0, window.aspect, simdata.clip_near, simdata.clip_far );
}
