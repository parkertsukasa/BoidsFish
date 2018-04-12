void InitScene( void );
void UpdateScene( void );

// draw.cpp
void DrawScene( void );


#define AQUARIUM_MAX 50
#define AQUARIUM_MIN -50
#define HEIGHT 50

#define POINTRADIUS 5

#define R_LENGTH 50
#define G_LENGTH 100
#define B_LENGTH 150

#define FEEDLENGTH 10

#include "platform.h"

#include "interaction.h"
#include "vector.h"


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

enum kind {RED, BLUE, GREEN, WHITE};


//-------- Camera Settings
typedef struct {
    
  Vector3 pos;
  Vector3 rot;

  int up;

} CamDataT;

typedef struct {
	float x;
	float y;
  float z;
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
  kind select;
  Color color;
  bool debug;
}Selected;

typedef struct {
  int length;
  float speed_max;
  float sightangle;
  float sightrange;
  float kc;
  float ks;
	float ka;
	float kch;
	float kes;
}Parameter;


void CameraRotate();
void MouseObj();

void FeedControl(int j);
void GiveFeed (float x, float y, float z);


int DensitySerch();

//----- fish.cpp -----
typedef struct {
    kind species;
    
    Vector3 pos;
    Vector3 rot;
    Vector3 move;
    Vector3 forward;
    Vector3 wall;
    GLfloat mat[16];
    int feednum;

    Parameter *param;

    Color color;
}FishDataT;

void FishInit();
void FishUpdate();

void FishStructInit(int i, FishDataT fish[], kind spc);
void ColorChange(Color *color, float r, float g, float b, float a);
void ParameterSet();

void Cruising (int i, FishDataT fish[]);
Vector3 Gather(int i, FishDataT fish[]);
Vector3 Separate(int i, FishDataT fish[]);
Vector3 Enclose(int i, FishDataT fish[]);
Vector3 Align(int i, FishDataT fish[]);
Vector3 EatFeed(int i, FishDataT fish[]);
Vector3 Avoid(int i, FishDataT fish[]);
Vector3 Chase(int i, FishDataT fish[]);
Vector3 Escape(int i, FishDataT fish[]);

//end of file
