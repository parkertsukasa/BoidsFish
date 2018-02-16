#include <stdlib.h>
#include <stdio.h>

#include "platform.h"

#include "common.h"


#include "sim.h"
#include "util.h"
#include "mymodel.h"

extern WindowDataT window;
extern SimDataT simdata;

extern CamObj camobj;
extern MouseDataT mouse;

extern FishDataT Rfish[];
extern FishDataT Gfish[];
extern FishDataT Bfish[];
extern FeedDataT feed[];

//------- prototypes for user functions

void setMaterialColor( float r, float g, float b );
void setEmissiveColor( float r, float g, float b );

/*--------------------------------------------------------- setMaterialColor
 * setMaterialColor:
 *--------*/
void setMaterialColor( float r, float g, float b )
{
    float diffuse[4];
    float specular[] = { 0.8, 0.8, 0.8, 1.0 };

	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = 1.0;

    glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
    glMaterialf( GL_FRONT, GL_SHININESS, 32.0 );

    return;
}
/*--------------------------------------------------------- setEmissiveColor
 * setEmissiveColor:
 *--------*/
void setEmissiveColor( float r, float g, float b )
{
    float diffuse[4];
    float specular[] = { 0.8, 0.8, 0.8, 1.0 };

	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = 1.0;

    glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
    glMaterialf( GL_FRONT, GL_SHININESS, 32.0 );
    glMaterialfv( GL_FRONT, GL_EMISSION, diffuse );

    return;
}
/*----------------------------------------------------------- drawSolidCube
 * drawSolidCube:
 *--------*/
void drawSolidCube( void )
{
	glPushMatrix();
	{
    setMaterialColor(0.9, 0.8, 0.1);
		glTranslatef( 0.0, 0.0, 0.0 );   //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 ); //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 0.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 ); //オブジェクト基準姿勢調整：ロール角
		glutSolidCube( 1 );
	}
	glPopMatrix();
    return;
}
/*--------------------------------------------------------- drawSolidSphere
 * drawSolidSphere:
 *--------*/
void drawSolidSphere( void )
{
	glPushMatrix();
	{
		glTranslatef( 0.0, 0.0, 0.0 );    //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 0.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
		glutSolidSphere( 1.0, 18.0, 16.0 );   //半径，経度方向分割数，緯度方向分割数
	}
	glPopMatrix();
    return;
}

/*--------------------------------------------------------- drawSolidCone
 * drawSolidCone:
 *--------*/
void drawSolidCone()
{
		glPushMatrix();
	{
    setMaterialColor(1.0, 1.0, 1.0);
		glTranslatef( 0.0, 0.0, 0.0 );    //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 0.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
		glutSolidCone( 0.7, 5.0, 10.0, 5.0 );   //半径，高さ, 円の分割, 高さの分割
	}
	glPopMatrix();
    return;
}


/*--------------------------------------------------------- drawFishModel
 * drawFishModel:
 *--------*/
void drawFishModel()
{
		glPushMatrix();
	{
    setMaterialColor(1.0, 1.0, 1.0);
		glTranslatef( 0.0, 0.0, 0.0 );    //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 0.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
		//DrawMyFishModel ();   
  }
	glPopMatrix();
    return;
}

/*---------------------------------------------------------------- drawMouseObj
 * drawMouseObj:
 *--------*/
void drawMouseObj()
{
  glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(mouse.x, 0.0, mouse.y);
    drawSolidSphere();
  glPopMatrix();
}


/*---------------------------------------------------------------- drawFeed
 * drawFeed:
 *--------*/
void drawFeed (int j)
{
  if (feed[j].alive)
  {
    glPushMatrix();
      glTranslatef(feed[j].pos.x, feed[j].pos.y, feed[j].pos.z);
      drawSolidCube ();
    glPopMatrix();
  }
}


/*---------------------------------------------------------------- drawAquarium
 * drawAquarium:
 *--------*/
void drawAquarium()
{
  glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.0, 0.0);
    glutWireCube(100.0);
  glPopMatrix();
}

/*---------------------------------------------------------------- drawFish
 * drawFish:
 *--------*/
void drawFish (int i, FishDataT fish[])
{
		glPushMatrix();
	{
    glColor3f(fish[i].color.r, fish[i].color.g, fish[i].color.b);
		glTranslatef(fish[i].pos.x, fish[i].pos.y, fish[i].pos.z);   //オブジェクト基準位置調整
		glRotatef( fish[i].rot.y, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
		glRotatef( fish[i].rot.x, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( fish[i].rot.z, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
    glGetFloatv( GL_MODELVIEW_MATRIX, fish[i].mat);//変換マトリクスの取得
		drawSolidCone();
    //drawFishModel();
	}
	glPopMatrix();
}


/*---------------------------------------------------------------- DrawScene
 * DrawScene:
 *--------*/
void DrawScene( void )
{		

	glPushMatrix();

		drawAquarium ();
    drawMouseObj ();

		for (int i = 0; i < LENGTH; i++)
		{
			drawFish(i, Rfish);
			drawFish(i, Gfish);
			drawFish(i, Bfish);
		}

    for (int j = 0; j < FEEDLENGTH; j++)
    {
      drawFeed(j);  
    }

	glPopMatrix();

    return;
}

//end of file
