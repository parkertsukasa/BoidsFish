#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "sim.h"
extern FishDataT Rfish[R_LENGTH];
extern FishDataT Gfish[G_LENGTH];
extern FishDataT Bfish[B_LENGTH];
extern Parameter Rparam;
extern Parameter Gparam;
extern Parameter Bparam;
extern FeedDataT feed[FEEDLENGTH];
extern MouseDataT mouse;


void FishInit()
{
  ParameterSet();

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
	fish[i].pos.y = Random(-(HEIGHT/2) + 5, (HEIGHT/2) - 5);
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
  Rparam.kc = 0.1;
  Rparam.ks = 0.07;
  Rparam.ka = 0.02;
  
  Gparam.length = G_LENGTH;
  Gparam.speed_max = 0.1;
  Gparam.sightangle = 90.0;
  Gparam.sightrange = AQUARIUM_MAX * 1.0;
  Gparam.kc = 0.1;
  Gparam.ks = 0.07;
  Gparam.ka = 0.02;
  
  Bparam.length = B_LENGTH;
  Bparam.speed_max = 0.05;
  Bparam.sightangle = 180.0;
  Bparam.sightrange = AQUARIUM_MAX * 1.2;
  Bparam.kc = 0.1;
  Bparam.ks = 0.07;
  Bparam.ka = 0.01;
  
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
	
  float sightangle = fish[i].param->sightangle;
  float sightrange = fish[i].param->sightrange;

	bool visible;
	if(fabs(angle) < sightangle && length < sightrange)
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
	
	
	//平均と自分の位置の差を移動量とする
	float speed_factor = 1.0;
	
	Vector3 diff_ave = VectorDiff(&ave, &fish[i].pos);
	
	float length_ave = GetVector3Length( &diff_ave );
	
	Vector3 move;
	move.x = (diff_ave.x / length_ave) * speed_factor/ (length_ave);
	move.y = (diff_ave.y / length_ave) * speed_factor/ (length_ave);
	move.z = (diff_ave.z / length_ave) * speed_factor/ (length_ave);
	
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
			float k = 1.0;//係数k
			
      float sightrange = fish[i].param->sightrange;

			if(length < sightrange && length > 0.0)
			{
				move.x += (diff.x / length) * k / (length * length);
				move.y += (diff.y / length) * k / (length * length);
				move.z += (diff.z / length) * k / (length * length);
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
	float maxvalue = 3.0;
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
  if(fromwall < 0.01)
    fromwall = 0.01;
	
	float k = 3.0;//係数K
	
	Vector3 move = VectorZero();
	
  move.x = (diff.x / length) * k * (1 / fromwall);
  move.y = 0.0;
  move.z = (diff.z / length) * k * (1 / fromwall);

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
	float l = 0.3;//係数
	if(rooflength > (HEIGHT/2))
		move.y = l * (1 - (rooflength / (HEIGHT/2)));
	//----- 底面を避ける動き -----
	Vector3 floordiff = VectorDiff(&wallfish[1], &fish[i].pos);
	float floorlength = GetVector3Length(&floordiff);
	if(floorlength > (HEIGHT/2))
		move.y = l * (1 - (floorlength / (HEIGHT/2))) * -1;
	
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
	
	
	//----- 自分の移動量との差を加える -----
	float speed_factor = 10.0;
	Vector3 move;
	move.x = (ave.x - fish[i].forward.x)/speed_factor;
	move.y = (ave.y - fish[i].forward.y)/speed_factor;
	move.z = (ave.z - fish[i].forward.z)/speed_factor;
	
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
	
	if (nearfeed == 100000)
		fish[i].feednum = -1;
	
	//----- 一定距離以内なら餌を食べて餌の総量(amount)を減らす -----
  if(nearfeed < 1.0)
    feed[fish[i].feednum].amount -= 1;
	
	//----- 餌の方向へ移動する -----
	float speed_factor = 300;
	
	if (feed[fish[i].feednum].alive && fish[i].feednum >= 0)
	{
		move.x = (fish[i].pos.x - feed[fish[i].feednum].pos.x)/speed_factor;
		move.y = (fish[i].pos.y - feed[fish[i].feednum].pos.y)/speed_factor;
		move.z = (fish[i].pos.z - feed[fish[i].feednum].pos.z)/speed_factor;
		
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
	
	int neartarget = -1;
	float nearlength = 1000000;
	
	for (int j = 0; j < target[0].param->length; j++)
	{
		Vector3 diff = VectorDiff(&target[j].pos, &fish[i].pos);
		float length = GetVector3Length(&diff);
		bool visible = isVisible(i, j, fish);
		
		if(visible && length < nearlength)
		{
			neartarget = j;
			nearlength = length;
		}
	}
	
	if(neartarget > -1)
	{
		Vector3 difftarget = VectorDiff(&fish[i].pos, &target[neartarget].pos);
		float lengthtarget = GetVector3Length(&difftarget);
		float k = 1.0;
		move.x = (difftarget.x / lengthtarget) / (lengthtarget * lengthtarget) * k;
		move.y = (difftarget.y / lengthtarget) / (lengthtarget * lengthtarget) * k;
		move.z = (difftarget.z / lengthtarget) / (lengthtarget * lengthtarget) * k;
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
	
	int neartarget = -1;
	float nearlength = 1000000;
	
	for (int j = 0; j < target[0].param->length; j++)
	{
		Vector3 diff = VectorDiff(&target[j].pos, &fish[i].pos);
		float length = GetVector3Length(&diff);
		
		if(length < nearlength)
		{
			neartarget = j;
			nearlength = length;
		}
	}
	
	if(neartarget > -1)
	{
		Vector3 difftarget = VectorDiff(&fish[i].pos, &target[neartarget].pos);
		float lengthtarget = GetVector3Length(&difftarget);
		float k = 1.0;
		move.x = (difftarget.x / lengthtarget) / (lengthtarget * lengthtarget) * k * -1;
		move.y = (difftarget.y / lengthtarget) / (lengthtarget * lengthtarget) * k * -1;
		move.z = (difftarget.z / lengthtarget) / (lengthtarget * lengthtarget) * k * -1;
	}
	
	return move;
	
}


/*-------------------------------------------------------------- Cruising
 * Cruising : 巡航 3種類の移動量を合成する
 * forwardベクトルは魚の速度を表すベクトル
 * moveベクトルは計算の結果導き出された方向変位のベクトル
 *--------*/
void Cruising (int i, FishDataT fish[])
{
	//----- それぞれの速度の重み ------
	float factor_cohe = fish[i].param->kc;
	float factor_sepa = fish[i].param->ks;
	float factor_alig = fish[i].param->ka;
	
	float factor_eat_ = 0.5;
	float factor_avoi = 0.01;
	float factor_encl = 0.03;
	float factor_chas = 0.01;
	float factor_esca = 0.05;
	
	//  printf("%f,%f,%f\n", factor_cohe, factor_sepa, factor_alig);
	
	//----- それぞれの速度を求める ------
	Vector3 move_cohe = Gather(i, fish);
	Vector3 move_sepa = Separate(i, fish);
	Vector3 move_alig = Align(i, fish);
	Vector3 move_encl = Enclose(i, fish);
	Vector3 move_eat_ = EatFeed (i, fish);
	Vector3 move_avoi = Avoid (i, fish);
	Vector3 move_chas = Chase (i, fish);
	Vector3 move_esca = Escape(i, fish);
	
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
  float k_pitch = 0.4;//係数
  float newpitch = pitchf + (k_pitch * pitch);
  

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


  float thrust_max = 0.001;
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
	const float k_yaw = 5.0; //moveベクトルから回転を制御する係数
	float newyaw = yawf + k_yaw * rotateyaw;
	

	float k_thrust = 0.0001;//係数
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

  //Yawの変化に応じてRollの値を変更する。
  float k_roll = 500.0;
  fish[i].rot.z = rotateyaw * k_roll; 
	
	//----- 左右へのブレを加える -----
	fish[i].rot.y += Gaussian(-20.0, 20.0);
	
	
}
//end of file


