//
//  GameOverScene.cpp
//  CC2DTutor
//
//  Created by Ace on 12/27/12.
//
//

#include "GameOverScene.h"
#include "GameScene.h"

bool GameOverScene::init()
{
    if (!CCScene::init())
        return false;
    
    GameOverLayer *layer = GameOverLayer::create();
    this->addChild(layer);
    
    return true;
}

bool GameOverLayer::init()
{
    if( !CCLayer::init() )
        return false;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // add game over text in the middle of the screen with red color
    CCLabelTTF *gameOverLabel = CCLabelTTF::create("GAME OVER", "Thonburi", 50);
    gameOverLabel->setColor( ccc3(255, 0, 0) );
    gameOverLabel->setPosition( ccp(winSize.width / 2, winSize.height / 2 + 50) );
    this->addChild(gameOverLabel);
    
    // add instruction below the game over text
    CCLabelTTF *infoLabel = CCLabelTTF::create("touch to continue", "Thonburi", 20);
    infoLabel->setColor( ccc3(255, 255, 255) );
    infoLabel->setPosition( ccp(winSize.width / 2, winSize.height / 2 - 20) );
    this->addChild(infoLabel);
    
    this->setTouchEnabled(true);
    
    return true;
}

void GameOverLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCScene *gameScene = GameScene::create();
    CCDirector::sharedDirector()->replaceScene(gameScene);
}