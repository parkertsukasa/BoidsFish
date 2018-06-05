#include <stdio.h>
#include "hud.h"
#include "fish.h"

extern Selected ui;
extern Parameter Rparam;
extern Parameter Gparam;
extern Parameter Bparam;

typedef struct 
{
  float data[100];
  int active_num;
} LineData;

LineData Rcohesion;

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

/*------------------------------------------------- DrawLineGraph
 * DrawLineGraph : 折れ線グラフの表示
 * 100フレーム更新ごとにリフレッシュ
 */
void DrawLineGraph (LineData line, float data)
{
  //----- データの更新 -----
  line.data[line.active_num] = data;
  line.active_num += 1;

  if(line.active_num > 100)
  {
    printf("refresh!");
    //--- グラフのリフレッシュ ---
    for(int i = 0; i < 100; i++)
    {
      line.data[i] = 0.5;
    }
    line.active_num = 0;
  }

  //----- 折れ線の描画 -----
  glPushMatrix ();
  glBegin ( GL_LINE_LOOP );
    for (int i = 0; i < 100; i ++)
    {
      glVertex2f (i * 0.01, line.data[i]);
    }
  glEnd ();
  glPopMatrix ();
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

void DrawGraph ()
{
  setAlphaMaterial(1.0, 0.0, 0.0, 0.5);
  DrawDataBar (1, Rparam.cohesion);

  setAlphaMaterial(0.0, 1.0, 0.0, 0.5);
  DrawDataBar (2, Gparam.cohesion);

  setAlphaMaterial(0.0, 0.0, 1.0, 0.5);
  DrawDataBar (3, Bparam.cohesion);


  setAlphaMaterial(1.0, 0.0, 0.0, 0.5);
  DrawLineGraph (Rcohesion, Rparam.cohesion);
}


/*------------------------------------------------- InitHUD
 * InitHUD : HUDに使うデータの初期化を行う．
 * fish.cppのInitScene ()から呼び出す．
 */
void InitHUD ()
{
  for (int i = 0; i < 100; i++)
  {
    Rcohesion.data[i] = 0.5;
  }

  Rcohesion.active_num = 0;
  
}

/*------------------------------------------------- DrawHUDScene
 * DrawHUDScene : HUDの描画全体.
 * fish.cppのUpdate()から呼び出す．
 */
void DrawHUDScene ()
{
  if(ui.graph)
  {
    DrawGraphOutLine();

    glEnable (GL_BLEND);
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    DrawGraph();
    glDisable (GL_BLEND);
  }
}
//end of file
