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


//-------- simulation data
typedef struct {

	float clip_near;
	float clip_far;
	float air_color[4];
	float sky_color[4];
    //////// ユーザ定義のシーンについてのデータをここにまとめる

	////////
} SimDataT;



void CameraRotate();
void MouseObj();

void FeedControl(int j);
void GiveFeed (float x, float y, float z);


int DensitySerch();


