#include <stdlib.h>
#include <stdio.h>

#include "platform.h"

#include "common.h"

#include "sim.h"

extern SimDataT simdata;
extern CamDataT cam;
extern CamObj camobj;
extern WindowDataT window;

/*---------------------------------------------------------------- Viewing
 * Viewing:
 *--------*/
void Viewing( void )
{
  if(cam.up == 2)
  {
      cam.pos.x = camobj.pos.x;
      cam.pos.y = camobj.pos.y;
      cam.pos.z = camobj.pos.z;
      cam.rot.x = 0.0;
      cam.rot.y = 0.0;
      cam.rot.z = 0.0;
      gluLookAt( cam.pos.x,  cam.pos.y,  cam.pos.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
  }
  else if(cam.up == 1)
  {      
      cam.pos.x = 0.0;
      cam.pos.y = 0.0;
      cam.pos.z = 175.0;
      cam.rot.x = 0.0;
      cam.rot.y = 0.0;
      cam.rot.z = 0.0;
      gluLookAt( cam.pos.x,  cam.pos.y,  cam.pos.z, cam.rot.x, cam.rot.y, cam.rot.z, 0.0, 1.0, 0.0 );
  }
  else 
  {      
      cam.pos.x = 0.0;
      cam.pos.y = 100.0;
      cam.pos.z = 0.01;
      cam.rot.x = 0.0;
      cam.rot.y = 0.0;
      cam.rot.z = 0.0;
      gluLookAt( cam.pos.x,  cam.pos.y,  cam.pos.z, cam.rot.x, cam.rot.y, cam.rot.z, 0.0, 1.0, 0.0 );
  }
}
/*---------------------------------------------------------------- Projection
 * Projection:
 *--------*/
void Projection( void )
{
	//---- begin projection transformation
    //gluPerspective( 45.0, window.aspect, simdata.clip_near, simdata.clip_far );
    glOrtho (AQUARIUM_MIN - 33.2, AQUARIUM_MAX + 33.2, AQUARIUM_MIN - 2, AQUARIUM_MAX + 2, 1.0, 300);
}

//end of file
