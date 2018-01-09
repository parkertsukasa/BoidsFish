#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"

#include "util.h"

#include "sim.h"

//#include "mymodel.h"

SimDataT simdata;
CamDataT cam;
FishDataT fish[LENGTH];
FeedDataT feed;

extern int time;

bool set;


/*---------------------------------------------------------------- InitScene
 * InitScene:
 *--------*/
void InitScene( void )
{

//  CreateMyModels ();

	////// シーンデータの初期化
	simdata.clip_far = 300.0;
	simdata.clip_near = 0.1;
	simdata.air_color[0] = 1.0;
	simdata.air_color[1] = 1.0;
	simdata.air_color[2] = 1.0;
	simdata.air_color[3] = 0.5; // fog density factor
	simdata.sky_color[0] = 0.2;
	simdata.sky_color[1] = 0.3;
	simdata.sky_color[2] = 0.8;
	simdata.sky_color[3] = 0.5; // sky color factor
	//////

  cam.pos.x = 0.0;
  cam.pos.y = 0.0;
  cam.pos.z = 0.0;
  cam.rot.x = 0.0;
  cam.rot.y = 0.0;
  cam.rot.z = 0.0;
  cam.up = false;  

   
	feed.pos.x = Random(-40, 40);
	feed.pos.y = 40;
	feed.pos.z = Random(-40, 40);
  feed.rot.x = 0.0;
  feed.rot.y = 0.0;
  feed.rot.z = 0.0;
  feed.amount = 50;
  feed.alive = true;


	for (int i = 0; i < LENGTH; i++)
	{
		fish[i].pos.x = Random(AQUARIUM_MIN, AQUARIUM_MAX);
		fish[i].pos.y = Random(AQUARIUM_MIN, AQUARIUM_MAX);
		fish[i].pos.z = Random(AQUARIUM_MIN, AQUARIUM_MAX);

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

  }

  set = false;

    return;
}
/*-------------------------------------------------------------- UpdateScene
 * UpdateScene:
 *--------*/
void UpdateScene( void )
{
	//////// データ更新 ////////

  FeedControl ();

	for(int i = 0; i < LENGTH; i++)
	{
    if(set)
      SetPosition(i);
    else
		  Cruising (i);
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
 * 餌を管理する関数群
 */


/*-------------------------------------------------------------- FeedControl
 * FeedControl : 餌の管理　自由落下や餌の残量
 *--------*/
void FeedControl ()
{
    if(feed.pos.y >= AQUARIUM_MIN)
    {
      feed.pos.y -= 0.1;
    }
    else
    {
      feed.pos.x = Random(-40,40);
      feed.pos.y = 35;
      feed.pos.z = Random(-40,40);
      //feed.alive = false;
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

	//----- 自分以外の固体の中心を求める -----
	for(int j = 0; j < LENGTH; j++)
	{
		if(i != j)
		{
			ave.x += fish[j].pos.x;
			ave.y += fish[j].pos.y;
			ave.z += fish[j].pos.z;
		}
	}
	ave.x /= LENGTH-1;
	ave.y /= LENGTH-1;
	ave.z /= LENGTH-1;


  //----- 自分と餌との距離を求める -----
  Vector3 dist_to_feed;
  dist_to_feed.x = feed.pos.x - fish[i].pos.x;
  dist_to_feed.y = feed.pos.y - fish[i].pos.y;
  dist_to_feed.z = feed.pos.z - fish[i].pos.z;
  float length_to_feed = GetVector3Length(dist_to_feed.x, dist_to_feed.y, dist_to_feed.z);


  //----- 群の中心と餌との距離を求める -----
  Vector3 dist_ave_to_feed;
  dist_ave_to_feed.x = feed.pos.x - ave.x;
  dist_ave_to_feed.y = feed.pos.y - ave.y;
  dist_ave_to_feed.z = feed.pos.z - ave.z;
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

		//----- 自分以外の固体の速度の平均を求める -----
	for(int j = 0; j < LENGTH; j++)
	{
		if(i != j)
		{
			ave.x += fish[j].move.x;
			ave.y += fish[j].move.y;
			ave.z += fish[j].move.z;
		}
	}

	ave.x /= LENGTH - 1;
	ave.y /= LENGTH - 1;
	ave.z /= LENGTH - 1;

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
	//----- 餌の方向へ移動する -----
	float speed_factor = 300;

	Vector3 move;
  if (feed.alive)
  {
	  move.x = (feed.pos.x - fish[i].pos.x)/speed_factor;
	  move.y = (feed.pos.y - fish[i].pos.y)/speed_factor;
	  move.z = (feed.pos.z - fish[i].pos.z)/speed_factor;
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
    factor_sepa = 0.7;
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
  if (fish[i].pos.x > AQUARIUM_MAX || fish[i].pos.x < AQUARIUM_MIN )
  {
        fish[i].move.x *= -1;
  }
  if (fish[i].pos.y > AQUARIUM_MAX || fish[i].pos.y < AQUARIUM_MIN )
  {
        fish[i].move.y *= -1;
  }
  if (fish[i].pos.z > AQUARIUM_MAX || fish[i].pos.z < AQUARIUM_MIN )
  {
        fish[i].move.z *= -1;
  }

}


//end of file
