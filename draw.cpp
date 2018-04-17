#include <stdlib.h>
#include <stdio.h>

#include "platform.h"

#include "common.h"

#include "fish.h"

#include "util.h"
//#include "mymodel.h"

extern WindowDataT window;
extern SimDataT simdata;

extern CamObj camobj;
extern MouseDataT mouse;


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


/*---------------------------------------------------------------- DrawScene
 * DrawScene:
 *--------*/
void DrawScene( void )
{		

	glPushMatrix();

		drawMouseObj ();
	
	glPopMatrix();
	
	FishDrawScene();

    return;
}

//end of file
