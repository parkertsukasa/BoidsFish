#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>

#include "common.h"

#include "util.h"

#include "sim.h"

//#include "mymodel.h"

SimDataT simdata;
CamDataT cam;
CamObj camobj;

MouseDataT mouse;
FishDataT Rfish[LENGTH];
FishDataT Gfish[LENGTH];
FishDataT Bfish[LENGTH];
FeedDataT feed[FEEDLENGTH];

Selected interface;

Parameter parameter;

//----- 密度を調べるためのパラメータ -----
Vector3 samplepos;//密度調査を行う場所の中心地
float samplelength;//調査範囲
int density;//対象範囲内にいる魚の数 


//extern int time;

bool set;
int nowfeed;

void loadFishParam( const char *file )
{
    FILE *fp;
    char num[16], num2[16], num3[16];
  
    fp = fopen( file, "r" );
    fscanf( fp, "%s", num );
    parameter.speed = strtod( num , NULL); //数値の文字列から数値に変換
    fscanf( fp, "%s", num );
    parameter.sightangle = strtod( num , NULL);
    fscanf( fp, "%s", num );
    parameter.sightrange = strtod( num , NULL);
    fscanf( fp, "%s", num );
    parameter.kc = strtod( num , NULL);
    fscanf( fp, "%s", num );
    parameter.ks = strtod( num , NULL);
    fscanf( fp, "%s", num );
    parameter.ka = strtod( num , NULL);
}

/*---------------------------------------------------------------- InitScene
 * InitScene:
 *--------*/
void InitScene( void )
{
  loadFishParam("parameter.txt");

    FishInit();
//  CreateMyModels ();
  glutSetCursor(GLUT_CURSOR_NONE);//マウスの非表示

  //----- 密度調査まわり -----
  samplepos = VectorZero();
  samplelength = AQUARIUM_MAX / 5;//水槽の幅の10%
  density = 0;

  //------ インターフェース関係 ------
  interface.select = RED;
  interface.debug = false;
  ColorChange(&interface.color, 1.0, 0.0, 0.0, 1.0);


	////// シーンデータの初期化
	simdata.clip_far = 1000.0;
	simdata.clip_near = 0.01;
	simdata.air_color[0] = 0.0;
	simdata.air_color[1] = 0.0;
	simdata.air_color[2] = 0.0;
	simdata.air_color[3] = 0.0; // fog density factor
	simdata.sky_color[0] = 0.0;
	simdata.sky_color[1] = 0.0;
	simdata.sky_color[2] = 0.0;
	simdata.sky_color[3] = 1.0; // sky color factor
	//////

  cam.pos = VectorZero();
  cam.rot = VectorZero();
  cam.up = 0;  

  camobj.radius = 150;
  camobj.speed = 0.005;
  camobj.pos.x = 0.0;
  camobj.pos.y = 70.0;
  camobj.pos.z = -camobj.radius;
  camobj.center = VectorZero();
  camobj.thete = 0.0;


  for (int j = 0; j < FEEDLENGTH; j++)
  {      
    feed[j].pos.x = 0;
    feed[j].pos.y = 40;
    feed[j].pos.z = 0;
    feed[j].rot = VectorZero();
    feed[j].amount = 50;
    feed[j].alive = false;
  }

  set = false;

  nowfeed = 0;

    return;
}

/*-------------------------------------------------------------- UpdateScene
 * UpdateScene:
 *--------*/
void UpdateScene( void )
{
    //////// データ更新 ////////
    //printf("%f,%f,%f\n", mouse.x, mouse.y, mouse.z);
    CameraRotate();
    MouseObj ();
    
    density = DensitySerch();
    //printf("%d\n", density);
    
    SelectType();
    
    FishUpdate();
    
    for(int j = 0; j < FEEDLENGTH; j++)
    {
        FeedControl (j);
    }
    
    ////////
    return;
}



/*--------------------------------------------------------------------------------------------
 * カメラを管理する関数群
 */


/*-------------------------------------------------------------- CameraRotate
 * 水槽の周囲にカメラを回転させる
 */
void CameraRotate ()
{
    //camobj.thete += camobj.speed;
    camobj.pos.x = camobj.radius * cosf(camobj.thete);
    camobj.pos.z = camobj.radius * sinf(camobj.thete);
}




/*--------------------------------------------------------------------------------------------
 * 餌を管理する関数群
 */

/*------------------------------------------------------------- MouseObj
 * MouseObj : マウスの位置のシミュレーション
 */
void MouseObj ()
{

}

/*------------------------------------------------------------- GiveFeed
 * GiveFeed : 餌を増やす　kbdmouse.cppから呼び出す
 */
void GiveFeed (float x, float y, float z)
{
  feed[nowfeed].pos.x = x;
  feed[nowfeed].pos.y = y;
  feed[nowfeed].pos.z = z;
    
  feed[nowfeed].alive = true;
  feed[nowfeed].amount = 50.0;
    
  if (nowfeed == FEEDLENGTH -1)
    nowfeed = 0;
  else
    nowfeed += 1;
}

/*-------------------------------------------------------------- FeedControl
 * FeedControl : 餌の管理　自由落下や餌の残量
 *--------*/
void FeedControl (int j)
{
  if(feed[j].amount <= 0.0)
    feed[j].alive = false;
}


/* ------------------------------------------------------------- DemsitySerch
 * DensitySerch 特定領域内に存在する魚の数をカウントする関数
 */
int DensitySerch ()
{
  int count = 0;

  for(int i = 0; i < LENGTH; i++)
  {
    Vector3 diff = VectorDiff(&samplepos, &Rfish[i].pos);
    float length = GetVector3Length(&diff);
    
    if(length < samplelength)
      count += 1;
  }

  for(int i = 0; i < LENGTH; i++)
  {
    Vector3 diff = VectorDiff(&samplepos, &Gfish[i].pos);
    float length = GetVector3Length(&diff);
    
    if(length < samplelength)
      count += 1;
  }

  for(int i = 0; i < LENGTH; i++)
  {
    Vector3 diff = VectorDiff(&samplepos, &Bfish[i].pos);
    float length = GetVector3Length(&diff);
    
    if(length < samplelength)
      count += 1;
  }

  return count;

}


//end of file
