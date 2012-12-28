//
//  GameOverScene.h
//  CC2DTutor
//
//  Created by Ace on 12/27/12.
//
//

#ifndef __CC2DTutor__GameOverScene__
#define __CC2DTutor__GameOverScene__

#include "cocos2d.h"

using namespace cocos2d;

class GameOverScene : public CCScene
{
public:
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameOverScene);
    
    virtual bool init();
};

class GameOverLayer : public CCLayer
{
public:
    CREATE_FUNC(GameOverLayer);
    
    virtual bool init();
    
private:
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
};

#endif /* defined(__CC2DTutor__GameOverScene__) */
