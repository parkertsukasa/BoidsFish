#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "fish.h"
#include "hud.h"

FishDataT Rfish[R_LENGTH];
FishDataT Gfish[G_LENGTH];
FishDataT Bfish[B_LENGTH];
Parameter Rparam;
Parameter Gparam;
Parameter Bparam;
FeedDataT feed[FEEDLENGTH];
MouseDataT mouse;


void FishInit()
{
	ParameterSet();
  InitHUD ();
	
	for (int i = 0; i < R_LENGTH; i++)
	{
		kind spc = RED;
		FishStructInit(i, Rfish, spc);
		ColorChange(&Rfish[i].color, 1.0, 0.0, 0.4, 1.0);
	}
	
	for (int i = 0; i < G_LENGTH; i++)
	{
		kind spc = GREEN;
		FishStructInit(i, Gfish, spc);
		ColorChange(&Gfish[i].color, 0.0, 1.0, 0.4, 1.0);
	}
	
	for (int i = 0; i < B_LENGTH; i++)
	{
		kind spc = BLUE;
		FishStructInit(i, Bfish, spc);
		ColorChange(&Bfish[i].color, 0.0, 0.4, 1.0, 1.0);
		
	}
}


void FishUpdate()
{

	for(int i = 0; i < R_LENGTH; i++)
	{
		Cruising (i, Rfish);//通常の巡行
	}
	
	for(int i = 0; i < G_LENGTH; i++)
	{
		Cruising (i, Gfish);//通常の巡行
	}
	
	for(int i = 0; i < B_LENGTH; i++)
	{
		Cruising (i, Bfish);//通常の巡行
	}

  Evaluate ();
}




/* ------------------------------------------------------------ FishColorChang
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
void FishStructInit(int i, FishDataT fish[], kind spc)
{
	fish[i].species = spc;
	fish[i].pos.x = Random(AQUARIUM_MIN + 30, AQUARIUM_MAX - 30);
	fish[i].pos.y = Random(-(HEIGHT/2) + 20, (HEIGHT/2) - 20);
	fish[i].pos.z = Random(AQUARIUM_MIN + 30, AQUARIUM_MAX - 30);
	
	fish[i].rot.x = 0.0;
	fish[i].rot.y = Random(0.0, 360.0);
	fish[i].rot.z = 0.0;
	
	fish[i].move.x = Random(-2,2);
	fish[i].move.y = Random(-2,2);
	fish[i].move.z = Random(5,5);
	
	fish[i].forward.x = -sinf(DegtoRad(fish[i].rot.y));
	fish[i].forward.y = sinf(DegtoRad(fish[i].rot.x));
	fish[i].forward.z = -cosf(DegtoRad(fish[i].rot.y));
	fish[i].forward = VectorScalar(&fish[i].forward, 0.01);
	
	fish[i].feednum = 0;
	
	fish[i].color.r = 1.0;
	fish[i].color.g = 1.0;
	fish[i].color.b = 1.0;
	fish[i].color.a = 1.0;
	fish[i].wall = VectorZero();
	
	switch (spc)
	{
		case RED:
			fish[i].param = &Rparam;
			break;
		case GREEN:
			fish[i].param = &Gparam;
			break;
		case BLUE:
			fish[i].param = &Bparam;
			break;
		default:
			
			break;
	}
	
}

/*-------------------------------------------------------------- ParameterSet
 * 種族間で共通のパラメータを設定する
 */
void ParameterSet()
{
	Rparam.length = R_LENGTH;
	Rparam.speed_max = 0.15;
	Rparam.sightangle = 60.0;
	Rparam.sightrange = AQUARIUM_MAX * 0.8;
	Rparam.kc = 1.0;
	Rparam.ks = 1.0;
	Rparam.ka = 1.0;
	Rparam.kch = 1.0;
	Rparam.kes = 1.0;
  Rparam.cohesion = 0.0;
  Rparam.separation = 0.0;
  Rparam.alignment = 0.0;
	
	
	Gparam.length = G_LENGTH;
	Gparam.speed_max = 0.075;
	Gparam.sightangle = 90.0;
	Gparam.sightrange = AQUARIUM_MAX * 1.0;
	Gparam.kc = 1.0;
	Gparam.ks = 1.0;
	Gparam.ka = 1.0;
	Gparam.kch = 1.0;
	Gparam.kes = 1.0;
  Gparam.cohesion = 0.0;
  Gparam.separation = 0.0;
  Gparam.alignment = 0.0;
	
	Bparam.length = B_LENGTH;
	Bparam.speed_max = 0.05;
	Bparam.sightangle = 120.0;
	Bparam.sightrange = AQUARIUM_MAX * 1.2;
	Bparam.kc = 1.0;
	Bparam.ks = 1.0;
	Bparam.ka = 1.0;
	Bparam.kch = 1.0;
	Bparam.kes = 1.0;
  Bparam.cohesion = 0.0;
  Bparam.separation = 0.0;
  Bparam.alignment = 0.0;

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
	float square_length = GetVector3LengthSquare (&diff);
	float square_sightrange = fish[i].param->sightrange * fish[i].param->sightrange;
  if(square_length > square_sightrange)
    return false;
	
	float angle = GetVector3Angle(&fish[i].forward, &diff);
	
	float sightangle = fish[i].param->sightangle;
	
	bool visible;
	if(fabs(angle) < sightangle)
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
	Vector3 move = VectorZero();
	
	int flock = 0;
	//----- 周囲の固体の中心を求める -----
	for(int j = 0; j < fish[i].param->length; j++)
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
	
	if(flock > 0)
	{
		ave.x /= (float)flock;
		ave.y /= (float)flock;
		ave.z /= (float)flock;
	}
  else
    return move;
	
	//平均と自分の位置の差を移動量とする
	float k = 0.0002;
	
	Vector3 diff_ave = VectorDiff(&fish[i].pos, &ave);
	
	float length_ave = GetVector3Length( &diff_ave );
	
	move.x = diff_ave.x * k;
	move.y = diff_ave.y * k;
	move.z = diff_ave.z * k;
	
	return move;
}


/*-------------------------------------------------------------- Separation
 * Separate : 分離 ほかの固体と距離が近過ぎたらはなれる
 *--------*/
Vector3 Separate(int i, FishDataT fish[])
{
	Vector3 move = VectorZero();
	
	int flock = 0;
	
	for(int j = 0; j < fish[i].param->length; j++)
	{
		if(i != j)
		{
			Vector3 diff = VectorDiff(&fish[j].pos, &fish[i].pos);
			float length = GetVector3Length(&diff);
			
			float sightrange = fish[i].param->sightrange;
			//if(length < sightrange && length > 0.0)
			
			float k = 0.02;//係数k
			bool visible = isVisible(i, j, fish);
			
			if(visible)
			{
				move.x += (diff.x / length) * (((k / (length * length)) * 0.5) + (((k / length) * 0.5)));
				move.y += (diff.y / length) * (((k / (length * length)) * 0.5) + (((k / length) * 0.5)));
				move.z += (diff.z / length) * (((k / (length * length)) * 0.5) + (((k / length) * 0.5)));
				flock += 1;
			}
		}
	}
	
	if(flock > 0)
	{
		move.x /= (float)flock;
		move.y /= (float)flock;
		move.z /= (float)flock;
	}
	//大きすぎる場合は正規化して丸める
	float movelength = GetVector3Length(&move);
	float maxvalue = 1.0;
	if(movelength > maxvalue)
	{
		move.x = move.x / movelength * maxvalue;
		move.y = move.y / movelength * maxvalue;
		move.z = move.z / movelength * maxvalue;
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
	if(fromwall < 0.1)
		fromwall = 0.1;
	
	float k = 0.3 * fish[i].param->speed_max;//係数K
	
	Vector3 move = VectorZero();
	
	move.x = (diff.x / length) * k * (1 / (fromwall * fromwall));
	move.y = 0.0;
	move.z = (diff.z / length) * k * (1 / (fromwall * fromwall));
	
	Vector3 wallfish[2];
	
	//wallfish[0]:各個体の天井面への射影
	wallfish[0].x = fish[i].pos.x;
	wallfish[0].y = (HEIGHT /2);
	wallfish[0].z = fish[i].pos.z;
	
	//wallfish[1]:各個体の底面への射影
	wallfish[1].x = fish[i].pos.x;
	wallfish[1].y = -(HEIGHT / 2);
	wallfish[1].z = fish[i].pos.z;
	
	//----- 天井を避ける動き -----
	Vector3 roofdiff = VectorDiff(&wallfish[0], &fish[i].pos);
	float rooflength = GetVector3Length(&roofdiff);//天井との距離
	float l = 0.0005;//係数
	if(rooflength > (HEIGHT/2))
	  move.y = l * (1 - (rooflength / (HEIGHT/2)));
	//----- 底面を避ける動き -----
	Vector3 floordiff = VectorDiff(&wallfish[1], &fish[i].pos);
	float floorlength = GetVector3Length(&floordiff);
	if(floorlength > (HEIGHT/2))
		move.y = l * (1 - (floorlength / (HEIGHT/2))) * -1;
	
  //大きくなりすぎないように調整
  float max_move = 0.1;
  float length_move = GetVector3Length(&move);
  if(length_move > max_move)
  {
    move.x = (move.x / length_move) * max_move;
    move.y = (move.y / length_move) * max_move;
    move.z = (move.z / length_move) * max_move;
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
	for(int j = 0; j < fish[i].param->length; j++)
	{
		if(i != j)
		{
			bool visible = isVisible(i, j, fish);
			
			if(visible)
			{
				ave.x += fish[j].forward.x;
				ave.y += fish[j].forward.y;
				ave.z += fish[j].forward.z;
				flock += 1;
			}
		}
	}
	
	if(flock > 0)
	{
		ave.x /= (float)flock;
		ave.y /= (float)flock;
		ave.z /= (float)flock;
	}
	
	//視野角内のエージェントの進行方向の平均に合わせる
	float k = 0.3;
	Vector3 move;
	move.x = ave.x * k;
	move.y = ave.y * k;
	move.z = ave.z * k;
	
	
	/*
	 move.x = (ave.x - fish[i].forward.x)/speed_factor;
	 move.y = (ave.y - fish[i].forward.y)/speed_factor;
	 move.z = (ave.z - fish[i].forward.z)/speed_factor;
	 */
	
	return move;
	
}


/*-------------------------------------------------------------- EatFeed
 * EatFeed : 餌を食べる 餌オブジェクトに向かって移動する
 *--------*/
Vector3 EatFeed (int i, FishDataT fish[])
{
	Vector3 move = VectorZero();
	
	if(fish[i].species == RED || fish[i].species == GREEN)
		return move;
	
	//----- 一番近い餌を探す -----
	float nearfeed = 100000;
	for (int j = 0; j < FEEDLENGTH; j++)
	{
		if(feed[j].alive)
		{
			Vector3 diff = VectorDiff(&fish[i].pos, &feed[j].pos);
			float length = GetVector3Length (&diff);
			
			float angle = 0.0;//GetVector3Angle(&fish[i].forward, &diff);
			
			float sightangle = fish[i].param->sightangle;
			float sightrange = fish[i].param->sightrange * 0.5;
			
			bool visible;
			if(length < sightrange && angle < sightangle)
				visible = true;
			else
				visible = false;
			
			if(visible && nearfeed > length)
			{
				nearfeed = length;
				fish[i].feednum = j;
			}
		}
	}
	
	if (nearfeed == 100000)
		fish[i].feednum = -1;
	
	//----- 一定距離以内なら餌を食べて餌の総量(amount)を減らす -----
	if(nearfeed < 2.5)
		feed[fish[i].feednum].amount -= 0.1;
	
	//----- 餌の方向へ移動する -----
	if (feed[fish[i].feednum].alive && fish[i].feednum >= 0)
	{
		Vector3 diff_feed = VectorDiff(&fish[i].pos, &feed[fish[i].feednum].pos);
		float length_feed = GetVector3Length(&diff_feed);
		
		float k = 0.03;
		
		move.x = diff_feed.x * (1 / length_feed) * k;
		move.y = diff_feed.y * (1 / length_feed) * k;
		move.z = diff_feed.z * (1 / length_feed) * k;
		
	}
	
	else
	{
		move = VectorZero();
		
	}
	return move;
	
}

/*-------------------------------------------------------------- Avoid
 * Avoid : 回避 特定のオブジェクトから逃げるように振る舞う
 * -> ポイントの真上/真下も回避対象として計算する
 *--------*/
Vector3 AvoidCylinder (int i, FishDataT fish[])
{
	Vector3 move = VectorZero();
	
	Vector3 diff;
	diff.x = mouse.x - fish[i].pos.x;
	diff.y = 0.0;
	diff.z = mouse.z - fish[i].pos.z;
	
	float length = GetVector3Length(&diff);
	length -= POINTRADIUS;//ポイントの半径分減算
	
	float k1 = 5.0;//係数:避ける力の大きさ
	move.x = (diff.x / length) * k1 * (1 / length);
	move.y = (diff.y / length) * k1 * (1 / length);
	move.z = (diff.z / length) * k1 * (1 / length);
	
	
	float move_length = GetVector3Length( &move );
	float max_length = 1.0;
	if(move_length > max_length)
	{
		move.x = (move.x / move_length) * max_length;
		move.y = (move.y / move_length) * max_length;
		move.z = (move.z / move_length) * max_length;
	}
	
	return move;
}


/*-------------------------------------------------------------- Avoid
 * AvoidSphere : 回避 特定のオブジェクトから逃げるように振る舞う
 * -> ポイントの真上/真下に移動できる。
 *--------*/
Vector3 AvoidSphere (int i, FishDataT fish[])
{
	Vector3 move = VectorZero();
	
	Vector3 diff;
	diff.x = mouse.x - fish[i].pos.x;
	diff.y = mouse.y - fish[i].pos.y;
	diff.z = mouse.z - fish[i].pos.z;
	
	float length = GetVector3Length(&diff);
	length -= POINTRADIUS;//ポイントの半径分減算
	
	float k1 = 0.1;//係数:避ける力の大きさ
	move.x = (diff.x / length) * k1 * (1 / length) * -1;
	move.y = (diff.y / length) * k1 * (1 / length) * -1;
	move.z = (diff.z / length) * k1 * (1 / length) * -1;
	
	return move;
}


/* ------------------------------------------------------------ Avoid
 * Avoid : Avoid関数の種類を切り替える
 */
Vector3 Avoid( int i, FishDataT fish[] )
{
	Vector3 move = AvoidCylinder( i, fish );
	return move;
}


/*-------------------------------------------------------------- Chase
 * Chase : 追いかける
 *--------*/
Vector3 Chase (int i, FishDataT fish[])
{
	FishDataT *target;
	
	Vector3 move = VectorZero();
	
	switch(fish[i].species)
	{
		case RED:
			target = Gfish;
			break;
			
		case GREEN:
			target = Bfish;
			break;
			
		case BLUE:
			
			return move;
			
			break;
			
		default:
			break;
	}
	
	int visiblenumber = 0;

	for (int j = 0; j < target[0].param->length; j++)
	{
		Vector3 diff = VectorDiff(&target[j].pos, &fish[i].pos);
		float square_length = GetVector3LengthSquare(&diff);
    float square_sightrange = fish[j].param->sightrange * fish[j].param->sightrange;

    float angle = GetVector3Angle(&diff, &fish[i].forward);
    float sightangle = fish[i].param->sightangle;

    bool visible;
    if(square_sightrange > square_length && sightangle > angle)
      visible = true;
    else 
      visible = false;

    if(visible)
    {
      move.x += diff.x;
      move.y += diff.y;
      move.z += diff.z;
      visiblenumber += 1;
    }
  }
  
  if(visiblenumber > 0)
  {
    float k = 20.0;
    move.x = (move.x / (float)visiblenumber) * k; 
    move.y = (move.y / (float)visiblenumber) * k; 
    move.z = (move.z / (float)visiblenumber) * k; 
  }
	
	return move;
	
}

/*-------------------------------------------------------------- Escape
 * Escape : 逃げる
 *--------*/
Vector3 Escape (int i, FishDataT fish[])
{
	FishDataT *target;
	
	Vector3 move = VectorZero();
	
	switch(fish[i].species)
	{
		case RED:
			
			return move;
			
			break;
			
		case BLUE:
			target = Gfish;
			break;
			
		case GREEN:
			target = Rfish;
			break;
		default:
			break;
	}
	

	for (int j = 0; j < target[0].param->length; j++)
	{
		Vector3 diff = VectorDiff(&fish[i].pos, &target[j].pos);
		
    move.x += -diff.x;
    move.y += -diff.y;
    move.z += -diff.z;
	}

  move.x = (move.x / (float)target[0].param->length);
  move.y = (move.y / (float)target[0].param->length);
  move.z = (move.z / (float)target[0].param->length);

  float length = GetVector3Length(&move);

  float k = 0.1;

  move.x = (move.x / length) * k * ( 1 / length );
  move.y = (move.y / length) * k * ( 1 / length );
  move.z = (move.z / length) * k * ( 1 / length );
	
	return move;
	
}



/*-------------------------------------------------------------- MakeMoveVector
 * 上記の関数を組み合わせてmoveベクトルを作りforwardベクトルに反映する関数
 */
void MakeMoveVector(int i, FishDataT fish[])
{
	//----- それぞれの速度の重み ------
	float factor_cohe = fish[i].param->kc;
	float factor_sepa = fish[i].param->ks;
	float factor_alig = fish[i].param->ka;
	
	float factor_eat_ = 1.0;
	float factor_avoi = 0.01;
	float factor_encl = 1.0;
	float factor_chas = fish[i].param->kch;
	float factor_esca = fish[i].param->kes;
	
	//----- それぞれの速度を求める ------
	static Vector3 move_cohe;
	static Vector3 move_sepa;
	static Vector3 move_alig;
	static Vector3 move_encl;
	static Vector3 move_eat_;
	static Vector3 move_avoi;
	static Vector3 move_chas;
	static Vector3 move_esca;
	
	move_cohe = Gather(i, fish);
	move_sepa = Separate(i, fish);
	move_alig = Align(i, fish);
	move_encl = Enclose(i, fish);
	move_eat_ = EatFeed (i, fish);
	move_avoi = Avoid (i, fish);
	move_chas = Chase (i, fish);
	move_esca = Escape(i, fish);
	
	fish[i].move.x = move_cohe.x * factor_cohe + move_sepa.x * factor_sepa + move_alig.x * factor_alig + move_encl.x * factor_encl + move_eat_.x * factor_eat_ + move_avoi.x * factor_avoi + move_cohe.x * factor_chas + move_esca.x *factor_esca;
	fish[i].move.y = move_cohe.y * factor_cohe + move_sepa.y * factor_sepa + move_alig.y * factor_alig + move_encl.y * factor_encl + move_eat_.y * factor_eat_ + move_avoi.y * factor_avoi + move_cohe.y * factor_chas + move_esca.y *factor_esca;
	fish[i].move.z = move_cohe.z * factor_cohe + move_sepa.z * factor_sepa + move_alig.z * factor_alig + move_encl.z * factor_encl + move_eat_.z * factor_eat_ + move_avoi.z * factor_avoi + move_cohe.z * factor_chas + move_esca.z *factor_esca;
	
	
	//----- 基礎情報の計算 -----
	//XZ平面におけるmoveベクトルの長さを求める(|move|)
	float move_xz = GetVector2Length(fish[i].move.x, fish[i].move.z);
	//XZ平面におけるforwardベクトルの長さ（速度）を求める(v1)
	float forward_xz = GetVector2Length(fish[i].forward.x, fish[i].forward.z);
	
	//垂直面におけるforwardベクトルの長さ
	float forward_yz = GetVector2Length(fish[i].forward.y, forward_xz);
	
	
	//---▽▽▽  Pitch Control ▽▽▽ ---
	//forwardベクトルのpitchを求める radian
	float pitchf = atan2f(fish[i].forward.y, forward_xz);
	
	//moveベクトルのpitchを求める radian
	float pitchm = atan2f(fish[i].move.y, move_xz);
	
	//moveベクトルのローカルにおけるpitchを求める radian
	float pitch = pitchm - pitchf;
	
	//--- pitchを元に新しいpitchを求める ---
	float k_pitch = 0.3;//係数
  float diff_pitch = k_pitch * pitch;
  float max_diff_pitch = 0.1 * 3.14;

  if(diff_pitch > max_diff_pitch)
    diff_pitch = max_diff_pitch;

  if(diff_pitch < -max_diff_pitch)
    diff_pitch = -max_diff_pitch;

	float newpitch = pitchf + diff_pitch;
	
	
	//--- △△△ Pitch Control △△△ ---
	
	//------ XZ平面 ------
	//forwardベクトルのyawを求める radian
	float yawf = atan2f(-fish[i].forward.x, -fish[i].forward.z);
	//moveベクトルのyawを求める(θm) radian
	float yawm = atan2f(-fish[i].move.x, -fish[i].move.z);
	
	//moveベクトルのローカルにおいてのyawを求める(θ) radian
	float yaw = yawm - yawf;
	
	//--- 推進力を求める ---(move.z)
	float thrust = move_xz * -cosf(yaw);
	
	
	float thrust_max = 0.01;
	if(thrust < thrust_max && thrust > -thrust_max)
	{
		if(thrust > 0.0)
			thrust = thrust_max;
		else
			thrust = -thrust_max;
	}
	
	//--- 回転させる力を求める ---(move.x)
	float rotateyaw = move_xz * -sinf(yaw);
	//変異した後のyawを求める(θi+1) radian
	const float k_yaw = 1.5 * ( 1.0 / fish[i].param->speed_max); //moveベクトルから回転を制御する係数
	float newyaw = yawf + k_yaw * rotateyaw;
	
	
	float k_thrust = 0.1;//係数
	//推進力を加える
	if(thrust != 0.0 && forward_xz != 0.0)
	{
		forward_xz *= (1 + (k_thrust * thrust)) / forward_xz;
		
		float speed_max = fish[i].param->speed_max;
		if(forward_xz > speed_max )
			forward_xz = speed_max;
	}
	
	
	//新たなpitchを元にベクトルを再構築
	fish[i].forward.y = -sinf(newpitch) * forward_yz;
	fish[i].forward.z = -cosf(newpitch) * forward_yz;
	
	//yawを元にベクトルを再構築(forward i+1)
	fish[i].forward.x = -sinf(newyaw) * forward_xz;
	fish[i].forward.z = -cosf(newyaw) * forward_xz;
	
	//Yawの変化に応じてRollの値を変更する。
	float k_roll = 500.0;
	fish[i].rot.z = rotateyaw * k_roll;
	
}

/*-------------------------------------------------------------- Cruising
 * Cruising : 巡航 3種類の移動量を合成する
 * forwardベクトルは魚の速度を表すベクトル
 * moveベクトルは計算の結果導き出された方向変位のベクトル
 *--------*/
void Cruising (int i, FishDataT fish[])
{
	//--- moveベクトルの更新を一定の確率で行う ----
	float update_factor = Random(0.0, 2.0);
	if(update_factor > 1.0)
		MakeMoveVector(i, fish);
	
	Vector3 nextpos  = VectorAdd(&fish[i].pos, &fish[i].forward);//次のフレームでの位置
	
	
	//----- 水槽外にはみ出てしまいそうな場合水槽に沿う位置に移動を制限する -----
	float length = GetVector3Length(&nextpos);//水槽の中心からの距離
	
	//--- XZ平面においての制限 ---
	if(length > AQUARIUM_MAX)
	{
		nextpos.x = (nextpos.x * AQUARIUM_MAX / length);
		nextpos.z = (nextpos.z * AQUARIUM_MAX / length);
	}
	//--- 高さ領域における制限(上部) ---
	if(nextpos.y > (HEIGHT/2) )
	{
		nextpos.y = HEIGHT/2;
	}
	//--- 高さ領域における制限(底部) ---
	if(nextpos.y < -(HEIGHT/2) )
	{
		nextpos.y = -(HEIGHT/2);
		
	}
	
	
	//----- 移動量を位置に与える -----
	fish[i].pos = nextpos;
	
	//----- 移動方向を向く ------
	//----- pitch -----
	fish[i].rot.x = RadtoDeg( atan2f (fish[i].forward.y, GetVector2Length (fish[i].forward.x, fish[i].forward.z)));
	//----- yaw -----
	fish[i].rot.y = RadtoDeg ( atan2f (-fish[i].forward.x, -fish[i].forward.z));
	
	
	//----- 左右へのブレを加える -----
	fish[i].rot.y += Gaussian(-5.0, 5.0);
	
	
}
//end of file

