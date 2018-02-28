struct Vector3
{
  float x, y, z;
};


float RadtoDeg (float f);
float DegtoRad (float f);
float GetVector2Length ( float x, float y );
float GetVector3Length ( Vector3 *v );
float GetInnerProduct (float x1, float y1, float x2, float y2);
float GetVector2Angle (float x1, float y1, float x2, float y2);
float GetVector3Angle (Vector3 *v1, Vector3 *v2);

Vector3 VectorAdd (Vector3 *v1, Vector3 *v2);
Vector3 VectorDiff (Vector3 *v1, Vector3 *v2);
Vector3 VectorScalar (Vector3 *v1, float f);
Vector3 VectorDivi (Vector3 *v1, float f);
Vector3 VectorZero ();


