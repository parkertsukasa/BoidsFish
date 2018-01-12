#include "platform.h"
#include "GLMetaseq.h"

#include "platform.h"

#include "sim.h"
#include "mymodel.h"

extern FishDataT fish[];

static MQO_MODEL fishmodel;
static MQO_MODEL fishhead;
static MQO_MODEL fishbody;
static MQO_MODEL fishtail;

/////////////////////////// 共通関数
/*----------------------------------------------------- CreateMyModels
 * CreateMyModels
 */
void CreateMyModels()
{
    fishmodel = mqoCreateModel( "FishModel/fish.mqo", 2.0 );
    fishhead = mqoCreateModel( "FishModel/Kumanomi_Head.mqo", 0.02 );
    fishbody = mqoCreateModel( "FishModel/Kumanomi_Body.mqo", 0.02 );
    fishtail = mqoCreateModel( "FishModel/Kumanomi_Tail.mqo", 0.02 );
}

/*----------------------------------------------------- DeleteMyModels
 * DeleteMyModels
 */
void DeleteMyModels()
{
    mqoDeleteModel( fishmodel );
    mqoDeleteModel( fishhead );
    mqoDeleteModel( fishbody );
    mqoDeleteModel( fishtail );
}
/////////////////////////// 個別の関数
/*------------------------------------------------------ DrawMymodel1
 */
void DrawMyFishModel()
{
    glPushMatrix();
    glRotatef( -90.0, 1.0, 0.0, 0.0 );//モデルの座標軸調整など
    mqoCallModel( fishmodel );
    glPopMatrix();
}

/*------------------------------------------------------ DrawHead
 */
void DrawHead()
{
    glPushMatrix();
    glRotatef ( -90.0, 0.0, 0.0, 90.0 );//モデルの座標軸調整など
    mqoCallModel( fishhead );
    glPopMatrix();
}

/*------------------------------------------------------ DrawBody
 */
void DrawBody()
{
    glPushMatrix();
    glRotatef ( -90.0, 0.0, 0.0, 1.0 );//モデルの座標軸調整など
    mqoCallModel( fishbody );
    glPopMatrix();
}

/*------------------------------------------------------ DrawTail
 */
void DrawTail()
{
    glPushMatrix();
    glRotatef ( -90.0, 0.0, 0.0, 1.0 );//モデルの座標軸調整など
    mqoCallModel( fishtail );
    glPopMatrix();
}


/*------------------------------------------------------ DrawKumanomi
 */
void DrawKumanomi(int i)
{
      glPushMatrix();
    {
        glTranslatef( 0.0, 0.0, 0.0 );
        glRotatef( fish[i].head, 0.0, 1.0, 0.0 );
        glRotatef( 0.0, 1.0, 0.0, 0.0 );
        glRotatef( 0.0, 0.0, 0.0, 1.0 );
		    glScalef(1.0, 1.0, 1.0);
        DrawHead();

        glPushMatrix();
        {
            glTranslatef( 0.0, 0.0, 1.0 );
            glRotatef( fish[i].body, 0.0, 1.0, 0.0 );
            glRotatef( 0.0, 1.0, 0.0, 0.0 );
            glRotatef( 0.01, 0.0, 0.0, 1.0 ); 
            DrawBody();

			  	glPushMatrix();
			  	{
	
				  	 glTranslatef( 0.0, 0.0, 2.0 );
				  	 glRotatef( fish[i].tail, 0.0, 1.0, 0.0 );
				  	 glRotatef( 0.0, 1.0, 0.0, 0.0 ); 
				  	 glRotatef( 0.0, 0.0, 0.0, 1.0 );
				     DrawTail();
				  }
			  	glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}




//----- end of file -----
