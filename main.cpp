/******************************************************************************
 * glsample_frame.cpp 
 * Level 3 - base framework */

#include <stdlib.h>
#include <stdio.h>

#include "platform.h"

#include "common.h"

#include "sim.h"

//-------- callback functions
static void display( void );
static void update( void );
static void reshape( int width, int height );

//-------- init GL window
static void initWindow( char *title );

//-------- kbdmouse.cpp: mouse callback functions
void mouseClick( int button , int state, int x, int y );
void mouseDrag( int x, int y );
void mouseMotion( int x, int y );

//-------- kbdmouse.cpp: keyboard callback functions
void charKeyDown( unsigned char key, int x, int y );
void charKeyUp( unsigned char key, int x, int y );
void funcKeyDown( int key, int x, int y );
void funcKeyUp( int key, int x, int y );

//-------- light.cpp
void MainLight( int light_id, float r, float g, float b );
void SubLight( int light_id, float r, float g, float b);
void HeadLight( int light_id, float r, float g, float b, float att );
void Fog( int fog_mode, float r, float g, float b, float d, float start, float end );

WindowDataT window;
extern SimDataT simdata;

int time;

inline float _blend( float k, float a, float b )
{ return ( k * a + ( 1.0 - k ) * b ); }

/*------------------------------------------------------------------- update
 * update - GLUT idle callback function
 *--------*/
void update( void )
{
	time = glutGet( GLUT_ELAPSED_TIME );

    UpdateScene();

	//-------- ready to redraw
	glutPostRedisplay();

	//-------- sync
	//Sleep( 0 ); // NVIDIA Quadro
	//Sleep( 16 ) // for cheap video chips @ ~60Hz

	return;
}

/*------------------------------------------------------------------- display
 * display - GLUT display callback function
 *--------*/
void display( void )
{
    glClearColor(
		_blend( simdata.sky_color[3], simdata.sky_color[0], simdata.air_color[0] ),
		_blend( simdata.sky_color[3], simdata.sky_color[1], simdata.air_color[1] ),
		_blend( simdata.sky_color[3], simdata.sky_color[2], simdata.air_color[2] ),
		1.0 );

	glEnable( GL_NORMALIZE ); //// 20161109追加

    //-------- viewport --------
	glViewport( 0, 0, window.width, window.height );

    //-------- clear buffers --------
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//-------- projection transformation --------
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	Projection();

	//-------- viewing transformation --------
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//▼ヘッドライトON: R, G, B, 照度半減距離[m]
	HeadLight( GL_LIGHT2, 0.25, 0.25, 0.25, simdata.clip_far );

    Viewing();

	//▼メインライトON: R, G, B
	MainLight( GL_LIGHT0, 1.0, 1.0, 1.0 );

	//▼サブライトON: R, G, B
	SubLight( GL_LIGHT1, 0.1, 0.2, 0.9 );

	//▽フォグON: R, G, B, 密度, 開始距離, 終了距離	
	Fog( GL_EXP, 
		simdata.air_color[0],
		simdata.air_color[1],
		simdata.air_color[2], 
		simdata.air_color[3], 
		simdata.clip_near, simdata.clip_far );

    //-------- draw --------
	glEnable( GL_DEPTH_TEST ); // ---- begin: 
   // glEnable( GL_LIGHTING );

    DrawScene();

    //glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

	//-------- swapbuffers --------
    glutSwapBuffers();

    return;
}
/*--------------------------------------------------------------------- reshape
 * reshape - GLUT reshape callback function
 *--------*/
void reshape( int width, int height )
{
	window.width = width;
	window.height = height;
	window.aspect = (float)window.width/window.height;
}
/*---------------------------------------------------------------- initWindow
 * initWindow: initializes OpenGL window
 *--------*/
void initWindow( char *winname )
{
	//////// ユーザカスタマイズ
	window.xo = 100;
	window.yo = 100;
	window.width = 800;
	window.height = 800;
	////////////////

	window.aspect = (float)window.width/window.height; // aspect ratio
	window.title = winname;

	//-------- window properties
	glutInitWindowPosition( window.xo, window.yo );
    glutInitWindowSize( window.width, window.height );

    //-------- config buffers
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	//-------- open window
	glutCreateWindow( window.title );

	//-------- GL mode
	glEnable( GL_NORMALIZE );
	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, true );

    return;
}
/*---------------------------------------------------------------------- main
 * main
 *--------*/
int main( int argc, char *argv[] )
{
    glutInit( &argc, argv );

    initWindow( argv[0] );

	//-------- basic callbacks
    glutDisplayFunc( display );  // display callback function
    glutIdleFunc( update );    // idle callback function
	glutReshapeFunc( reshape ); // reshape callback function

	//-------- keyboard callbacks
    glutKeyboardFunc( charKeyDown ); // character key down callback
	glutKeyboardUpFunc( charKeyUp ); // character key up callback
	glutSpecialFunc( funcKeyDown ); // function key down callback
	glutSpecialUpFunc( funcKeyUp ); // function key up callback
	glutIgnoreKeyRepeat( 1 );   // disable key-repeat

	//-------- mouse callbacks
    glutMouseFunc( mouseClick );       // mouse click callback
	glutPassiveMotionFunc( mouseMotion ); // passive motion callback
	glutMotionFunc( mouseDrag ); // mouse drag callback	
	
    printf( "[H]:Help\n" );     // indicate help instruction

    InitScene();

	time = glutGet( GLUT_ELAPSED_TIME );

    glutMainLoop(); // run main loop

    return 0;
}
/******************************************************************************
 * end of source code */
