#include "fish.h"
#include <math.h>


extern FishDataT Rfish[R_LENGTH];
extern FishDataT Gfish[G_LENGTH];
extern FishDataT Bfish[B_LENGTH];
extern Parameter Rparam;
extern Parameter Gparam;
extern Parameter Bparam;

//----- ▽▽▽ 関数プロトタイプ宣言 ▽▽▽ -----
void Assign (FishDataT fish[]);
void EvaluateCohesion (FishDataT fish[]);

/* ---------------------------------------------------------- Evaluate
 * Evaluate : 評価関数 群行動に対する評価指数の計算
 */
void Evaluate ()
{
  Assign(Rfish);
  Assign(Gfish);
  Assign(Bfish);
}

/* ---------------------------------------------------------- Assign
 * Assign : 種族ごとに各評価関数を呼び出す
 */
void Assign (FishDataT fish[])
{
  EvaluateCohesion(fish);
}

/* ---------------------------------------------------------- EvaluateCohesion
 * EvaluateCohesion : 密集度を評価する
 */
void EvaluateCohesion (FishDataT fish[])
{
  //--- 全個体の平均位置を求める ---
  Vector3 ave = VectorZero();
  for (int i = 0; i < fish[0].param->length; i++)
  {
    ave = VectorAdd(&ave, &fish[i].pos);
  }
  ave.x /= (float)fish[0].param->length;
  ave.y /= (float)fish[0].param->length;
  ave.z /= (float)fish[0].param->length;

  //------ 各個体と平均位置との総和を求める -----
  float sigma_length = 0.0;
  for (int j = 0; j < B_LENGTH; j++)
  {
    Vector3 diff = VectorDiff(&ave, &fish[j].pos);
    float length = GetVector3Length(&diff);
    sigma_length += length;
  }

  //--- 取りうる値の最大は水槽の対角線の長さ * 魚の個体数 ---
  static float diameter = 2.0 * AQUARIUM_MAX;
  static float maxvalue = sqrt((diameter * diameter) + (HEIGHT * HEIGHT)) * fish[0].param->length;

  float temp = sigma_length / maxvalue;
  fish[0].param->cohesion = 1.0 - temp;

}

//emd of file
