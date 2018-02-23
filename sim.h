// sim.cpp
void InitScene( void );
void UpdateScene( void );

// draw.cpp
void DrawScene( void );


#define AQUARIUM_MAX 50
#define AQUARIUM_MIN -50

#define LENGTH 100
#define FEEDLENGTH 10

#include "platform.h"

#include "vector.h"
#include "interaction.h"

//-------- object data
typedef struct {
	float pos[3];
	float rot[3];
	int state;
	bool visible;
} ObjDataT;

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

typedef struct {
  enum kind {RED, BLUE, GREEN};
  kind select;
  Color color;
}Selected;

typedef struct {
  float speed;
  float sightangle;
  float sightrange;
  float kc, ks, ka;
}Parameter;


void CameraRotate();
void MouseObj();

void FeedControl(int j);
void GiveFeed (float x, float y);

void FishInit(int i, FishDataT fish[]);
void ColorChange(Color *color, float r, float g, float b, float a);

void Cruising (int i, FishDataT fish[]);
Vector3 Gather(int i, FishDataT fish[]);
Vector3 Separate(int i, FishDataT fish[]);
Vector3 Enclose(int i, FishDataT fish[]);
Vector3 Align(int i, FishDataT fish[]);
Vector3 EatFeed(int i, FishDataT fish[]);
Vector3 Avoid(int i, FishDataT fish[]);

void ColorChange (int i);

int DensitySerch();

//end of file
