#include <stdio.h>
#include <stdlib.h>

#include "sim.h"

extern FishDataT Rfish[];
extern FishDataT Gfish[];
extern FishDataT Bfish[];

extern Selected interface;

/* -------------------------------------------------------------- SpeedUp
 * SpeedUp : 選択している種族の速度を増す
 */
void SpeedUp()
{
  switch(interface.select)
  {
    case interface.RED:

      if(Rfish[0].speed < 2.0)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Rfish[i].speed += 0.1;
        }
      }
      break;

    case interface.GREEN:

      if(Gfish[0].speed < 2.0)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Gfish[i].speed += 0.1;
        }
      }

      break;

    case interface.BLUE:

      if(Bfish[0].speed < 2.0)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Bfish[i].speed += 0.1;
        }
      }

      break;
  }
}


/* -------------------------------------------------------------- SpeedDown
 * SpeedDown : 選択している種族の速度を増す
 */
void SpeedDown()
{
  switch(interface.select)
  {
    case interface.RED:

      if(Rfish[0].speed > 0.1)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Rfish[i].speed -= 0.1;
        }
      }
      break;

    case interface.GREEN:

      if(Gfish[0].speed > 0.1)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Gfish[i].speed -= 0.1;
        }
      }

      break;

    case interface.BLUE:

      if(Bfish[0].speed > 0.1)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Bfish[i].speed -= 0.1;
        }
      }

      break;
  }
}


/* ------------------------------------------------------------- SightAngleUp
 * SightAngleUp 視野角を増やす
 */
void SightAngleUp()
{
  switch(interface.select)
  {
    case interface.RED:

      if(Rfish[0].sightangle < 180.0)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Rfish[i].sightangle += 10.0;
        }
        printf("%f\n", Rfish[0].sightangle);
      }

      break;

    case interface.GREEN:

      if(Gfish[0].sightangle < 180.0)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Gfish[i].sightangle += 10.0;
        }
      }

      break;

    case interface.BLUE:

      if(Bfish[0].sightangle < 180.0)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Bfish[i].sightangle += 10.0;
        }
      }

      break;
  }
}

/* ------------------------------------------------------------- SightAngleDown
 * SightAngleDown 視野角を減らす
 */
void SightAngleDown()
{
  switch(interface.select)
  {
    case interface.RED:

      if(Rfish[0].sightangle > 10.0)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Rfish[i].sightangle -= 10.0;
        }
      }

      break;

    case interface.GREEN:

      if(Gfish[0].sightangle > 10.0)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Gfish[i].sightangle -= 10.0;
        }
      }

      break;

    case interface.BLUE:

      if(Bfish[0].sightangle > 10.0)
      {
        for(int i = 0; i < LENGTH; i++)
        {
          Bfish[i].sightangle -= 10.0;
        }
      }

      break;
  }
}

/* ------------------------------------------------------------ SightRangeUp
 * SightRangeUp 視野の長さを増やす
 */


/* ------------------------------------------------------------- SelectType
 * SelectType 選択されている種族の色に変更する関数
 */
void SelectType()
{
  switch(interface.select)
  {
    case interface.RED:
         ColorChange(&interface.color, 1.0, 0.0, 0.0, 1.0);
         break;

    case interface.GREEN:
         ColorChange(&interface.color, 0.0, 1.0, 0.0, 1.0);
         break;

    case interface.BLUE:
         ColorChange(&interface.color, 0.0, 0.0, 1.0, 1.0);
         break;
  }
}

//end of file
