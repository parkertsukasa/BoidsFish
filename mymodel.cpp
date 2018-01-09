#include "platform.h"
#include <gl/glut.h>
#include "GLMetaseq.h"

#include "platform.h"

#include "sim.h"
#include "mymodel.h"

static MQO_MODEL fishmodel;

/////////////////////////// 共通関数
/*----------------------------------------------------- CreateMyModels
 * CreateMyModels
 */
void CreateMyModels()
{
    fishmodel = mqoCreateModel( "/Users/tsukasa/Desktop/Lab/iCG/BoidsFish/FishModel/fish.mqo", 1.0 );
}

/*----------------------------------------------------- DeleteMyModels
 * DeleteMyModels
 */
void DeleteMyModels()
{
    mqoDeleteModel( fishmodel );
}
/////////////////////////// 個別の関数
/*------------------------------------------------------ DrawMymodel1
 */
void DrawMyFishModel()
{
    glPushMatrix();
    glRotatef( 0.0, 1.0, 0.0, 0.0 );//モデルの座標軸調整など
    mqoCallModel( fishmodel );
    glPopMatrix();
}

//----- end of file -----
