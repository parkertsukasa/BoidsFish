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
FishDataT fish[LENGTH];
FeedDataT feed[FEEDLENGTH];

extern int time;

bool set;
int nowfeed;

/*---------------------------------------------------------------- InitScene
 * InitScene:
 *--------*/
void InitScene( void )
{

//  CreateMyModels ();

	////// シーンデータの初期化
	simdata.clip_far = 250.0;
	simdata.clip_near = 0.01;
	simdata.air_color[0] = 0.2;
	simdata.air_color[1] = 0.3;
	simdata.air_color[2] = 1.0;
	simdata.air_color[3] = 0.8; // fog density factor
	simdata.sky_color[0] = 0.1;
	simdata.sky_color[1] = 0.3;
	simdata.sky_color[2] = 0.9;
	simdata.sky_color[3] = 0.5; // sky color factor
	//////

  cam.pos.x = 0.0;
  cam.pos.y = 0.0;
  cam.pos.z = 0.0;
  cam.rot.x = 0.0;
  cam.rot.y = 0.0;
  cam.rot.z = 0.0;
  cam.up = 0;  

  camobj.radius = 150;
  camobj.speed = 0.005;
  camobj.pos.x = 0.0;
  camobj.pos.y = 50.0;
  camobj.pos.z = -camobj.radius;
  camobj.center.x = 0.0;
  camobj.center.y = 0.0;
  camobj.center.z = 0.0;
  camobj.thete = 0.0;

	for (int i = 0; i < LENGTH; i++)
	{
		fish[i].pos.x = Random(AQUARIUM_MIN + 20, AQUARIUM_MAX - 20);
		fish[i].pos.y = Random(AQUARIUM_MIN + 20, AQUARIUM_MAX - 20);
		fish[i].pos.z = Random(AQUARIUM_MIN + 20, AQUARIUM_MAX - 20);

		fish[i].rot.x = 0.0;
		fish[i].rot.y = 0.0;
		fish[i].rot.z = 0.0;

		fish[i].move.x = Random(-2,2);
		fish[i].move.y = Random(-2,2);
		fish[i].move.z = Random(5,5);

    fish[i].forward.x = 0.0;
    fish[i].forward.y = 0.0;
    fish[i].forward.z = 0.0;

    fish[i].hungry = false;

    fish[i].feednum = 0;

    fish[i].out = false;
  }

  for (int j = 0; j < FEEDLENGTH; j++)
  {      
	  feed[j].pos.x = 0;
	  feed[j].pos.y = 40;
	  feed[j].pos.z = 0;
    feed[j].rot.x = 0.0;
    feed[j].rot.y = 0.0;
    feed[j].rot.z = 0.0;
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

  CameraRotate();

	for(int i = 0; i < LENGTH; i++)
	{
    if(set)
    {
      SetPosition(i);//整列
    }
    else
    {
      if(fish[i].out)
       ReturnAquarium(i); //水槽の外にいたら戻ろうとする
      else
		    Cruising (i);//通常の巡行
    }
	}


	for(int j = 0; j < FEEDLENGTH; j++)
  {
    FeedControl (j);
  }

	////////
    return;
}


/*--------------------------------------------------------------------------------------------
 * 数学的便利関数群
 */


/*-------------------------------------------------------------- RadtoDeg
 * ラジアンから度数へ変換する関数
 */
float RadtoDeg (float f)
{
    float deg = f * 180.0 / 3.1415;
    return deg;
}

/*-------------------------------------------------------------- GetVector2Length
 * GetVector2Length : ベクトルの長さを取得する(2軸)
 */
float GetVector2Length ( float x, float y )
{
  float length = sqrtf ( x * x + y * y );
  return length;
}

/*-------------------------------------------------------------- GetVector3Length
 * GetVector3Length : ベクトルの長さを取得する(3軸)
 */
float GetVector3Length ( float x, float y, float z )
{
  float length = sqrtf ( (x * x) + (y * y) + (z * z));
  return length;
}

/*-------------------------------------------------------------- GetInnerProduct
 * GetInnerProduct : ベクトルの内積を求める
 */
float GetInnerProduct (float x1, float y1, float x2, float y2)
{
    float product = x1 * x2 + y1 * y2;
    return product;
}

/*-------------------------------------------------------------- GetVectorAngle
 * GetVector2Angle : ベクトルのなす角度を取得する
 */
float GetVector2Angle (float x1, float y1, float x2, float y2)
{
    //----- それぞれのベクトルの長さを求める -----
    float Alength = GetVector2Length (x1, y1);
    float Blength = GetVector2Length (x2, y2);

    //----- 内積を求める -----
    float product = GetInnerProduct (x1, y1, x2, y2);

    //----- cosθを求める -----
    float cos_theta = product / (Alength * Blength);

    //----- θを求める -----
    float theta = acosf(cos_theta);

    //----- ラジアンを度数に変換 -----
    float angle = theta * 180.0 / 3.1415;

    return angle;
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


/*--------------------------------------------------------------------------------------------
 * 魚の挙動をコントロールする関数群
 */


/*-------------------------------------------------------------- Coheision
 * Cohesion : 結合　群れの中心に向かって進行する
 *--------*/
Vector3 Cohesion(int i)
{

	Vector3 ave;
	ave.x = 0;
	ave.y = 0;
	ave.z = 0;

  int flock = 0;
	//----- 周囲の固体の中心を求める -----
	for(int j = 0; j < LENGTH; j++)
	{
		if(i != j)
		{
      //--- 各個体との距離を計算し一定距離より遠い個体は除外する ---
      Vector3 dist_other;
      dist_other.x = fish[i].pos.x - fish[j].pos.x;
      dist_other.y = fish[i].pos.y - fish[j].pos.y;
      dist_other.z = fish[i].pos.z - fish[j].pos.z;
      float dist = GetVector3Length (dist_other.x, dist_other.y, dist_other.z);

      float range = (float)AQUARIUM_MAX * 0.8;//水槽の大きさの4割

      if(fish[i].hungry)//餌に近い場合は餌を優先
        range *= 0.5;

      if(dist < range && !fish[j].out)
      {
			  ave.x += fish[j].pos.x;
			  ave.y += fish[j].pos.y;
		  	ave.z += fish[j].pos.z;
        flock += 1;
      }
		}
	}
	ave.x /= flock;
	ave.y /= flock;
	ave.z /= flock;


  //----- 自分と餌との距離を求める -----
  Vector3 dist_to_feed;
  dist_to_feed.x = feed[fish[i].feednum].pos.x - fish[i].pos.x;
  dist_to_feed.y = feed[fish[i].feednum].pos.y - fish[i].pos.y;
  dist_to_feed.z = feed[fish[i].feednum].pos.z - fish[i].pos.z;
  float length_to_feed = GetVector3Length(dist_to_feed.x, dist_to_feed.y, dist_to_feed.z);


  //----- 群の中心と餌との距離を求める -----
  Vector3 dist_ave_to_feed;
  dist_ave_to_feed.x = feed[fish[i].feednum].pos.x - ave.x;
  dist_ave_to_feed.y = feed[fish[i].feednum].pos.y - ave.y;
  dist_ave_to_feed.z = feed[fish[i].feednum].pos.z - ave.z;
  float length_ave_to_feed = GetVector3Length(dist_ave_to_feed.x, dist_ave_to_feed.y, dist_ave_to_feed.z);

  if(length_ave_to_feed > length_to_feed)
    fish[i].hungry = true;
  else
    fish[i].hungry = false;

	//平均と自分の位置の差を移動量とする
	float speed_factor = 300;

	Vector3 move;
	move.x = (ave.x - fish[i].pos.x)/speed_factor;
	move.y = (ave.y - fish[i].pos.y)/speed_factor;
	move.z = (ave.z - fish[i].pos.z)/speed_factor;


	return move;

}

/*-------------------------------------------------------------- Separation
 * Separation : 分離 ほかの固体と距離が近過ぎたらはなれる
 *--------*/
Vector3 Separation(int i)
{
	float dist_min = 5;//ほかの固体に近づける最短の距離

	Vector3 move;
	move.x = 0;
	move.y = 0;
	move.z = 0;

	for(int j = 0; j < LENGTH; j++)
	{
		if(i != j)
		{
			//----- ほかの固体との距離を求める ------
			Vector3 diff;
			diff.x = fish[i].pos.x - fish[j].pos.x;
			diff.y = fish[i].pos.y - fish[j].pos.y;
			diff.z = fish[i].pos.z - fish[j].pos.z;
			float dist = sqrtf((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));

			if(dist < dist_min)//一定以下の距離なら
			{
				//----- 反対方向へ移動量を与える ------
				float speed_factor = 20;
				move.x -= (fish[j].pos.x - fish[i].pos.x) / speed_factor;
				move.y -= (fish[j].pos.y - fish[i].pos.y) / speed_factor;
				move.z -= (fish[j].pos.z - fish[i].pos.z) / speed_factor;
			}
		}
	}

	return move;
}

/*-------------------------------------------------------------- Alignment
 * Alignment : 整列 ほかの固体と同じ方向へ移動する
 *--------*/
Vector3 Alignment (int i)
{
	Vector3 ave;
	ave.x = 0;
	ave.y = 0;
	ave.z = 0;

  int flock = 0;//自分の周囲の個体の数
		//----- 自分以外の固体の移動量の平均を求める -----
	for(int j = 0; j < LENGTH; j++)
	{
		if(i != j)
		{
      //--- 各個体との距離を計算し一定距離より遠い個体は除外する ---
      Vector3 dist_other;
      dist_other.x = fish[i].pos.x - fish[j].pos.x;
      dist_other.y = fish[i].pos.y - fish[j].pos.y;
      dist_other.z = fish[i].pos.z - fish[j].pos.z;
      float dist = GetVector3Length (dist_other.x, dist_other.y, dist_other.z);

      float range = (float)AQUARIUM_MAX * 1.2;//水槽の大きさの6割

      if(fish[i].hungry)//餌に近い場合は餌を優先
        range *= 0.5;

      if(dist < range && !fish[j].out)
      {
			  ave.x += fish[j].move.x;
			  ave.y += fish[j].move.y;
			  ave.z += fish[j].move.z;
        flock += 1;
      }
		}
	}

	ave.x /= flock;
	ave.y /= flock;
	ave.z /= flock;

	//----- 自分の移動量との差を加える -----
	float speed_factor = 1.0;
	Vector3 move;
	move.x = (fish[i].move.x - ave.x)/speed_factor;
	move.y = (fish[i].move.y - ave.y)/speed_factor;
	move.z = (fish[i].move.z - ave.z)/speed_factor;

	return move;
}


/* ------------------------------------------------------------- SetPosition
 * SetPosition 一列に整列させる。
 */
void SetPosition (int i)
{
  Vector3 set;//整列の目標地点
  float width = abs(AQUARIUM_MIN) + abs(AQUARIUM_MAX);
  set.x = (width / LENGTH) * i + AQUARIUM_MIN;
  set.y = 0;
  set.z = 0;

	float speed_factor = 30;

	Vector3 move;
	move.x = (set.x - fish[i].pos.x)/speed_factor;
	move.y = (set.y - fish[i].pos.y)/speed_factor;
	move.z = (set.z - fish[i].pos.z)/speed_factor;

  //----- 移動量を位置に与える -----
	fish[i].pos.x += move.x;
	fish[i].pos.y += move.y;
	fish[i].pos.z += move.z;
  
  //----- 移動方向を向く ------
  //----- pitch -----
  fish[i].rot.x = RadtoDeg( atan2f(move.y, GetVector2Length(move.x, move.z) ) );
  
  //----- yaw -----
  fish[i].rot.y = RadtoDeg ( atan2f(-move.x, -move.z) );

  return;
}


/*-------------------------------------------------------------- EatFeed
 * EatFeed : 餌を食べる 餌オブジェクトに向かって移動する
 *--------*/
Vector3 EatFeed (int i)
{
  //----- 一番近い餌を探す -----
  float nearfeed = 100000;
  for (int j = 0; j < FEEDLENGTH; j++)
  {
    if(feed[j].alive)
    {
      Vector3 diff;
      diff.x = feed[j].pos.x - fish[i].pos.x;
      diff.y = feed[j].pos.y - fish[i].pos.y;
      diff.z = feed[j].pos.z - fish[i].pos.z;
      //餌との距離を求める
      float dist = GetVector3Length(diff.x, diff.y, diff.z);
      if (nearfeed > dist)
      {
        nearfeed = dist;
        fish[i].feednum = j;
      }
    }
  }

  if (nearfeed == 100000)
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
    move.x = 0.0;    
    move.y = 0.0;    
    move.z = 0.0;    
  }
  return move;
}


/*-------------------------------------------------------------- Cruising
 * Cruising : 巡航 3種類の移動量を合成する
 *--------*/
void Cruising (int i)
{
	//----- それぞれの速度の重み ------
  float factor_cohe;
	float factor_sepa;
	float factor_alig;
  float factor_eat_;

	//--- 餌に近い場合は餌に向かう ---
  if(fish[i].hungry)
  {
    factor_cohe = 0.2;
    factor_sepa = 0.5;
    factor_alig = 0.6;
    factor_eat_ = 3.0;
  }
  else
  {
    factor_cohe = 0.6;
    factor_sepa = 0.5;
    factor_alig = 0.95;
    factor_eat_ = 3.0;
  }

	//----- それぞれの速度を求める ------
	Vector3 move_cohe = Cohesion (i);
	Vector3 move_sepa = Separation (i);
	Vector3 move_alig = Alignment (i);
  Vector3 move_eat_ = EatFeed (i);

	fish[i].move.x = move_cohe.x * factor_cohe + move_sepa.x * factor_sepa + move_alig.x * factor_alig + factor_eat_ * move_eat_.x; 
	fish[i].move.y = move_cohe.y * factor_cohe + move_sepa.y * factor_sepa + move_alig.y * factor_alig + factor_eat_ * move_eat_.y; 
	fish[i].move.z = move_cohe.z * factor_cohe + move_sepa.z * factor_sepa + move_alig.z * factor_alig + factor_eat_ * move_eat_.z; 

  //----- 速度が早すぎた場合、正規化を行う -----
  float velocity_max = 3.0;
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


  //----- 正面ベクトルの取得 -----
  fish[i].forward.x = fish[i].mat[9];
  fish[i].forward.y = fish[i].mat[10];
  fish[i].forward.z = fish[i].mat[11];
  

  //----- 移動方向を向く ------
  //----- pitch -----
  fish[i].rot.x = RadtoDeg( atan2f (fish[i].move.y, GetVector2Length (fish[i].move.x, fish[i].move.z)));

  //----- yaw -----
  fish[i].rot.y = RadtoDeg ( atan2f (-fish[i].move.x, -fish[i].move.z));


  //----- 水槽の端まで行ったら反転 ------
  if (fish[i].pos.x > AQUARIUM_MAX || fish[i].pos.x < AQUARIUM_MIN ||
      fish[i].pos.y > AQUARIUM_MAX || fish[i].pos.y < AQUARIUM_MIN ||
      fish[i].pos.z > AQUARIUM_MAX || fish[i].pos.z < AQUARIUM_MIN )
  {
        fish[i].out = true;
  }
}


/*-------------------------------------------------------------- ReturnAquarium
 * ReturnAquarium : 水槽へ戻る 水槽の外へ出てしまった時に水槽の中心へ向けて移動
 *--------*/
void ReturnAquarium (int i)
{
  Vector3 center;
  center.x = 0;
  center.y = 0;
  center.z = 0;

	float speed_factor = 1;

	Vector3 move;
	move.x = (center.x - fish[i].pos.x)/speed_factor;
	move.y = (center.y - fish[i].pos.y)/speed_factor;
	move.z = (center.z - fish[i].pos.z)/speed_factor;

  //----- 速度が早すぎた場合、正規化を行う -----
  float velocity_max = 10.0;
  float velocity = sqrtf((move.x * move.x) + (move.y * move.y) + (move.z * move.z));
  if (velocity > velocity_max)
  {
    move.x = (move.x / velocity) * velocity_max;    
    move.y = (move.y / velocity) * velocity_max;    
    move.z = (move.z / velocity) * velocity_max;    
  }
  //----- 移動量を位置に与える -----
	fish[i].pos.x += move.x;
	fish[i].pos.y += move.y;
	fish[i].pos.z += move.z;
  
  //----- 移動方向を向く ------
  //----- pitch -----
  fish[i].rot.x = RadtoDeg( atan2f (move.y, GetVector2Length (move.x, move.z)));

  //----- yaw -----
  fish[i].rot.y = RadtoDeg ( atan2f (-move.x, -move.z));

  //----- 水槽に戻ったら終了 ------
  if (fish[i].pos.x < AQUARIUM_MAX && fish[i].pos.x > AQUARIUM_MIN &&
      fish[i].pos.y < AQUARIUM_MAX && fish[i].pos.y > AQUARIUM_MIN &&
      fish[i].pos.z < AQUARIUM_MAX && fish[i].pos.z > AQUARIUM_MIN )
  {
      fish[i].out = false;
  }
}

//end of file
