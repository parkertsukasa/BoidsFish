#include <stdlib.h>
#include <stdio.h>

#include "fish.h"

#include "util.h"
#include "myShape.h"

extern FishDataT Rfish[];
extern FishDataT Gfish[];
extern FishDataT Bfish[];
extern FeedDataT feed[];

extern Selected ui;

/* --------------------------------------------------------- drawLine
 * drawLine
 */
void drawLine(Vector3 *b, Vector3 *e)
{
	glBegin( GL_LINES );
	glVertex3f(b->x, b->y, b->z);
	glVertex3f(e->x, e->y, e->z);
	glEnd();
}


/*--------------------------------------------------------- drawFishModel
 * drawFishModel:
 *--------*/
void drawFishModel()
{
	glPushMatrix();
	{
		glBegin( GL_POLYGON );
		glVertex3f(0.0, 0.0, 2.0);
		glVertex3f(0.0, 0.0, -1.0);
		glVertex3f(-0.5, -0.5, 0.0);
		glEnd();
		
		glBegin( GL_POLYGON );
		glVertex3f(0.0, 0.0, -1.0);
		glVertex3f(0.0, 0.0, 2.0);
		glVertex3f(0.5, -0.5, 0.0);
		glEnd();
	}
	glPopMatrix();
	return;
}


/*---------------------------------------------------------------- drawFeed
 * drawFeed:
 *--------*/
void drawFeed (int j)
{
	if (feed[j].alive)
	{
		float rotspeed = 1.0;
		feed[j].rot.x += rotspeed;
		if(feed[j].rot.x > 360.0)
			feed[j].rot.x = 0.0;
		feed[j].rot.y += rotspeed;
		if(feed[j].rot.y > 360.0)
			feed[j].rot.y = 0.0;
		feed[j].rot.z += rotspeed;
		if(feed[j].rot.z > 360.0)
			feed[j].rot.z = 0.0;
		
		glPushMatrix();
		setMaterialColor(1.0, 1.0, 0.0);
		glTranslatef(feed[j].pos.x, feed[j].pos.y, feed[j].pos.z);
		glRotatef( feed[j].rot.x, 0.0, 1.0, 0.0 ); //オブジェクト基準姿勢調整：ヨー角
		glRotatef( feed[j].rot.y, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( feed[j].rot.z, 0.0, 0.0, 1.0 ); //オブジェクト基準姿勢調整：ロール角
		float size = feed[j].amount / 25;
		glScalef(size, size, size);
		drawSolidCube ();
		glPopMatrix();
	}
}

/*---------------------------------------------------------------- drawFish
 * drawFish:
 *--------*/
void drawFish (int i, FishDataT fish[])
{
	//glEnable(GL_LIGHTING);
	  
  if( fish[i].alive )
  {
	  glPushMatrix();
	  {
	  	setMaterialColor(fish[i].color.r, fish[i].color.g, fish[i].color.b);
	  	glTranslatef(fish[i].pos.x, fish[i].pos.y, fish[i].pos.z);   //オブジェクト基準位置調整
	  	glRotatef( fish[i].rot.y, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
	  	glRotatef( fish[i].rot.x, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
	  	glRotatef( fish[i].rot.z, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
	  	glGetFloatv( GL_MODELVIEW_MATRIX, fish[i].mat);//変換マトリクスの取得
	  	
	  	if(fish[i].species == GREEN)
	  		glScalef(0.75, 0.75, 0.75);
	  	
	  	if(fish[i].species == BLUE)
	  		glScalef(0.5, 0.5, 0.5);
	  	
	  	drawFishModel();
	  }
	  glPopMatrix();
	}

	//glDisable(GL_LIGHTING);
	
	
	if(ui.debug)
	{
		glPushMatrix();
		{
			// ----- デバッグ用の線の描画 -----
			glColor3f(1.0, 1.0, 0.0);
			Vector3 bigmove = VectorScalar(&fish[i].move , -150.0);
			Vector3 movepoint = VectorAdd(&fish[i].pos, &bigmove);
			drawLine(&fish[i].pos, &movepoint);
			
			glColor3f(1.0, 1.0, 1.0);
			
			Vector3 bigforward = VectorScalar(&fish[i].forward, 50.0);
			Vector3 forwardpoint = VectorAdd(&fish[i].pos, &bigforward);
			drawLine(&fish[i].pos, &forwardpoint);
		}
		glPopMatrix();
	}
}


/*---------------------------------------------------------------- drawAquarium
 * drawAquarium:
 *--------*/
void drawAquarium()
{
	glPushMatrix();
	setMaterialColor(ui.color.r, ui.color.g, ui.color.b);
	glTranslatef(0.0, 0.0, 0.0);
	//glutWireCube(100.0);
	//glutWireSphere( AQUARIUM_MAX, 18.0, 16.0 );   //半径，経度方向分割数，緯度方向分割数
	myWireCylinder( AQUARIUM_MAX, HEIGHT, 20 ); //半径50, 高さ100, 20角形の角柱
	glPopMatrix();
}





/*---------------------------------------------------------------- FishDrawScene
 * FishDrawScene:
 *--------*/
void FishDrawScene()
{
	glPushMatrix();
	
		drawAquarium ();
	
		for (int i = 0; i < R_LENGTH; i++)
		{
			drawFish(i, Rfish);
		}
	
		for (int i = 0; i < G_LENGTH; i++)
		{
			drawFish(i, Gfish);
		}
	
		for (int i = 0; i < B_LENGTH; i++)
		{
			drawFish(i, Bfish);
		}
	
		for (int j = 0; j < FEEDLENGTH; j++)
		{
			drawFeed(j);
		}
	
	glPopMatrix();

}
