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

  bool up;

} CamDataT;

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
  bool hungry;
  int feednum;
  bool out;

  float head;
  float body;
  float tail;

}FishDataT;

float RadtoDeg (float f);
float GetVector2Length ( float x, float y );
float GetInnerProduct (float x1, float y1, float x2, float y2);
float GetVector2Angle (float x1, float y1, float x2, float y2);

void FeedControl(int j);
void GiveFeed (float x, float y);

void Cruising(int i);
Vector3 Cohesion(int i);
Vector3 Separation(int i);
Vector3 Alignment(int i);
Vector3 EatFeed(int i);

void ReturnAquarium(int i);

void SetPosition (int i);

