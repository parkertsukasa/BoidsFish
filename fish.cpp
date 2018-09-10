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

extern Selected ui;

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
	Evaluate ();
	HUDUpdate ();
	
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
	fish[i].pos.y = Random(-(HEIGHT/2) + 15, (HEIGHT/2) - 15);
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
	fish[i].forward = VectorScalar(&fish[i].forward, 0.1);
	
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
	Rparam.sightangle = 170.0;
	Rparam.sightrange = AQUARIUM_MAX * 0.8;
	Rparam.kc = 1.0;
	Rparam.ks = 1.0;
	Rparam.ka = 1.0;
	Rparam.kch = 0.5;
	Rparam.kes = 1.0;
	Rparam.cohesion = 0.0;
	Rparam.separation = 0.0;
	Rparam.alignment = 0.0;
	
	
	Gparam.length = G_LENGTH;
	Gparam.speed_max = 0.075;
	Gparam.sightangle = 170.0;
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
	Bparam.sightangle = 170.0;
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
 * 同じ種族同士の判定に使う
 * 返り値は0~1のfloat値
 */
float isVisible(int i, int j, FishDataT fish[])
{

  float visible = 0.0;
  float depth = 0.0;
  float range = 0.0;
  
  const float depth_max = fish[0].param->sightrange;
  const float half_depth = depth_max * 0.5; 

  const float range_max = fish[0].param->sightangle;
  const float half_angle = range_max * 0.5;

  //----- depth -----
	//--- 距離を求める ---
	Vector3 diff = VectorDiff( &fish[i].pos, &fish[j].pos );
  float length = GetVector3Length( &diff );
  //--- 視界外は除外 ---
  if( length > depth_max )
  {
    return 0.0;
  }
  else
  {
    depth = 1.0;
  }

  //----- range -----
  //--- なす角を求める ---
  float angle = GetVector3Angle( &fish[i].forward, &diff );//angleは度数
  if( isnan( angle ) )
  {
    printf( "%s\n", "angleでNANを検出" );
    printf( "%d\n", i );
  }
  angle = fabs( angle );

  //--- 視野外は除外 ---
  if( angle > range_max )
  {
    return 0.0;
  }
  else
  {
    //--- 視野の半分より内側の時は1.0 ---
    if( angle < half_angle )
      range = 1.0;
    else
    {
      float temp_angle = angle - half_angle;
      if( isnan( temp_angle ) )
      {
        printf( "%s\n", "temp_angleでNANを検出" );
        printf( "%d\n", i );
      }

      if( temp_angle < 0.01 )
        temp_angle = 0.01;

      if( temp_angle != 0.0 )
        range = temp_angle / half_angle;
      
      if( isnan( range ) )
      {
        printf( "%s\n", "rangeでNANを検出" );
        printf( "%d\n", i );
        //exit( 0 );
        range = 0.0;
      }
    }

    /*
    if( range < 0.0 || 1.0 < range )
    {
      printf("%s\n", "range is out of range");
    }
    */

  }

  visible = depth * range;
  
  return visible;
}

/* ------------------------------------------------------------- TargetVisible
 * TargetVisible 対象としているものが視界内にいるのかどうかを判定する関数
 * 別種族との判定に使う．
 * 返り値は0~1のfloat値
 */
float TargetVisible(int i, int j, FishDataT fish[], FishDataT target[])
{
	
	float visible = 0.0;
	float depth = 0.0;
	float range = 0.0;
	
	const float depth_max = fish[0].param->sightrange;
	const float half_depth = depth_max * 0.5;
	
	const float range_max = fish[0].param->sightangle;
	const float half_angle = range_max * 0.5;
	
	//----- depth -----
	//--- 距離を求める ---
	Vector3 diff = VectorDiff( &fish[i].pos, &target[j].pos );
	float length = GetVector3Length( &diff );
	//--- 視界外は除外 ---
	if( length > depth_max )
	{
		return 0.0;
	}
	else
	{
    depth = 1.0;
	}
	
	//----- range -----
	//--- なす角を求める ---
	float angle = GetVector3Angle( &fish[i].forward, &diff );//angleは度数
	angle = fabs( angle );
	
	//--- 視野外は除外 ---
	if( angle > range_max )
	{
		return 0.0;
	}
	else
	{
		//--- 視野の半分より内側の時は1.0 ---
		if( angle < half_angle )
			range = 1.0;
		else
		{
			float temp_angle = angle - half_angle;
			
			if( temp_angle < 0.01 )
				temp_angle = 0.01;
			
			if( temp_angle != 0.0 )
				range = temp_angle / half_angle;
			
		}
		
	}
	
	visible = depth * range;
	
	return visible;
}


/*-------------------------------------------------------------- Cohesion
 * Gather : 結合　群れの中心に向かって進行する
 *--------*/
Vector3 Gather(int i, FishDataT fish[])
{
	Vector3 move = VectorZero();

  int flock = 0;
	
	for(int j = 0; j < fish[i].param->length; j++)
	{
		if(i != j)
		{
			float visible = isVisible(i, j, fish);
      Vector3 diff = VectorZero();
      diff = VectorDiff(&fish[j].pos, &fish[i].pos);
			
      move.x += ( diff.x * visible );
      move.y += ( diff.y * visible );
      move.z += ( diff.z * visible );

      if( visible > 0 )
        flock += 1;
		}
	}

  if( flock > 0 )
  {
    move.x /= (float)flock;
    move.y /= (float)flock;
    move.z /= (float)flock;
  }
  else
  {
    move = VectorZero();
    return move;
  }
	
  float k = 0.001;
  
  move = VectorScalar(&move, k);

	return move;
}

/*-------------------------------------------------------------- Separation
 * Separate : 分離 ほかの固体と距離が近過ぎたらはなれる
 *--------*/
Vector3 Separate(int i, FishDataT fish[])
{
	Vector3 move = VectorZero();
  int flock = 0.0;
	
	for(int j = 0; j < fish[i].param->length; j++)
	{
		if(i != j)
		{
			Vector3 diff = VectorDiff(&fish[i].pos, &fish[j].pos);
			float length = GetVector3Length(&diff);
			
			float sightrange = fish[i].param->sightrange;
			//if(length < sightrange && length > 0.0)
			
			float k = 0.1;//係数k
		  float visible = isVisible(i, j, fish);
			
			move.x += ((diff.x / length) * (((k / (length * length)) * 0.5) + (((k / length) * 0.5))) * visible );
			move.y += ((diff.y / length) * (((k / (length * length)) * 0.5) + (((k / length) * 0.5))) * visible );
		  move.z += ((diff.z / length) * (((k / (length * length)) * 0.5) + (((k / length) * 0.5))) * visible );

      if( visible > 0.0 )
        flock += 1;
		}
	}

  if( flock > 0 )
  {
    move.x /= (float)flock;
    move.y /= (float)flock;
    move.z /= (float)flock;
  }
  else
  {
    return VectorZero();
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
	Vector3 diff = VectorDiff(&center, &fish[i].pos);
	float length = GetVector3Length(&diff);
	
	//----- 壁からの距離を求める -----
	float fromwall = AQUARIUM_MAX - length;
	if(fromwall < 0.1)
		fromwall = 0.1;
	
	float k = 0.4;// * fish[i].param->speed_max;//係数K
	
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
	float l = -0.05;//係数
	if(rooflength < (HEIGHT/2))
		move.y = l * (1 - (rooflength / (HEIGHT/2)));
	
	
	//----- 底面を避ける動き -----
	Vector3 floordiff = VectorDiff(&wallfish[1], &fish[i].pos);
	float floorlength = GetVector3Length(&floordiff);
	if(floorlength < (HEIGHT/2))
		move.y = l * (1 - (floorlength / (HEIGHT/2))) * -1;
	
	
	//大きくなりすぎないように調整
	float max_move = 0.2;
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
  int flock = 0;
	
	//----- 自分以外の固体の移動量の平均を求める -----
	for(int j = 0; j < fish[i].param->length; j++)
	{
		if(i != j)
		{
			float visible = isVisible(i, j, fish);

			ave.x += ( fish[j].forward.x * visible );
			ave.y += ( fish[j].forward.y * visible );
			ave.z += ( fish[j].forward.z * visible );

      if( visible > 0.0 )
        flock += 1;
		}
	}

  if ( flock > 0 )
  {
    ave.x /= (float)flock;
    ave.y /= (float)flock;
    ave.z /= (float)flock;
  }
  else
  {
    return VectorZero();
  }
	
	
	//視野角内のエージェントの進行方向の平均に合わせる
	float k = 0.2;
	Vector3 move;
	move.x = ave.x * k;
	move.y = ave.y * k;
	move.z = ave.z * k;
	
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
		
		float k = -0.03;
		
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
	
	float k1 = -5.0;//係数:避ける力の大きさ
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
		
		float visible = TargetVisible(i, j, fish, target);
		
		if(visible > 0.0)
		{
			move.x += ( diff.x * visible );
			move.y += ( diff.y * visible );
			move.z += ( diff.z * visible );
			visiblenumber += 1;
		}
	}
	
	if(visiblenumber > 0)
	{
		float k = 0.001;
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
		Vector3 diff = VectorDiff(&target[j].pos, &fish[i].pos);
		
		move.x += -diff.x;
		move.y += -diff.y;
		move.z += -diff.z;
	}
	
	move.x = (move.x / (float)target[0].param->length);
	move.y = (move.y / (float)target[0].param->length);
	move.z = (move.z / (float)target[0].param->length);
	
	float length = GetVector3Length(&move);
	
	float k = 0.35;
	
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
	float factor_avoi = 0.0;//0.05;
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
	
	fish[i].move.x = move_cohe.x * factor_cohe + move_sepa.x * factor_sepa + move_alig.x * factor_alig + move_encl.x * factor_encl + move_eat_.x * factor_eat_ + move_avoi.x * factor_avoi + move_chas.x * factor_chas + move_esca.x * factor_esca;
	fish[i].move.y = move_cohe.y * factor_cohe + move_sepa.y * factor_sepa + move_alig.y * factor_alig + move_encl.y * factor_encl + move_eat_.y * factor_eat_ + move_avoi.y * factor_avoi + move_chas.y * factor_chas + move_esca.y * factor_esca;
	fish[i].move.z = move_cohe.z * factor_cohe + move_sepa.z * factor_sepa + move_alig.z * factor_alig + move_encl.z * factor_encl + move_eat_.z * factor_eat_ + move_avoi.z * factor_avoi + move_chas.z * factor_chas + move_esca.z * factor_esca;
	
	//----- moveベクトルが小さい = 周囲に魚がおらず行動を決めかねている状態のときは内発的な移動行動に切り替える -----
	float move_length = GetVector3Length(&fish[i].move);
	float min_length = 0.01;
	if(move_length < min_length)
	{
		fish[i].move = VectorScalar(&fish[i].forward, 0.5);
		//return;
	}
	
	
	
	//----- 基礎情報の計算 -----
	//XZ平面におけるmoveベクトルの長さを求める(|move|)
	float move_xz = GetVector2Length(fish[i].move.x, fish[i].move.z);
	//XZ平面におけるforwardベクトルの長さ（速度）を求める(v1)
	float forward_xz = GetVector2Length(fish[i].forward.x, fish[i].forward.z);
	
	//垂直面におけるforwardベクトルの長さ
	float forward_yz = GetVector2Length(fish[i].forward.y, forward_xz);
	//垂直面におけるmoveベクトルの長さ
	float move_yz = GetVector2Length(fish[i].move.y, move_xz);
	
	
	
	
	//--- ▽▽▽  Yaw Control ▽▽▽ ---
	//forwardベクトルのyawを求める radian
	float yawf = atan2f(-fish[i].forward.x, -fish[i].forward.z);
	//moveベクトルのyawを求める(θm) radian
	float yawm = atan2f(-fish[i].move.x, -fish[i].move.z);
	
	//moveベクトルのローカルにおいてのyawを求める(θ) radian
	float local_yawm = yawm - yawf;
	
	//角度の差分に係数をかける(0~1の値を取る)
	float k_yaw = 0.2;
	float diff_yaw = k_yaw * local_yawm;
	
	//速度ベクトルのyawに足し合わせる
	float newyaw = yawf + diff_yaw;
	
	
	/*
	 //--- 工事中 ---
	 //pitch回転力を求め,係数をかける
	 float k_yaw = 100.0 * fish[i].param->speed_max;
	 float yaw_rotate = move_xz * sinf(local_yawm) * k_yaw;
	 
	 //変位ベクトルの先端から速度ベクトルに垂線をおろした時の速度ベクトルと垂線の交点から速度ベクトルの根元までの長さを求める
	 float move_localbase_xz = move_xz * cosf(local_yawm);
	 
	 //速度ベクトルに加えるべき角度を求める
	 float diff_yaw = atan2f(yaw_rotate, move_localbase_xz);
	 
	 //速度ベクトルのyawに足し合わせる
	 float newyaw = yawf + diff_pitch;
	 */
	
	//--- △△△ Yaw Control △△△ ---
	//---▽▽▽  Pitch Control ▽▽▽ ---
	//forwardベクトルのpitchを求める radian
	float pitchf = atan2f(-fish[i].forward.y, forward_xz);
	
	//moveベクトルのpitchを求める radian
	float pitchm = atan2f(-fish[i].move.y, move_xz);
	
	//moveベクトルのローカルにおけるpitchを求める radian
	float local_pitchm = pitchm - pitchf;
	
	//角度の差分に係数をかける(0~1の値を取る)
	float k_pitch = 0.05;
	float diff_pitch = k_pitch * local_pitchm;
	
	//速度ベクトルのpitchに足し合わせる
	float newpitch = pitchf + diff_pitch;
	
	/*
	 //--- 工事中 ---
	 //pitch回転力を求め,係数をかける
	 float k_pitch = 0.1;
	 float pitch_rotate = move_yz * sinf(local_pitchm) * k_pitch;
	 
	 //変位ベクトルの先端から速度ベクトルに垂線をおろした時の速度ベクトルと垂線の交点から速度ベクトルの根元までの長さを求める
	 float move_localbase_vertical = move_yz * cosf(local_pitchm);
	 
	 //速度ベクトルに加えるべき角度を求める
	 float diff_pitch = atan2f(pitch_rotate, move_localbase_vertical);
	 
	 //速度ベクトルのpitchに足し合わせる
	 float newpitch = pitchf + diff_pitch;
	 */
	
	//--- △△△ Pitch Control △△△ ---
	//--- ▽▽▽  Thrust Control ▽▽▽ ---
	
	float move_localbase_xz = move_xz * cosf(local_yawm);
	//推進力(move.z)を求めて，係数をかける
	float k_thrust = 2.0;
	float thrust = move_localbase_xz * k_thrust;
	
	forward_xz +=thrust;
	
	//速度に制限をかける
	float speed_max = fish[i].param->speed_max;
	if(forward_xz > speed_max )
		forward_xz = speed_max;
	
	/*
	 //速度ベクトルの正規化
	 Vector3 normalized_forward;
	 float length_forward = GetVector3Length(&fish[i].forward);
	 normalized_forward.x = fish[i].forward.x / length_forward;
	 normalized_forward.y = fish[i].forward.y / length_forward;
	 normalized_forward.z = fish[i].forward.z / length_forward;
	 
	 //速度ベクトルに推進力を適用する
	 fish[i].forward.x = normalized_forward.x * (length_forward * thrust);
	 fish[i].forward.y = normalized_forward.y * (length_forward * thrust);
	 fish[i].forward.z = normalized_forward.z * (length_forward * thrust);
	 
	 
	 //--- 推進力を求める ---(move.z)
	 float thrust = move_xz * -cosf(local_yawm);
	 
	 
	 float thrust_max = 0.01;
	 if(thrust < thrust_max && thrust > -thrust_max)
	 {
	 if(thrust > 0.0)
	 thrust = thrust_max;
	 else
	 thrust = -thrust_max;
	 }
	 
	 float k_thrust = 0.1;//係数
	 //推進力を加える
	 if(thrust != 0.0 && forward_xz != 0.0)
	 {
	 forward_xz *= (1 + (k_thrust * thrust)) / forward_xz;
	 
	 float speed_max = fish[i].param->speed_max;
	 if(forward_xz > speed_max )
	 forward_xz = speed_max;
	 }
	 */
	
	//yawを元にベクトルを再構築(forward i+1)
	fish[i].forward.x = -sinf(newyaw) * forward_xz;
	fish[i].forward.z = -cosf(newyaw) * forward_xz;
	
	//新たなpitchを元にベクトルを再構築
	fish[i].forward.y = -sinf(newpitch) * forward_yz;
	//fish[i].forward.z = -cosf(newpitch) * forward_yz;
	
	//Yawの変化に応じてRollの値を変更する。
	//float k_roll = 500.0;
	//fish[i].rot.z = yaw_rotate * k_roll;
	
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
	
	//--- 水の抵抗によって速度を減衰させる ---
	fish[i].forward = VectorScalar(&fish[i].forward, 0.99);
	
	
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
	fish[i].rot.y += Gaussian(-15.0, 15.0);
	
	
}
//end of file

