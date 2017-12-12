#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"

#include "util.h"

#include "sim.h"

SimDataT simdata;
CamDataT cam;
FishDataT fish[LENGTH];

extern int time;



/*---------------------------------------------------------------- InitScene
 * InitScene:
 *--------*/
void InitScene( void )
{
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


	for (int i = 0; i < LENGTH; i++)
	{
		fish[i].pos.x = Random(AQUARIUM_MIN, AQUARIUM_MAX);
		fish[i].pos.y = Random(AQUARIUM_MIN, AQUARIUM_MAX);
		fish[i].pos.z = Random(AQUARIUM_MIN, AQUARIUM_MAX);

		fish[i].rot.x = Random (0.0, 360.0);
		fish[i].rot.y = Random (0.0, 360.0);
		fish[i].rot.z = Random (0.0, 360.0);

		fish[i].move.x = Random(-2,2);
		fish[i].move.y = Random(-2,2);
		fish[i].move.z = Random(2,5);

    fish[i].forward.x = 0.0;
    fish[i].forward.y = 0.0;
    fish[i].forward.z = 0.0;
	}

    return;
}
/*-------------------------------------------------------------- UpdateScene
 * UpdateScene:
 *--------*/
void UpdateScene( void )
{
	//////// データ更新 ////////

	for(int i = 0; i < LENGTH; i++)
	{
		Cruising (i);
	}

	////////
    return;
}

/*-------------------------------------------------------------- GetVectorLength
 * GetVectorLength : ベクトルの長さを取得する
 */
float GetVector2Length ( float x, float y )
{
  float length = sqrtf ( x * x + y * y );
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
			ave.y += fish[j].pos.z;
			ave.y += fish[j].pos.z;
		}
	}
	ave.x /= LENGTH-1;
	ave.y /= LENGTH-1;
	ave.z /= LENGTH-1;

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


/*-------------------------------------------------------------- Cruising
 * Cruising : 巡航 3種類の移動量を合成する
 *--------*/
void Cruising (int i)
{
	//----- それぞれの速度の重み ------
	float factor_cohe = 0.6;
	float factor_sepa = 0.7;
	float factor_alig = 0.95;

	//----- それぞれの速度を求める ------
	Vector3 move_cohe = Cohesion (i);
	Vector3 move_sepa = Separation (i);
	Vector3 move_alig = Alignment (i);

	fish[i].move.x = move_cohe.x * factor_cohe + move_sepa.x * factor_sepa + move_alig.x * factor_alig; 
	fish[i].move.y = move_cohe.y * factor_cohe + move_sepa.y * factor_sepa + move_alig.y * factor_alig; 
	fish[i].move.z = move_cohe.z * factor_cohe + move_sepa.z * factor_sepa + move_alig.z * factor_alig; 

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
  //----- roll ------
  fish[i].rot.z = GetVector2Angle(fish[i].move.x, fish[i].move.y, fish[i].forward.x, fish[i].forward.y);

  //----- pitch -----
  fish[i].rot.x = GetVector2Angle(fish[i].move.y, fish[i].move.z, fish[i].forward.y, fish[i].forward.z);

  //----- yaw -----
  fish[i].rot.y = GetVector2Angle(fish[i].move.x, fish[i].move.z, fish[i].forward.x, fish[i].forward.z);



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
