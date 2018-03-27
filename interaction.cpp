#include <stdio.h>
#include <stdlib.h>

#include "sim.h"

extern Parameter Rparam;
extern Parameter Gparam;
extern Parameter Bparam;

extern Selected ui;

/* -------------------------------------------------------------- SpeedUp
 * SpeedUp : 選択している種族の速度を増す
 */
void SpeedUp()
{
  switch(ui.select)
  {
    case RED:

      if(Rparam.speed_max < 0.3)
        Rparam.speed_max += 0.02;

      break;

    case GREEN:

      if(Gparam.speed_max < 0.3)
        Gparam.speed_max += 0.02;

      break;

    case BLUE:

      if(Bparam.speed_max < 0.3)
        Bparam.speed_max += 0.02;

      break;

    case WHITE:

      break;
  }
}


/* -------------------------------------------------------------- SpeedDown
 * SpeedDown : 選択している種族の速度を増す
 */
void SpeedDown()
{
  switch(ui.select)
  {
    case RED:

      if(Rparam.speed_max < 0.3)
        Rparam.speed_max -= 0.02;

      break;

    case GREEN:

      if(Gparam.speed_max < 0.3)
        Gparam.speed_max -= 0.02;

      break;

    case BLUE:

      if(Bparam.speed_max < 0.3)
        Bparam.speed_max -= 0.02;

      break;

    case WHITE:

      break;
  }
}


/* ------------------------------------------------------------- SightAngleUp
 * SightAngleUp 視野角を増やす
 */
void SightAngleUp()
{
  switch(ui.select)
  {
    case RED:

      if(Rparam.sightangle < 180.0)
        Rparam.sightangle += 10.0;

      break;

    case GREEN:

      if(Gparam.sightangle < 180.0)
        Gparam.sightangle += 10.0;

      break;

    case BLUE:

      if(Bparam.sightangle < 180.0)
        Bparam.sightangle += 10.0;

      break;

    case WHITE:

      break;
  }
}

/* ------------------------------------------------------------- SightAngleDown
 * SightAngleDown 視野角を減らす
 */
void SightAngleDown()
{
  switch(ui.select)
  {
    case RED:

      if(Rparam.sightangle < 180.0)
        Rparam.sightangle -= 10.0;

      break;

    case GREEN:

      if(Gparam.sightangle < 180.0)
        Gparam.sightangle -= 10.0;

      break;

    case BLUE:

      if(Gparam.sightangle < 180.0)
        Gparam.sightangle -= 10.0;

      break;

    case WHITE:

      break;
  }
}

/* ------------------------------------------------------------ SightRangeUp
 * SightRangeUp 視野の長さを増やす
 */


/* ------------------------------------------------------------ ChangeDebugMode 
 * ChanegeDebugMode
 */
void ChangeDebugMode()
{
  ui.debug = !ui.debug;
}

/* ------------------------------------------------------------ ShowParameter 
 * ShowParameter パラメーターを表示する
 */
void ShowParameter()
{
  printf(" RED :Speed:%f,Angle:%f,Range:%f\n", Rparam.speed_max, Rparam.sightangle, Rparam.sightrange);
  printf("GREEN:Speed:%f,Angle:%f,Range:%f\n", Gparam.speed_max, Gparam.sightangle, Gparam.sightrange);
  printf("BLUE :Speed:%f,Angle:%f,Range:%f\n", Bparam.speed_max, Bparam.sightangle, Bparam.sightrange);
}


/* ------------------------------------------------------------ SelectType
 * SelectType 選択されている種族の色に変更する関数
 */
void SelectType()
{
  switch(ui.select)
  {
    case RED:
         ColorChange(&ui.color, 1.0, 0.0, 0.0, 0.5);
         break;

    case GREEN:
         ColorChange(&ui.color, 0.0, 1.0, 0.0, 0.5);
         break;

    case BLUE:
         ColorChange(&ui.color, 0.0, 0.0, 1.0, 0.5);
         break;

    case WHITE:
         ColorChange(&ui.color, 1.0, 1.0, 1.0, 0.5);
         break;

    default:

         break;
  }
}

//end of file
