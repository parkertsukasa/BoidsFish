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
		Cruising (i, Rfish);//í èÌÇÃèÑçs
	}
	
	for(int i = 0; i < G_LENGTH; i++)
	{
		Cruising (i, Gfish);//í èÌÇÃèÑçs
	}
	
	for(int i = 0; i < B_LENGTH; i++)
	{
		Cruising (i, Bfish);//í èÌÇÃèÑçs
	}
	
	//ê¬ÇÃmoveÉxÉNÉgÉãÇÃí∑Ç≥
	//"float blue_move = GetVector3Length(&Bfish[0].move);
	//printf("%f\n", blue_move);
	
}



/* ------------------------------------------------------------ FishColorChang
 * ColorChange êFÇïœçXÇ∑ÇÈä÷êî
 */
void ColorChange(Color *color, float r , float g, float b, float a)
{
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;
}


/* ------------------------------------------------------------- FishInit
 * FishInit äeéÌóﬁÇÃãõÇÃílÇèâä˙âªÇ∑ÇÈä÷êî
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
 * éÌë∞ä‘Ç≈ã§í ÇÃÉpÉâÉÅÅ[É^Çê›íËÇ∑ÇÈ
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
	Rparam.kch = 1.0;
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
 * ãõÇÃãììÆÇÉRÉìÉgÉçÅ[ÉãÇ∑ÇÈä÷êîåQ
 */

/* ------------------------------------------------------------- isVisible
 * isVisible ëŒè€Ç∆ÇµÇƒÇ¢ÇÈãõÇ™éãäEì‡Ç…Ç¢ÇÈÇÃÇ©Ç«Ç§Ç©ÇîªíËÇ∑ÇÈä÷êî
 */
bool isVisible(int i, int j, FishDataT fish[])
{
	//--- äeå¬ëÃÇ∆ÇÃãóó£ÇåvéZÇµàÍíËãóó£ÇÊÇËâìÇ¢å¬ëÃÇÕèúäOÇ∑ÇÈ ---
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
 * Gather : åãçáÅ@åQÇÍÇÃíÜêSÇ…å¸Ç©Ç¡ÇƒêiçsÇ∑ÇÈ
 *--------*/
Vector3 Gather(int i, FishDataT fish[])
{
	Vector3 ave = VectorZero();
	Vector3 move = VectorZero();
	
	int flock = 0;
	//----- é¸àÕÇÃå≈ëÃÇÃíÜêSÇãÅÇﬂÇÈ -----
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
	
	//ïΩãœÇ∆é©ï™ÇÃà íuÇÃç∑Çà⁄ìÆó Ç∆Ç∑ÇÈ
	float k = 0.001;
	
	Vector3 diff_ave = VectorDiff(&ave, &fish[i].pos);
	
	float length_ave = GetVector3Length( &diff_ave );
	
	move.x = diff_ave.x * k;
	move.y = diff_ave.y * k;
	move.z = diff_ave.z * k;
	
	return move;
}


/*-------------------------------------------------------------- Separation
 * Separate : ï™ó£ ÇŸÇ©ÇÃå≈ëÃÇ∆ãóó£Ç™ãﬂâﬂÇ¨ÇΩÇÁÇÕÇ»ÇÍÇÈ
 *--------*/
Vector3 Separate(int i, FishDataT fish[])
{
	Vector3 move = VectorZero();
	
	int flock = 0;
	
	for(int j = 0; j < fish[i].param->length; j++)
	{
		if(i != j)
		{
			Vector3 diff = VectorDiff(&fish[i].pos, &fish[j].pos);
			float length = GetVector3Length(&diff);
			
			float sightrange = fish[i].param->sightrange;
			//if(length < sightrange && length > 0.0)
			
			float k = 0.1;//åWêîk
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
	//ëÂÇ´Ç∑Ç¨ÇÈèÍçáÇÕê≥ãKâªÇµÇƒä€ÇﬂÇÈ
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
 * Enclose : ï«Ç©ÇÁì¶Ç∞ÇÈ
 *--------*/
Vector3 Enclose(int i, FishDataT fish[])
{
	Vector3 center = VectorZero();//êÖëÖÇÃíÜêS
	
	//----- íÜêSÇ©ÇÁÇÃãóó£ÇãÅÇﬂÇÈ -----
	Vector3 diff = VectorDiff(&center, &fish[i].pos);
	float length = GetVector3Length(&diff);
	
	//----- ï«Ç©ÇÁÇÃãóó£ÇãÅÇﬂÇÈ -----
	float fromwall = AQUARIUM_MAX - length;
	if(fromwall < 0.1)
		fromwall = 0.1;
	
	float k = 0.4;// * fish[i].param->speed_max;//åWêîK
	
	Vector3 move = VectorZero();
	
	move.x = (diff.x / length) * k * (1 / (fromwall * fromwall));
	move.y = 0.0;
	move.z = (diff.z / length) * k * (1 / (fromwall * fromwall));
	
	Vector3 wallfish[2];
	
	//wallfish[0]:äeå¬ëÃÇÃìVà‰ñ Ç÷ÇÃéÀâe
	wallfish[0].x = fish[i].pos.x;
	wallfish[0].y = (HEIGHT /2);
	wallfish[0].z = fish[i].pos.z;
	
	//wallfish[1]:äeå¬ëÃÇÃíÍñ Ç÷ÇÃéÀâe
	wallfish[1].x = fish[i].pos.x;
	wallfish[1].y = -(HEIGHT / 2);
	wallfish[1].z = fish[i].pos.z;
	
	//----- ìVà‰ÇîÇØÇÈìÆÇ´ -----
	Vector3 roofdiff = VectorDiff(&wallfish[0], &fish[i].pos);
	float rooflength = GetVector3Length(&roofdiff);//ìVà‰Ç∆ÇÃãóó£
	float l = -0.02;//åWêî
	if(rooflength < (HEIGHT/2))
		move.y = l * (1 - (rooflength / (HEIGHT/2)));
	
	
	//----- íÍñ ÇîÇØÇÈìÆÇ´ -----
	Vector3 floordiff = VectorDiff(&wallfish[1], &fish[i].pos);
	float floorlength = GetVector3Length(&floordiff);
	if(floorlength < (HEIGHT/2))
		move.y = l * (1 - (floorlength / (HEIGHT/2))) * -1;
	
	
	//ëÂÇ´Ç≠Ç»ÇËÇ∑Ç¨Ç»Ç¢ÇÊÇ§Ç…í≤êÆ
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
 * Align : êÆóÒ ÇŸÇ©ÇÃå≈ëÃÇ∆ìØÇ∂ï˚å¸Ç÷à⁄ìÆÇ∑ÇÈ
 *--------*/
Vector3 Align (int i, FishDataT fish[])
{
	Vector3 ave = VectorZero();
	
	int flock = 0;//é©ï™ÇÃé¸àÕÇÃå¬ëÃÇÃêî
	//----- é©ï™à»äOÇÃå≈ëÃÇÃà⁄ìÆó ÇÃïΩãœÇãÅÇﬂÇÈ -----
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
	
	//éãñÏäpì‡ÇÃÉGÅ[ÉWÉFÉìÉgÇÃêiçsï˚å¸ÇÃïΩãœÇ…çáÇÌÇπÇÈ
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
 * EatFeed : âaÇêHÇ◊ÇÈ âaÉIÉuÉWÉFÉNÉgÇ…å¸Ç©Ç¡Çƒà⁄ìÆÇ∑ÇÈ
 *--------*/
Vector3 EatFeed (int i, FishDataT fish[])
{
	Vector3 move = VectorZero();
	
	if(fish[i].species == RED || fish[i].species == GREEN)
		return move;
	
	//----- àÍî‘ãﬂÇ¢âaÇíTÇ∑ -----
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
	
	//----- àÍíËãóó£à»ì‡Ç»ÇÁâaÇêHÇ◊ÇƒâaÇÃëçó (amount)Çå∏ÇÁÇ∑ -----
	if(nearfeed < 2.5)
		feed[fish[i].feednum].amount -= 0.1;
	
	//----- âaÇÃï˚å¸Ç÷à⁄ìÆÇ∑ÇÈ -----
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
 * Avoid : âÒî ì¡íËÇÃÉIÉuÉWÉFÉNÉgÇ©ÇÁì¶Ç∞ÇÈÇÊÇ§Ç…êUÇÈïëÇ§
 * -> É|ÉCÉìÉgÇÃê^è„/ê^â∫Ç‡âÒîëŒè€Ç∆ÇµÇƒåvéZÇ∑ÇÈ
 *--------*/
Vector3 AvoidCylinder (int i, FishDataT fish[])
{
	Vector3 move = VectorZero();
	
	Vector3 diff;
	diff.x = mouse.x - fish[i].pos.x;
	diff.y = 0.0;
	diff.z = mouse.z - fish[i].pos.z;
	
	float length = GetVector3Length(&diff);
	length -= POINTRADIUS;//É|ÉCÉìÉgÇÃîºåaï™å∏éZ
	
	float k1 = -5.0;//åWêî:îÇØÇÈóÕÇÃëÂÇ´Ç≥
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
 * AvoidSphere : âÒî ì¡íËÇÃÉIÉuÉWÉFÉNÉgÇ©ÇÁì¶Ç∞ÇÈÇÊÇ§Ç…êUÇÈïëÇ§
 * -> É|ÉCÉìÉgÇÃê^è„/ê^â∫Ç…à⁄ìÆÇ≈Ç´ÇÈÅB
 *--------*/
Vector3 AvoidSphere (int i, FishDataT fish[])
{
	Vector3 move = VectorZero();
	
	Vector3 diff;
	diff.x = mouse.x - fish[i].pos.x;
	diff.y = mouse.y - fish[i].pos.y;
	diff.z = mouse.z - fish[i].pos.z;
	
	float length = GetVector3Length(&diff);
	length -= POINTRADIUS;//É|ÉCÉìÉgÇÃîºåaï™å∏éZ
	
	float k1 = 0.1;//åWêî:îÇØÇÈóÕÇÃëÂÇ´Ç≥
	move.x = (diff.x / length) * k1 * (1 / length) * -1;
	move.y = (diff.y / length) * k1 * (1 / length) * -1;
	move.z = (diff.z / length) * k1 * (1 / length) * -1;
	
	return move;
}


/* ------------------------------------------------------------ Avoid
 * Avoid : Avoidä÷êîÇÃéÌóﬁÇêÿÇËë÷Ç¶ÇÈ
 */
Vector3 Avoid( int i, FishDataT fish[] )
{
	Vector3 move = AvoidCylinder( i, fish );
	return move;
}


/*-------------------------------------------------------------- Chase
 * Chase : í«Ç¢Ç©ÇØÇÈ
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
		float square_sightrange = 50.0;//fish[j].param->sightrange * fish[j].param->sightrange;
		
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
		float k = 0.01;
		move.x = (move.x / (float)visiblenumber) * k;
		move.y = (move.y / (float)visiblenumber) * k;
		move.z = (move.z / (float)visiblenumber) * k;
	}
	
	return move;
	
}

/*-------------------------------------------------------------- Escape
 * Escape : ì¶Ç∞ÇÈ
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
	
	float k = 0.3;
	
	move.x = (move.x / length) * k * ( 1 / length );
	move.y = (move.y / length) * k * ( 1 / length );
	move.z = (move.z / length) * k * ( 1 / length );
	
	return move;
	
}



/*-------------------------------------------------------------- MakeMoveVector
 * è„ãLÇÃä÷êîÇëgÇ›çáÇÌÇπÇƒmoveÉxÉNÉgÉãÇçÏÇËforwardÉxÉNÉgÉãÇ…îΩâfÇ∑ÇÈä÷êî
 */
void MakeMoveVector(int i, FishDataT fish[])
{
	//----- ÇªÇÍÇºÇÍÇÃë¨ìxÇÃèdÇ› ------
	float factor_cohe = fish[i].param->kc;
	float factor_sepa = fish[i].param->ks;
	float factor_alig = fish[i].param->ka;
	
	float factor_eat_ = 1.0;
	float factor_avoi = 0.05;
	float factor_encl = 1.0;
	float factor_chas = fish[i].param->kch;
	float factor_esca = fish[i].param->kes;
	
	//----- ÇªÇÍÇºÇÍÇÃë¨ìxÇãÅÇﬂÇÈ ------
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
	
	//----- moveÉxÉNÉgÉãÇ™è¨Ç≥Ç¢ = é¸àÕÇ…ãõÇ™Ç®ÇÁÇ∏çsìÆÇåàÇﬂÇ©ÇÀÇƒÇ¢ÇÈèÛë‘ÇÃÇ∆Ç´ÇÕì‡î≠ìIÇ»à⁄ìÆçsìÆÇ…êÿÇËë÷Ç¶ÇÈ -----
	float move_length = GetVector3Length(&fish[i].move);
	float min_length = 0.015;
	if(move_length < min_length)
	{
		fish[i].move = VectorScalar(&fish[i].forward, 0.5);
		//return;
	}
	
	
	
	//----- äÓëbèÓïÒÇÃåvéZ -----
	//XZïΩñ Ç…Ç®ÇØÇÈmoveÉxÉNÉgÉãÇÃí∑Ç≥ÇãÅÇﬂÇÈ(|move|)
	float move_xz = GetVector2Length(fish[i].move.x, fish[i].move.z);
	//XZïΩñ Ç…Ç®ÇØÇÈforwardÉxÉNÉgÉãÇÃí∑Ç≥Åië¨ìxÅjÇãÅÇﬂÇÈ(v1)
	float forward_xz = GetVector2Length(fish[i].forward.x, fish[i].forward.z);
	
	//êÇíºñ Ç…Ç®ÇØÇÈforwardÉxÉNÉgÉãÇÃí∑Ç≥
	float forward_yz = GetVector2Length(fish[i].forward.y, forward_xz);
	//êÇíºñ Ç…Ç®ÇØÇÈmoveÉxÉNÉgÉãÇÃí∑Ç≥
	float move_yz = GetVector2Length(fish[i].move.y, move_xz);
	
	
	
	
	//--- Å§Å§Å§  Yaw Control Å§Å§Å§ ---
	//forwardÉxÉNÉgÉãÇÃyawÇãÅÇﬂÇÈ radian
	float yawf = atan2f(-fish[i].forward.x, -fish[i].forward.z);
	//moveÉxÉNÉgÉãÇÃyawÇãÅÇﬂÇÈ(É∆m) radian
	float yawm = atan2f(-fish[i].move.x, -fish[i].move.z);
	
	//moveÉxÉNÉgÉãÇÃÉçÅ[ÉJÉãÇ…Ç®Ç¢ÇƒÇÃyawÇãÅÇﬂÇÈ(É∆) radian
	float local_yawm = yawm - yawf;
	
	//äpìxÇÃç∑ï™Ç…åWêîÇÇ©ÇØÇÈ(0~1ÇÃílÇéÊÇÈ)
	float k_yaw = 0.2;
	float diff_yaw = k_yaw * local_yawm;
	
	//ë¨ìxÉxÉNÉgÉãÇÃyawÇ…ë´ÇµçáÇÌÇπÇÈ
	float newyaw = yawf + diff_yaw;
	
	
	/*
	 //--- çHéñíÜ ---
	 //pitchâÒì]óÕÇãÅÇﬂ,åWêîÇÇ©ÇØÇÈ
	 float k_yaw = 100.0 * fish[i].param->speed_max;
	 float yaw_rotate = move_xz * sinf(local_yawm) * k_yaw;
	 
	 //ïœà ÉxÉNÉgÉãÇÃêÊí[Ç©ÇÁë¨ìxÉxÉNÉgÉãÇ…êÇê¸ÇÇ®ÇÎÇµÇΩéûÇÃë¨ìxÉxÉNÉgÉãÇ∆êÇê¸ÇÃåì_Ç©ÇÁë¨ìxÉxÉNÉgÉãÇÃç™å≥Ç‹Ç≈ÇÃí∑Ç≥ÇãÅÇﬂÇÈ
	 float move_localbase_xz = move_xz * cosf(local_yawm);
	 
	 //ë¨ìxÉxÉNÉgÉãÇ…â¡Ç¶ÇÈÇ◊Ç´äpìxÇãÅÇﬂÇÈ
	 float diff_yaw = atan2f(yaw_rotate, move_localbase_xz);
	 
	 //ë¨ìxÉxÉNÉgÉãÇÃyawÇ…ë´ÇµçáÇÌÇπÇÈ
	 float newyaw = yawf + diff_pitch;
	 */
	
	//--- Å¢Å¢Å¢ Yaw Control Å¢Å¢Å¢ ---
	//---Å§Å§Å§  Pitch Control Å§Å§Å§ ---
	//forwardÉxÉNÉgÉãÇÃpitchÇãÅÇﬂÇÈ radian
	float pitchf = atan2f(-fish[i].forward.y, forward_xz);
	
	//moveÉxÉNÉgÉãÇÃpitchÇãÅÇﬂÇÈ radian
	float pitchm = atan2f(-fish[i].move.y, move_xz);
	
	//moveÉxÉNÉgÉãÇÃÉçÅ[ÉJÉãÇ…Ç®ÇØÇÈpitchÇãÅÇﬂÇÈ radian
	float local_pitchm = pitchm - pitchf;
	
	//äpìxÇÃç∑ï™Ç…åWêîÇÇ©ÇØÇÈ(0~1ÇÃílÇéÊÇÈ)
	float k_pitch = 0.2;
	float diff_pitch = k_pitch * local_pitchm;
	
	//ë¨ìxÉxÉNÉgÉãÇÃpitchÇ…ë´ÇµçáÇÌÇπÇÈ
	float newpitch = pitchf + diff_pitch;
	
	/*
	 //--- çHéñíÜ ---
	 //pitchâÒì]óÕÇãÅÇﬂ,åWêîÇÇ©ÇØÇÈ
	 float k_pitch = 0.1;
	 float pitch_rotate = move_yz * sinf(local_pitchm) * k_pitch;
	 
	 //ïœà ÉxÉNÉgÉãÇÃêÊí[Ç©ÇÁë¨ìxÉxÉNÉgÉãÇ…êÇê¸ÇÇ®ÇÎÇµÇΩéûÇÃë¨ìxÉxÉNÉgÉãÇ∆êÇê¸ÇÃåì_Ç©ÇÁë¨ìxÉxÉNÉgÉãÇÃç™å≥Ç‹Ç≈ÇÃí∑Ç≥ÇãÅÇﬂÇÈ
	 float move_localbase_vertical = move_yz * cosf(local_pitchm);
	 
	 //ë¨ìxÉxÉNÉgÉãÇ…â¡Ç¶ÇÈÇ◊Ç´äpìxÇãÅÇﬂÇÈ
	 float diff_pitch = atan2f(pitch_rotate, move_localbase_vertical);
	 
	 //ë¨ìxÉxÉNÉgÉãÇÃpitchÇ…ë´ÇµçáÇÌÇπÇÈ
	 float newpitch = pitchf + diff_pitch;
	 */
	
	//--- Å¢Å¢Å¢ Pitch Control Å¢Å¢Å¢ ---
	//--- Å§Å§Å§  Thrust Control Å§Å§Å§ ---
	
	float move_localbase_xz = move_xz * cosf(local_yawm);
	//êÑêióÕ(move.z)ÇãÅÇﬂÇƒÅCåWêîÇÇ©ÇØÇÈ
	float k_thrust = 2.0;
	float thrust = move_localbase_xz * k_thrust;
	
	forward_xz +=thrust;
	
	//ë¨ìxÇ…êßå¿ÇÇ©ÇØÇÈ
	float speed_max = fish[i].param->speed_max;
	if(forward_xz > speed_max )
		forward_xz = speed_max;
	
	/*
	 //ë¨ìxÉxÉNÉgÉãÇÃê≥ãKâª
	 Vector3 normalized_forward;
	 float length_forward = GetVector3Length(&fish[i].forward);
	 normalized_forward.x = fish[i].forward.x / length_forward;
	 normalized_forward.y = fish[i].forward.y / length_forward;
	 normalized_forward.z = fish[i].forward.z / length_forward;
	 
	 //ë¨ìxÉxÉNÉgÉãÇ…êÑêióÕÇìKópÇ∑ÇÈ
	 fish[i].forward.x = normalized_forward.x * (length_forward * thrust);
	 fish[i].forward.y = normalized_forward.y * (length_forward * thrust);
	 fish[i].forward.z = normalized_forward.z * (length_forward * thrust);
	 
	 /*
	 //--- êÑêióÕÇãÅÇﬂÇÈ ---(move.z)
	 float thrust = move_xz * -cosf(local_yawm);
	 
	 
	 float thrust_max = 0.01;
	 if(thrust < thrust_max && thrust > -thrust_max)
	 {
	 if(thrust > 0.0)
	 thrust = thrust_max;
	 else
	 thrust = -thrust_max;
	 }
	 
	 float k_thrust = 0.1;//åWêî
	 //êÑêióÕÇâ¡Ç¶ÇÈ
	 if(thrust != 0.0 && forward_xz != 0.0)
	 {
	 forward_xz *= (1 + (k_thrust * thrust)) / forward_xz;
	 
	 float speed_max = fish[i].param->speed_max;
	 if(forward_xz > speed_max )
	 forward_xz = speed_max;
	 }
	 */
	
	//yawÇå≥Ç…ÉxÉNÉgÉãÇçƒç\íz(forward i+1)
	fish[i].forward.x = -sinf(newyaw) * forward_xz;
	fish[i].forward.z = -cosf(newyaw) * forward_xz;
	
	//êVÇΩÇ»pitchÇå≥Ç…ÉxÉNÉgÉãÇçƒç\íz
	fish[i].forward.y = -sinf(newpitch) * forward_yz;
	//fish[i].forward.z = -cosf(newpitch) * forward_yz;
	
	//YawÇÃïœâªÇ…âûÇ∂ÇƒRollÇÃílÇïœçXÇ∑ÇÈÅB
	//float k_roll = 500.0;
	//fish[i].rot.z = yaw_rotate * k_roll;
	
}

/*-------------------------------------------------------------- Cruising
 * Cruising : èÑçq 3éÌóﬁÇÃà⁄ìÆó Ççáê¨Ç∑ÇÈ
 * forwardÉxÉNÉgÉãÇÕãõÇÃë¨ìxÇï\Ç∑ÉxÉNÉgÉã
 * moveÉxÉNÉgÉãÇÕåvéZÇÃåãâ ì±Ç´èoÇ≥ÇÍÇΩï˚å¸ïœà ÇÃÉxÉNÉgÉã
 *--------*/
void Cruising (int i, FishDataT fish[])
{
	//--- moveÉxÉNÉgÉãÇÃçXêVÇàÍíËÇÃämó¶Ç≈çsÇ§ ----
	float update_factor = Random(0.0, 2.0);
	if(update_factor > 1.0)
		MakeMoveVector(i, fish);
	
	Vector3 nextpos  = VectorAdd(&fish[i].pos, &fish[i].forward);//éüÇÃÉtÉåÅ[ÉÄÇ≈ÇÃà íu
	
	//--- êÖÇÃíÔçRÇ…ÇÊÇ¡Çƒë¨ìxÇå∏êäÇ≥ÇπÇÈ ---
	fish[i].forward = VectorScalar(&fish[i].forward, 0.99);
	
	
	//----- êÖëÖäOÇ…ÇÕÇ›èoÇƒÇµÇ‹Ç¢ÇªÇ§Ç»èÍçáêÖëÖÇ…âàÇ§à íuÇ…à⁄ìÆÇêßå¿Ç∑ÇÈ -----
	float length = GetVector3Length(&nextpos);//êÖëÖÇÃíÜêSÇ©ÇÁÇÃãóó£
	
	//--- XZïΩñ Ç…Ç®Ç¢ÇƒÇÃêßå¿ ---
	if(length > AQUARIUM_MAX)
	{
		nextpos.x = (nextpos.x * AQUARIUM_MAX / length);
		nextpos.z = (nextpos.z * AQUARIUM_MAX / length);
	}
	//--- çÇÇ≥óÃàÊÇ…Ç®ÇØÇÈêßå¿(è„ïî) ---
	if(nextpos.y > (HEIGHT/2) )
	{
		nextpos.y = HEIGHT/2;
	}
	//--- çÇÇ≥óÃàÊÇ…Ç®ÇØÇÈêßå¿(íÍïî) ---
	if(nextpos.y < -(HEIGHT/2) )
	{
		nextpos.y = -(HEIGHT/2);
	}
	
	
	//----- à⁄ìÆó Çà íuÇ…ó^Ç¶ÇÈ -----
	fish[i].pos = nextpos;
	
	//----- à⁄ìÆï˚å¸Çå¸Ç≠ ------
	//----- pitch -----
	fish[i].rot.x = RadtoDeg( atan2f (fish[i].forward.y, GetVector2Length (fish[i].forward.x, fish[i].forward.z)));
	//----- yaw -----
	fish[i].rot.y = RadtoDeg ( atan2f (-fish[i].forward.x, -fish[i].forward.z));
	
	
	//----- ç∂âEÇ÷ÇÃÉuÉåÇâ¡Ç¶ÇÈ -----
	fish[i].rot.y += Gaussian(-15.0, 15.0);
	
	
}
//end of file

