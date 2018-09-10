#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"

/*--------------------------------------------------------------------------------------------
 * 数学的便利関数群
 */


/*-------------------------------------------------------------- RadtoDeg
 * ラジアンから度数へ変換する関数
 */
float RadtoDeg (float f)
{
  float deg = f * 180.0 / M_PI;
  return deg;
}

/*-------------------------------------------------------------- RadtoDeg
 * 度数からラジアンへ変換する関数
 */
float DegtoRad (float f)
{
  float rad = f * M_PI / 180.0;
  return rad;
}

/*-------------------------------------------------------------- GetVector2Length
 * GetVector2Length : ベクトルの長さを取得する(2軸)
 */
float GetVector2Length ( float x, float y )
{
  float length = sqrtf ( x * x + y * y );
  
  int nan = isnan(length);

  if(nan != 0)
    length = 0.0;

  return length;
}

/*-------------------------------------------------------------- GetVector3Length
 * GetVector3Length : ベクトルの長さを取得する(3軸)
 */
float GetVector3Length ( Vector3 *v )
{
  float length = sqrtf ((v->x * v->x) + (v->y * v->y) + (v->z * v->z));

  int nan = isnan(length); 

  //if(nan != 0)
  //  length = 0.0;

  return length;
}

/*-------------------------------------------------------------- GetVector3LengthSquare
 * GetVector3LengthSquare : ベクトルの長さの2乗を取得する(3軸)
 */
float GetVector3LengthSquare ( Vector3 *v )
{
  float length = ( (v->x * v->x) + (v->y * v->y) + (v->z * v->z));

  return length;
}

/*-------------------------------------------------------------- GetInnerProduct
 * GetInnerProduct : ベクトルの内積を求める
 */
float GetInnerProduct (float x1, float y1, float x2, float y2)
{
    float product = x1 * x2 + y1 * y2;
    return product;
}

/*-------------------------------------------------------------- GetVectorAngle
 * GetVector2Angle : ベクトルのなす角度を取得する
 */
float GetVector2Angle (float x1, float y1, float x2, float y2)
{
    //----- それぞれのベクトルの長さを求める -----
    float Alength = GetVector2Length (x1, y1);
    float Blength = GetVector2Length (x2, y2);

    //----- 内積を求める -----
    float product = GetInnerProduct (x1, y1, x2, y2);

    //----- cosθを求める -----
    float cos_theta = product / (Alength * Blength);

    //----- θを求める -----
    float theta = acosf(cos_theta);

    //----- ラジアンを度数に変換 -----
    float angle = theta * 180.0 / M_PI;

    return angle;
}

/*-------------------------------------------------------------- GetVectorAngle
 * GetVector3Angle : ベクトルのなす角度を取得する
 */
float GetVector3Angle (Vector3 *v1, Vector3 *v2)
{
    //----- それぞれのベクトルの長さを求める -----
    float Alength = GetVector3Length (v1);
    if( isnan( Alength ))
    {
      printf( "%s\n", "AlengthでNaNを検出");
      exit( 0 );
    }

    float Blength = GetVector3Length (v2);
    if( isnan( Blength ))
    {
      printf( "%s\n", "BlengthでNaNを検出");
      exit( 0 );
    }

    //----- 内積を求める -----
    float product = (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z); 
    if( isnan( product ))
    {
      printf( "%s\n", "productでNaNを検出");
      exit( 0 );
    }

    float cos_theta = product / (Alength * Blength);
    if( isnan( cos_theta ))
    {
      printf( "%s\n", "cos_theteでNaNを検出");
      exit( 0 );
    }

    if( cos_theta < -1.0 )
      cos_theta = -1.0;

    if( cos_theta > 1.0 )
      cos_theta = 1.0;

    //----- θを求める -----
    float theta = acosf(cos_theta);
    if( isnan( theta ))
    {
      printf( "%s\n", "theteでNaNを検出");
      exit( 0 );
    }

    //----- ラジアンを度数に変換 -----
    float angle = RadtoDeg(theta);

    return angle;
}

/* -------------------------------------------------------------- VectorAdd
 * VectorAdd ベクトル同士の足し算をする関数
 */
Vector3 VectorAdd(Vector3 *v1, Vector3 *v2)
{
  Vector3 v = VectorZero();
  v.x = v1->x + v2->x;
  v.y = v1->y + v2->y;
  v.z = v1->z + v2->z;

  return v;  
}


/* -------------------------------------------------------------- VectorDiff
 * VectorDiff ベクトル同士の引き算をする関数
 */
Vector3 VectorDiff(Vector3 *v1, Vector3 *v2)
{
  Vector3 v = VectorZero();
  v.x = v1->x - v2->x;
  v.y = v1->y - v2->y;
  v.z = v1->z - v2->z;

  return v;  
}


/* -------------------------------------------------------------- VectorScalar
 * VectorScalar ベクトルをスカラー倍する関数
 */
Vector3 VectorScalar(Vector3 *v1, float f)
{
  Vector3 v = VectorZero();
  v.x = v1->x * f;
  v.y = v1->y * f;
  v.z = v1->z * f;

  return v;  
}

/* -------------------------------------------------------------- VectorDivi
 * VectorDivi ベクトルを割り算する関数
 */
Vector3 VectorDivi(Vector3 *v1, float f)
{
  Vector3 v = VectorZero();
  if(v1->x > 0.0 && v1->y > 0.0 && v1->z > 0.0 && f > 0.0)
  {
    v.x = v1->x / f;
    v.y = v1->y / f;
    v.z = v1->z / f;
  }
  return v;  
}

/* ------------------------------------------------------------- VectorZero
 * VectorZero 全ての値が0のベクトルを返す関数
 */
Vector3 VectorZero()
{
  Vector3 v;
  v.x = 0.0;
  v.y = 0.0;
  v.z = 0.0;

  return v;
}

//end of file

