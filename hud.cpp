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
 
static LineData Rcohesion;
static LineData Gcohesion;
static LineData Bcohesion;

/*----------------------------------------------- LineDataInit
 * LineDataInit : LineDataの初期化を行う関数
 */
void LineDataInit (LineData *line)
{
  for (int i = 0; i < 100; i++)
  {
    line->data[i] = 0.5;
  }
  line->active_num = 50;
}

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


/*------------------------------------------------- DrawLineGraph
 * DrawLineGraph : 折れ線グラフの表示
 */
void DrawLineGraph (LineData line, float data)
{
  //----- 折れ線の描画 -----
  glPushMatrix ();
  glBegin ( GL_LINE_STRIP );
    for (int i = 0; i < 100; i ++)
    {
      glVertex2f ((float)i * 0.01, line.data[i]);
    }
  glEnd ();
  glPopMatrix ();
}


/*------------------------------------------------ LineGraphSimulation
 * LineGraphSimulation : 折れ線グラフを描画するのに必要なデータの更新を行う
 * 100フレーム毎にリフレッシュする．
 */
void LineGraphSimulation (LineData *line, float data)
{
  //----- データの更新 -----
  line->data[line->active_num] = data;
  line->active_num += 1; 
  //printf("%f\n", data);

  if(line->active_num > 100)
  {
    //--- グラフのリフレッシュ ---
    for(int i = 0; i < 100; i++)
    {
      line->data[i] = 0.5;
    }
    line->active_num = 0;
  }
}

void DrawGraph ()
{
  setAlphaMaterial(1.0, 0.0, 0.0, 0.5);
  DrawDataBar (1, Rparam.cohesion);
  DrawLineGraph (Rcohesion, Rparam.cohesion);

  setAlphaMaterial(0.0, 1.0, 0.0, 0.5);
  DrawDataBar (2, Gparam.cohesion);
  DrawLineGraph (Gcohesion, Gparam.cohesion);

  setAlphaMaterial(0.0, 0.0, 1.0, 0.5);
  DrawDataBar (3, Bparam.cohesion);
  DrawLineGraph (Bcohesion, Bparam.cohesion);

}


/*------------------------------------------------- InitHUD
 * InitHUD : HUDに使うデータの初期化を行う．
 * fish.cppのInitScene ()から呼び出す．
 */
void InitHUD ()
{
  LineDataInit(&Rcohesion);
  LineDataInit(&Gcohesion);
  LineDataInit(&Bcohesion);
}

/*------------------------------------------------- HUDUpdate
 * HUDUpdate : HUDの描画で扱うデータのシミュレーションを行う
 * fish.cppのFishUpdate()から呼び出す．
 * シミュレーション関数なので描画処理は行わない．
 */
void HUDUpdate ()
{
  LineGraphSimulation(&Rcohesion, Rparam.cohesion); 
  LineGraphSimulation(&Gcohesion, Gparam.cohesion); 
  LineGraphSimulation(&Bcohesion, Bparam.cohesion); 
}

/*------------------------------------------------- DrawHUDScene
 * DrawHUDScene : HUDの描画全体.
 * main.cppのsetHUD ()から呼び出されている．
 * 描画関数のためシミュレーションは行わない．．
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
