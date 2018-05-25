#include "sim.h"

#define AQUARIUM_MAX 50
#define AQUARIUM_MIN -50
#define HEIGHT 50

#define POINTRADIUS 5

#define R_LENGTH 50
#define G_LENGTH 100
#define B_LENGTH 200

#define FEEDLENGTH 10

typedef struct {
	
	float r;
	float g;
	float b;
	float a;
	
} Color;

enum kind {RED, BLUE, GREEN, WHITE};

typedef struct {
	kind select;
	Color color;
	bool debug;
  bool graph;
}Selected;


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
	int length;
	float speed_max;
	float sightangle;
	float sightrange;
	float kc;
	float ks;
	float ka;
	float kch;
	float kes;
  //--- 群れとしての状態を評価する値 ---
  float cohesion;
  float separation;
  float alignment;
}Parameter;


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

//----- fishdraw.cpp -----
void FishDrawScene();

//----- evaluate.cpp -----
void Evaluate ();

//end of file

