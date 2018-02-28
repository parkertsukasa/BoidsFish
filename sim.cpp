#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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


extern int time;

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

//  CreateMyModels ();
  glutSetCursor(GLUT_CURSOR_NONE);//マウスの非表示

  //----- 密度調査まわり -----
  samplepos = VectorZero();
  samplelength = AQUARIUM_MAX / 5;//水槽の幅の10%
  density = 0;

  //------ インターフェース関係 ------
  interface.select = interface.RED;
  ColorChange(&interface.color, 1.0, 0.0, 0.0, 1.0);


	////// シーンデータの初期化
	simdata.clip_far = 1000.0;
	simdata.clip_near = 0.01;
	simdata.air_color[0] = 0.0;
	simdata.air_color[1] = 0.0;
	simdata.air_color[2] = 0.0;
	simdata.air_color[3] = 0.8; // fog density factor
	simdata.sky_color[0] = 0.0;
	simdata.sky_color[1] = 0.0;
	simdata.sky_color[2] = 0.0;
	simdata.sky_color[3] = 0.5; // sky color factor
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

	for (int i = 0; i < LENGTH; i++)
	{
    FishInit(i, Rfish);
    ColorChange(&Rfish[i].color, 1.0, 0.0, 0.0, 1.0);
      
    FishInit(i, Gfish);
    ColorChange(&Gfish[i].color, 0.0, 1.0, 0.0, 1.0);

    FishInit(i, Bfish);
    ColorChange(&Bfish[i].color, 0.0, 0.0, 1.0, 1.0);
  }


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

/* ------------------------------------------------------------ FishColorChange
 * ColorChange 色を変更する関数
 */
void ColorChange(Color *color, float r , float g, float b, float a)
{
  color->r = r;
  color->g = g;
  color->b = b;
  color->a = a;
}

/* ------------------------------------------------------------- FishInit
 * FishInit 各種類の魚の値を初期化する関数
 */
void FishInit(int i, FishDataT fish[])
{
	fish[i].pos.x = Random(AQUARIUM_MIN + 30, AQUARIUM_MAX - 30);
	fish[i].pos.y = Random(AQUARIUM_MIN + 30, AQUARIUM_MAX - 30);
	fish[i].pos.z = Random(AQUARIUM_MIN + 30, AQUARIUM_MAX - 30);
	fish[i].rot.x = Random(0.0, 360.0);
	fish[i].rot.y = Random(0.0, 360.0);
	fish[i].rot.z = 0.0;

	fish[i].move.x = Random(-2,2);
	fish[i].move.y = Random(-2,2);
	fish[i].move.z = Random(5,5);

  fish[i].forward.x = -sinf(DegtoRad(fish[i].rot.y)); 
  fish[i].forward.y = sinf(DegtoRad(fish[i].rot.x)); 
  fish[i].forward.z = -cosf(DegtoRad(fish[i].rot.y)); 

  fish[i].speed = parameter.speed;
  
  fish[i].range = (float)AQUARIUM_MAX * (parameter.sightrange * 2.0);
  fish[i].sightangle = parameter.sightangle;//実際はこの値の二倍が視野角の広さになる
  fish[i].hungry = false;
  fish[i].feednum = 0;

  fish[i].color.r = 1.0;
  fish[i].color.g = 1.0;
  fish[i].color.b = 1.0;
  fish[i].color.a = 1.0;
}


/*-------------------------------------------------------------- UpdateScene
 * UpdateScene:
 *--------*/
void UpdateScene( void )
{
	//////// データ更新 ////////
  //printf("%f,%f,%f\n", Rfish[0].pos.x, Rfish[0].pos.y, Rfish[0].pos.z);
  
  CameraRotate();
  MouseObj ();

  density = DensitySerch();
  //printf("%d\n", density);
  
  SelectType();

	for(int i = 0; i < LENGTH; i++)
	{
		Cruising (i, Rfish);//通常の巡行
		Cruising (i, Gfish);//通常の巡行
		Cruising (i, Bfish);//通常の巡行
	}


	for(int j = 0; j < FEEDLENGTH; j++)
  {
    FeedControl (j);
  }

	////////
    return;
}


/*--------------------------------------------------------------------------------------------
 * 魚の挙動をコントロールする関数群
 */

/* ------------------------------------------------------------- isVisible
 * isVisible 対象としている魚が視界内にいるのかどうかを判定する関数
 */
bool isVisible(int i, int j, FishDataT fish[])
{
  //--- 各個体との距離を計算し一定距離より遠い個体は除外する ---
  Vector3 diff = VectorDiff(&fish[i].pos, &fish[j].pos);
  float length = GetVector3Length (&diff);

  float angle = GetVector3Angle(&fish[i].forward, &diff);

  bool visible;
  if(fabs(angle) < fish[i].sightangle && length < fish[i].range)
    visible = true;
  else
    visible = false;

  return visible;
}

/*-------------------------------------------------------------- Cohesion
 * Gather : 結合　群れの中心に向かって進行する
 *--------*/
Vector3 Gather(int i, FishDataT fish[])
{

	Vector3 ave = VectorZero();

  int flock = 0;
	//----- 周囲の固体の中心を求める -----
	for(int j = 0; j < LENGTH; j++)
	{
		if(i != j)
		{

      bool visible = isVisible(i, j, fish);

      if(visible)
      {
        ave = VectorAdd(&ave, &fish[j].pos);
        flock += 1;
      }
		}
	}

  //ave = VectorDivi(&ave, (float)flock);
  if(flock > 0)
  {
    ave.x /= (float)flock; 
    ave.y /= (float)flock; 
    ave.z /= (float)flock; 
  }
  
	//平均と自分の位置の差を移動量とする
	float speed_factor = 10.0;

  Vector3 diff_ave = VectorDiff(&ave, &fish[i].pos);

  float length_ave = GetVector3Length( &diff_ave );

	Vector3 move;
	move.x = (diff_ave.x / length_ave) * speed_factor/ (length_ave * length_ave);
	move.y = (diff_ave.y / length_ave) * speed_factor/ (length_ave * length_ave);
	move.z = (diff_ave.z / length_ave) * speed_factor/ (length_ave * length_ave);

	return move;

}

/*-------------------------------------------------------------- Separation
 * Separate : 分離 ほかの固体と距離が近過ぎたらはなれる
 *--------*/
Vector3 Separate(int i, FishDataT fish[])
{
	Vector3 move = VectorZero();

  int flock = 0;

	for(int j = 0; j < LENGTH; j++)
	{
		if(i != j)
		{
      Vector3 diff = VectorDiff(&fish[j].pos, &fish[i].pos);
      float length = GetVector3Length(&diff);
        
      if(length < fish[i].range && length > 0.0)
      {
        move.x += (diff.x / length) * 10.0 / (length * length) * -1; 
        move.y += (diff.y / length) * 10.0 / (length * length) * -1; 
        move.z += (diff.z / length) * 10.0 / (length * length) * -1; 
        flock += 1;
      }
		}
	}

  //move = VectorDivi(&move, (float)flock);
  if(flock > 0)
  {
    move.x /= (float)flock;
    move.y /= (float)flock;
    move.z /= (float)flock;
  }

	return move;
}


/*-------------------------------------------------------------- Enclose
 * Enclose : 壁から逃げる
 *--------*/
Vector3 Enclose(int i, FishDataT fish[])
{
  Vector3 center = VectorZero();//水槽の中心

  //----- 中心からの距離を求める -----
  Vector3 diff = VectorDiff(&fish[i].pos, &center);
  float length = GetVector3Length(&diff);
  
  //----- 壁からの距離を求める -----
  float fromwall = AQUARIUM_MAX - length;

  float k = 50.0 * fish[i].speed;//定数K
  if(fish[i].speed > 1.0)
    k *= fish[i].speed;

  Vector3 move = VectorZero();
  if(length > (AQUARIUM_MAX * 0.6))
  {
    move.x = (diff.x / length) * k / ((length * 0.5) + (length * length * 0.5)) * -1;
    move.y = (diff.y / length) * k / ((length * 0.5) + (length * length * 0.5)) * -1;
    move.z = (diff.z / length) * k / ((length * 0.5) + (length * length * 0.5)) * -1;

    if(length > AQUARIUM_MAX)
      move = VectorScalar(&move, 10.0);
  }

  return move;
}

/*-------------------------------------------------------------- Alignment
 * Align : 整列 ほかの固体と同じ方向へ移動する
 *--------*/
Vector3 Align (int i, FishDataT fish[])
{
	Vector3 ave = VectorZero();

  int flock = 0;//自分の周囲の個体の数
		//----- 自分以外の固体の移動量の平均を求める -----
	for(int j = 0; j < LENGTH; j++)
	{
		if(i != j)
		{

      bool visible = isVisible(i, j, fish);

      if(visible)
      {
        ave.x += fish[j].move.x;
			  ave.y += fish[j].move.y;
			  ave.z += fish[j].move.z;
        flock += 1;
      }
		}
	}

  //ave = VectorDivi(&ave, (float)flock);
  if(flock > 0)
  {
    ave.x /= (float)flock;
    ave.y /= (float)flock;
    ave.z /= (float)flock;
  }

	//----- 自分の移動量との差を加える -----
	float speed_factor = 1.0;
	Vector3 move;
	move.x = (ave.x - fish[i].move.x)/speed_factor;
	move.y = (ave.y - fish[i].move.y)/speed_factor;
	move.z = (ave.z - fish[i].move.z)/speed_factor;

	return move;
}


/*-------------------------------------------------------------- EatFeed
 * EatFeed : 餌を食べる 餌オブジェクトに向かって移動する
 *--------*/
Vector3 EatFeed (int i, FishDataT fish[])
{
  //----- 一番近い餌を探す -----
  float nearfeed = -1;
  for (int j = 0; j < FEEDLENGTH; j++)
  {
    if(feed[j].alive)
    {
      Vector3 diff = VectorDiff(&feed[j].pos, &fish[i].pos);
      //餌との距離を求める
      float dist = GetVector3Length(&diff);
      if (nearfeed > dist)
      {
        nearfeed = dist;
        fish[i].feednum = j;
      }
    }
  }

  if (nearfeed == -1)
    fish[i].feednum = -1;



	//----- 餌の方向へ移動する -----
	float speed_factor = 300;

	Vector3 move;
  if (feed[fish[i].feednum].alive && fish[i].feednum >= 0)
  {
	  move.x = (feed[fish[i].feednum].pos.x - fish[i].pos.x)/speed_factor;
	  move.y = (feed[fish[i].feednum].pos.y - fish[i].pos.y)/speed_factor;
	  move.z = (feed[fish[i].feednum].pos.z - fish[i].pos.z)/speed_factor;
  }
  else
  {
    move = VectorZero();    
  }
  return move;
}


/*-------------------------------------------------------------- Avoid
 * Avoid : 回避 特定のオブジェクトから逃げるように振る舞う
 *--------*/
Vector3 Avoid (int i, FishDataT fish[])
{
  Vector3 move = VectorZero();

  Vector3 diff;
  diff.x = mouse.x - fish[i].pos.x;
  diff.y = 0.0 - fish[i].pos.y;
  diff.z = mouse.y - fish[i].pos.z;

  float length = GetVector3Length(&diff);

  if(length < fish[i].range && length > 0.0)
  {
    move.x = (diff.x / length) * 100.0 / (length * length) * -1;
    move.y = (diff.y / length) * 100.0 / (length * length) * -1;
    move.z = (diff.z / length) * 100.0 / (length * length) * -1;
  }

  return move;

}

/*-------------------------------------------------------------- Cruising
 * Cruising : 巡航 3種類の移動量を合成する
 *--------*/
void Cruising (int i, FishDataT fish[])
{
	//----- それぞれの速度の重み ------
  float factor_cohe = parameter.kc;
	float factor_sepa = parameter.ks;
	float factor_alig = parameter.ka;
  float factor_eat_ = 0.1;
  float factor_avoi = 0.1;
  float factor_encl = 1.0;

//  printf("%f,%f,%f\n", factor_cohe, factor_sepa, factor_alig);

	//----- それぞれの速度を求める ------
 	Vector3 move_cohe = Gather(i, fish);
	Vector3 move_sepa = Separate(i, fish);
	Vector3 move_alig = Align(i, fish);
	Vector3 move_encl = Enclose(i, fish);
  Vector3 move_eat_ = EatFeed (i, fish);
  Vector3 move_avoi = Avoid (i, fish);

	fish[i].move.x = move_cohe.x * factor_cohe + move_sepa.x * factor_sepa + move_alig.x * factor_alig + move_encl.x * factor_encl + move_eat_.x * factor_eat_ + move_avoi.x * factor_avoi; 
	fish[i].move.y = move_cohe.y * factor_cohe + move_sepa.y * factor_sepa + move_alig.y * factor_alig + move_encl.y * factor_encl + move_eat_.y * factor_eat_ + move_avoi.y * factor_avoi; 
	fish[i].move.z = move_cohe.z * factor_cohe + move_sepa.z * factor_sepa + move_alig.z * factor_alig + move_encl.z * factor_encl + move_eat_.z * factor_eat_ + move_avoi.z * factor_avoi; 

  //----- 角度から正面方向を求める ------
  fish[i].forward.y = sinf(DegtoRad(fish[i].rot.x)); 
  //XZ平面への射影の長さlを求める
  float l = cosf(DegtoRad(fish[i].rot.x));
  fish[i].forward.x = -sinf(DegtoRad(fish[i].rot.y)) * l; 
  fish[i].forward.z = -cosf(DegtoRad(fish[i].rot.y)) * l; 

  //----- 正面方向への移動量を加える -----
  fish[i].move.x = fish[i].move.x + (fish[i].forward.x * fish[i].speed);
  fish[i].move.y = fish[i].move.y + (fish[i].forward.y * fish[i].speed);
  fish[i].move.z = fish[i].move.z + (fish[i].forward.z * fish[i].speed);

  //----- 速度が早すぎた場合、正規化を行う -----
  float velocity_max = 10.0;
  float velocity = sqrtf((fish[i].move.x * fish[i].move.x) + (fish[i].move.y * fish[i].move.y) + (fish[i].move.z * fish[i].move.z));
  if (velocity > velocity_max)
  {
    fish[i].move.x = (fish[i].move.x / velocity) * velocity_max;    
    fish[i].move.y = (fish[i].move.y / velocity) * velocity_max;    
    fish[i].move.z = (fish[i].move.z / velocity) * velocity_max;    
  }

  //----- 移動量を位置に与える -----
  fish[i].pos.x += fish[i].move.x;
  fish[i].pos.y += fish[i].move.y;
  fish[i].pos.z += fish[i].move.z;

  //----- 移動方向を向く ------
  //----- pitch -----
  fish[i].rot.x = RadtoDeg( atan2f (fish[i].move.y, GetVector2Length (fish[i].move.x, fish[i].move.z)));
  //----- yaw -----
  fish[i].rot.y = RadtoDeg ( atan2f (-fish[i].move.x, -fish[i].move.z));

  //----- 左右へのブレを加える -----
  fish[i].rot.y += Gaussian(-5.0, 5.0);

}



/*--------------------------------------------------------------------------------------------
 * カメラを管理する関数群
 */


/*-------------------------------------------------------------- CameraRotate
 * 水槽の周囲にカメラを回転させる
 */
void CameraRotate ()
{
    camobj.thete += camobj.speed;
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
    mouse.x *= AQUARIUM_MAX;
    mouse.y *= AQUARIUM_MAX;
}

/*------------------------------------------------------------- GiveFeed
 * GiveFeed : 餌を増やす　kbdmouse.cppから呼び出す
 */
void GiveFeed (float x, float y)
{
    feed[nowfeed].pos.x = x * 50;
    feed[nowfeed].pos.y = 50;
    feed[nowfeed].pos.z = y * 50;
    
    feed[nowfeed].alive = true;
    
    if (nowfeed == FEEDLENGTH -1)
    {
        nowfeed = 0;
    }
    else
    {
        nowfeed += 1;
    }
}

/*-------------------------------------------------------------- FeedControl
 * FeedControl : 餌の管理　自由落下や餌の残量
 *--------*/
void FeedControl (int j)
{
    if(feed[j].pos.y >= -30)
    {
        feed[j].pos.y -= 0.1;
    }
    else
    {
        feed[j].alive = false;
    }
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
