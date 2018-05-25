#include <stdio.h>
#include "hud.h"
#include "fish.h"

extern Selected ui;
extern Parameter Rparam;
extern Parameter Gparam;
extern Parameter Bparam;


/*------------------------------------------------- setAlphaMaterial
 * setAlphaMaterial : 透過のあるマテリアルを設定する
 */
void setAlphaMaterial (float r, float g, float b, float a)
{
  float diffuse[4];
  float specular[] = { 0.8, 0.8, 0.8, 1.0 };

	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
  
  glColor4f(r, g, b, a);

  glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
  glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
  glMaterialf( GL_FRONT, GL_SHININESS, 32.0 );

    return;
}

/*------------------------------------------------- DrawGraphOutLine
 * DrawGraphOutLine : グラフの枠線を表示する
 */
void DrawGraphOutLine ()
{
  glColor3f ( 1.0, 1.0, 1.0 );

  glPushMatrix();
  glBegin( GL_LINE_LOOP );
    glVertex2f ( 0.0, 0.0);
    glVertex2f ( 0.0, 1.0);
  glEnd ();
  glBegin( GL_LINE_LOOP );
    glVertex2f ( 0.0, 0.0);
    glVertex2f ( 1.0, 0.0);
  glEnd ();
  glPopMatrix();
}


/*------------------------------------------------- DrawDataBar
 * DrawDataBar : 棒グラフのバーを表示する
 * numにはデータ番号, dataには表示したいデータを渡す．
 * ただし，dataは0~1の間に丸めた上で渡す．
 */
void DrawDataBar (int num, float data)
{
  float l_point = (float)num * 0.1;
  float r_point = (float)num * 0.1 + 0.05;
  glBegin( GL_POLYGON );
    glVertex2f (l_point, 0.0);
    glVertex2f (l_point, data);
    glVertex2f (r_point, data);
    glVertex2f (r_point, 0.0);
  glEnd ();
}

void DrawLine ()
{
  setAlphaMaterial(1.0, 0.0, 0.0, 0.5);
  DrawDataBar (1, Rparam.cohesion);

  setAlphaMaterial(0.0, 1.0, 0.0, 0.5);
  DrawDataBar (2, Gparam.cohesion);

  setAlphaMaterial(0.0, 0.0, 1.0, 0.5);
  DrawDataBar (3, Bparam.cohesion);



}

void DrawHUDScene ()
{
  if(ui.graph)
  {
    DrawGraphOutLine();

    glEnable (GL_BLEND);
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    DrawLine();
    glDisable (GL_BLEND);
  }
}
//end of file
