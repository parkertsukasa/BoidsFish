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
 * EvaluateCohesion : 集合度を評価する
 * 全個体の位置の平均をPa，種族の個体数をnとして，
 * 各個体の位置PnとPaとの差の2乗の総和をnで割り正規化したものの平方根を集合度とする．
 * 参考 : 統計学，標準偏差
 */
void EvaluateCohesion (FishDataT fish[])
{
  //--- 全個体の平均位置Paを求める ---
  Vector3 Pa = VectorZero();
  for (int i = 0; i < fish[0].param->length; i++)
  {
    Pa = VectorAdd(&Pa, &fish[i].pos);
  }
  Pa.x /= (float)fish[0].param->length;
  Pa.y /= (float)fish[0].param->length;
  Pa.z /= (float)fish[0].param->length;

  //---- 各個体と平均位置との差の2乗の2乗の総和を求める ---
  float sigma_length = 0.0;
  for (int j = 0; j < fish[0].param->length; j++)
  {
    Vector3 diff = VectorDiff(&Pa, &fish[j].pos);
    float length = GetVector3Length(&diff);
    sigma_length += (length * length);
  }

  //--- 総和を個体数で割る ---
  sigma_length /= fish[0].param->length;

  //--- 標準偏差を求める
  float deviation = sqrtf(sigma_length);

  //--- 個々のデータの取りうる値の最大は水槽の対角線の長さ ---
  static float diameter = 2.0 * AQUARIUM_MAX;//直径
  static float diagonal = sqrt((diameter * diameter) + (HEIGHT * HEIGHT));//対角線
  static float maxvalue = sqrtf(diagonal * diagonal);

  float temp = deviation / maxvalue;
  fish[0].param->cohesion = 1.0 - temp;

}

//emd of file
