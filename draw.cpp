#include <stdlib.h>
#include <stdio.h>

#include "platform.h"

#include "common.h"

#include "sim.h"
#include "util.h"
#include "myShape.h"
//#include "mymodel.h"

extern WindowDataT window;
extern SimDataT simdata;

extern CamObj camobj;
extern MouseDataT mouse;

extern FishDataT Rfish[];
extern FishDataT Gfish[];
extern FishDataT Bfish[];
extern FeedDataT feed[];

extern Selected interface;

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

  glColor3f(r,g,b);  

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

/*--------------------------------------------------------- drawWireSphere
 * drawWireSphere:
 *--------*/
void drawWireSphere( void )
{
	glPushMatrix();
	{
		glTranslatef( 0.0, 0.0, 0.0 );    //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 0.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
		glutWireSphere( 1.0, 18.0, 16.0 );   //半径，経度方向分割数，緯度方向分割数
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
		glTranslatef( 0.0, 0.0, -0.75 );    //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 0.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
    glScalef(0.5, 0.5, 0.5);
		glutSolidCone( 0.7, 5.0, 3.0, 5.0 );   //半径，高さ, 円の分割, 高さの分割
	}
	glPopMatrix();
    return;
}

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
      glVertex3f(0.0, 0.0, -1.0);
      glVertex3f(-0.5, -0.5, 0.0);
      glVertex3f(0.0, 0.0, 2.0);
    glEnd(); 

    glBegin( GL_POLYGON );  
      glVertex3f(0.0, 0.0, -1.0);
      glVertex3f(0.5, -0.5, 0.0);
      glVertex3f(0.0, 0.0, 2.0);
    glEnd(); 
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
    setMaterialColor(1.0, 1.0, 1.0);
    glTranslatef(mouse.x, mouse.y, mouse.z);
    glScalef(POINTRADIUS, POINTRADIUS, POINTRADIUS);
    drawWireSphere();
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
      setMaterialColor(1.0, 1.0, 0.0);
      glTranslatef(feed[j].pos.x, feed[j].pos.y, feed[j].pos.z);
		  glRotatef( feed[j].rot.x, 0.0, 1.0, 0.0 ); //オブジェクト基準姿勢調整：ヨー角
		  glRotatef( feed[j].rot.y, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		  glRotatef( feed[j].rot.z, 0.0, 0.0, 1.0 ); //オブジェクト基準姿勢調整：ロール角
      float size = feed[j].amount / 15;
      glScalef(size, size, size);
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
    setMaterialColor(interface.color.r, interface.color.g, interface.color.b);
    glTranslatef(0.0, 0.0, 0.0);
    //glutWireCube(100.0);
    //glutWireSphere( AQUARIUM_MAX, 18.0, 16.0 );   //半径，経度方向分割数，緯度方向分割数
    myWireCylinder( AQUARIUM_MAX, HEIGHT, 20 ); //半径50, 高さ100, 20角形の角柱
  glPopMatrix();
}

/*---------------------------------------------------------------- drawFish
 * drawFish:
 *--------*/
void drawFish (int i, FishDataT fish[])
{
  //glEnable(GL_LIGHTING);

	glPushMatrix();
	{
    setMaterialColor(fish[i].color.r, fish[i].color.g, fish[i].color.b);
		glTranslatef(fish[i].pos.x, fish[i].pos.y, fish[i].pos.z);   //オブジェクト基準位置調整
		glRotatef( fish[i].rot.y, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
		glRotatef( fish[i].rot.x, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( fish[i].rot.z, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
    glGetFloatv( GL_MODELVIEW_MATRIX, fish[i].mat);//変換マトリクスの取得
    drawFishModel();
  }
  glPopMatrix();

  //glDisable(GL_LIGHTING);


  if(interface.debug)
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
