#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

class GameScene : public CCScene
{
public:    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameScene);
    
};

class GameLayer : public CCLayer
{
public:
    GameLayer();
    virtual ~GameLayer();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameLayer);
    
    virtual bool init();
    
    void spriteMoveFinished(CCNode* sender);
    
    // touch event handler
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    
private:
    void shoot(float elapsed);
    void addAsteroid(float elapsed);
    void updateGame(float elapsed);
    
private:
    CCSprite    *shipSp;
    CCArray     *bullets;
    CCArray     *asteroids;

};

#endif // __GAMESCENE_H__
