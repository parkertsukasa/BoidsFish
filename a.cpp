
GameObject array[];

float minlength = 0.0;//最小の値を入れる変数
int nearnumber = 0;

for (int i = 0; i < array.Length -1; i++)
{
  //ベクトル求める
  float length = Vector3.Length;//距離を求める
  if(minlength > length)
  {
    minlength = length;
    nearnumber = i;
  }
}

array[nearnumber];
