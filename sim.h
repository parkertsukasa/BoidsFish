// sim.cpp
void InitScene( void );
void UpdateScene( void );

// draw.cpp
void DrawScene( void );


#define AQUARIUM_MAX 50
#define AQUARIUM_MIN -50

#define LENGTH 200
#define FEEDLENGTH 10



//-------- object data
typedef struct {
	float pos[3];
	float rot[3];
	int state;
	bool visible;
} ObjDataT;

typedef struct {

	float x;
	float y;
	float z;

} Vector3;

typedef struct {

	float r;
	float g;
	float b;
  float a;

} Color;

//-------- simulation data
typedef struct {

	float clip_near;
	float clip_far;
	float air_color[4];
	float sky_color[4];
    //////// ユーザ定義のシーンについてのデータをここにまとめる

	////////
} SimDataT;

//-------- Camera Settings
typedef struct {
    
  Vector3 pos;
  Vector3 rot;

  int up;

} CamDataT;

typedef struct {

	float x;
	float y;
} MouseDataT;

typedef struct {
  float speed;
  float radius;
  float thete;
  Vector3 pos;
  Vector3 center;
} CamObj;

typedef struct {
  Vector3 pos;
  Vector3 rot;
  float amount;
  bool alive;
} FeedDataT;

typedef struct {

  Vector3 pos;
  Vector3 rot;
  Vector3 move;
  Vector3 forward;
  GLfloat mat[16];
  float range;
  float sightangle;
  float speed;
  bool hungry;
  int feednum;

  Color color;
}FishDataT;

float RadtoDeg (float f);
float DegtoRad (float f);
float GetVector2Length ( float x, float y );
float GetInnerProduct (float x1, float y1, float x2, float y2);
float GetVector2Angle (float x1, float y1, float x2, float y2);
float GetVector3Angle (float x1, float y1, float z1, float x2, float y2, float z2);

void CameraRotate();
void MouseObj();

void FeedControl(int j);
void GiveFeed (float x, float y);

void FishInit(int i, FishDataT fish[]);
void FishColorChange(Color *color, float r, float g, float b, float a);

void Cruising (int i, FishDataT fish[]);
Vector3 Gather(int i, FishDataT fish[]);
Vector3 Separate(int i, FishDataT fish[]);
Vector3 Enclose(int i, FishDataT fish[]);
Vector3 Align(int i, FishDataT fish[]);
Vector3 EatFeed(int i, FishDataT fish[]);
Vector3 Avoid(int i, FishDataT fish[]);

void ColorChange (int i);

//end of file
