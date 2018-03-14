/******************************************************************************
 * keybdmouse.cpp: keyboard and mouse callback functions 
 */
#include <stdlib.h>
#include <stdio.h>

#include "platform.h"

#include "common.h"

#include "sim.h"
#include "interaction.h"

extern SimDataT simdata;
extern CamDataT cam;
extern MouseDataT mouse;
extern WindowDataT window;

extern bool set;

//-------- modifier key status
bool isShiftKeyDown( void );
bool isCtrlKeyDown( void );
bool isAltKeyDown( void );

/*-------------------------------------------------- modifier key status
 * Shift, Ctrl, Alt key status
 */
bool isShiftKeyDown( void )
{ return (bool)(glutGetModifiers() & GLUT_ACTIVE_SHIFT); }
bool isCtrlKeyDown( void )
{ return (bool)(glutGetModifiers() & GLUT_ACTIVE_CTRL); }
bool isAltKeyDown( void )
{ return (bool)(glutGetModifiers() & GLUT_ACTIVE_ALT); }

/*------------------------------------------------------------- charKeyDown/Up
 * charKeyDown/Up - GLUT keyboard callback function
 * key: ASCII code
 * x, y: mouse position
 *--------*/
void charKeyDown( unsigned char key, int x, int y )
{
    switch( key ){
      case 'h': // help
        printf( "Instruction\n" );
        printf( "[H]:Help\n" );
        printf( "[Q]:Quit\n" );
        printf( "[S]:ShowParameter\n" );
        printf( "[F1]:SelectRED\n" );
        printf( "[F2]:SelectGREEN\n" );
        printf( "[F3]:SelectBLUE\n" );
        printf( "[1]:SpeedUp\n" );
        printf( "[2]:SpeedDown\n" );
        printf( "[3]:SightAngleUp\n" );
        printf( "[4]:SightAngleDown\n" );
        break;
      case 'q': // quit
        exit(0);
        break;

      case 'c': // camera position change
        if(cam.up < 2)
          cam.up += 1;
        else
          cam.up = 0;

        break;

      case 's': // Fish position Alignment
        ShowParameter();
        break;

      case '1':
        SpeedUp();
        break;

      case '2':
        SpeedDown();
        break;

      case '3':
        SightAngleUp();
        break;

      case '4':
        SightAngleDown();
        break;

      default:
        break;
    }
    return;
}
void charKeyUp( unsigned char key, int x, int y )
{
    switch( key ){
	  default:
		break;
	}
}
/*--------------------------------------------------------------- funcKeyDown/Up
 * funcKeyDown/Up - GLUT special key callback
 * key: GLUT key code
 * x, y: mouse position
 */
void funcKeyDown( int key, int x, int y )
{
	//ファンクションキー
	//[F*]: GLUT_KEY_F*(*は1～12)
	//[F1]: GLUT_KEY_F1
	//[F12]:GLUT_KEY_F12
	//その他の機能キー
    //[PageUp]  : GLUT_KEY_PAGE_UP:
    //[PageDown]: GLUT_KEY_PAGE_DOWN:
    //[Home]    : GLUT_KEY_HOME:
    //[End]     : GLUT_KEY_END:
    //[Insert]  : GLUT_KEY_INSERT:

	//カーソルキー
	switch( key ){
	  case GLUT_KEY_LEFT: //[←]

		break;
  	  case GLUT_KEY_RIGHT://[→]

		break;
	  case GLUT_KEY_UP: //[↑]

		break;
	  case GLUT_KEY_DOWN://[↓]

		break;

    extern Selected interface;

    case GLUT_KEY_F1:
      interface.select = RED;
    break;

    case GLUT_KEY_F2:
      interface.select = GREEN;
    break;

    case GLUT_KEY_F3:
      interface.select = BLUE;
    break;

    case GLUT_KEY_F4:
      interface.select = WHITE;
    break;
	}
}
void funcKeyUp( int key, int x, int y )
{
	switch( key ){
	  case GLUT_KEY_LEFT: //[←]

		break;
  	  case GLUT_KEY_RIGHT://[→]

		break;
	  case GLUT_KEY_UP: //[↑]

		break;
	  case GLUT_KEY_DOWN://[↓]

		break;
	}
}
/*------------------------------------------------------------- mouseClick
 * mouseClick - GLUT mouse callback function
 * button: mouse button
 * state：press or release
 * x, y：mouse position
 *--------*/
void mouseClick( int button , int state, int x, int y )
{
  //----- マウス座標の正規化 -----
  float mouseX = (float)x/window.width * 2.0 -1.0;
  float mouseY = (float)y/window.height * 2.0 -1.0;

    switch( button ){
      case GLUT_LEFT_BUTTON:
        switch( state ){
          case GLUT_DOWN: // press


            break;
          case GLUT_UP: // release

            break;
          default:
            break;
        }
        break;
      case GLUT_MIDDLE_BUTTON:
        switch( state ){
          case GLUT_DOWN: // press

            break;
          case GLUT_UP: // release

            break;
          default:
            break;
        }
        break;
      case GLUT_RIGHT_BUTTON:
        switch( state ){
          case GLUT_DOWN: // press

              GiveFeed (mouseX, mouseY);
            break;
          case GLUT_UP: // release

            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
    return;
}
/*---------------------------------------------------------------- mouseDrag 
 * mouseDrag - mouse drag callback function
 */
void mouseDrag( int x, int y )
{
	////////

 //----- マウス座標の正規化 -----
  float mouseX = (float)x/window.width * 2.0 -1.0;
  float mouseY = (float)y/window.height * 2.0 -1.0;
  mouse.x = mouseX;
  mouse.z = mouseY;

	////////
    return;
}
/*-------------------------------------------------------------- mouseMotion
 * mouseMotion - passive mouse motion callback function
 */
void mouseMotion( int x, int y )
{
	////////

	////////
    return;
}
//end of file
