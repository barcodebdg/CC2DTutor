#include "GameScene.h"
#include "GameOverScene.h"

bool GameScene::init()
{
    if( !CCScene::init() )
        return false;
    
    GameLayer *gameLayer = GameLayer::create();
    this->addChild(gameLayer);
    
    return true;
}

GameLayer::GameLayer() :
bullets(NULL),
asteroids(NULL)
{
    
}

GameLayer::~GameLayer()
{
    if(bullets)
    {
        bullets->release();
        bullets = NULL;
    }
    
    if(asteroids)
    {
        asteroids->release();
        asteroids = NULL;
    }
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    // super init first
    if ( !CCLayer::init() )
        return false;
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCSprite* bg = CCSprite::create("SpaceBackground1.png");
    bg->setPosition( ccp(size.width / 2, size.height / 2) );
    this->addChild(bg);
    
    shipSp = CCSprite::create("ShipB.png");
    shipSp->setPosition( ccp(100, size.height / 2) );
    this->addChild(shipSp);
 
    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(GameLayer::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    // initialize sprite list
    bullets = new CCArray();
    asteroids = new CCArray();
    
    // player ship would shoot every 0.5s
    this->schedule( schedule_selector(GameLayer::shoot), 0.5f );
    
    // spawn asteroid every second
    this->schedule( schedule_selector(GameLayer::addAsteroid), 1.0f );
    
    // Call game update every game loop
    this->schedule( schedule_selector(GameLayer::updateGame) );
    
    // enable touch on this layer
    this->setTouchEnabled(true);
    
    // play bgm
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("backsound.mp3", true);
    
    return true;
}

void GameLayer::shoot(float elapsed)
{
    // store ship Y position for later
    int yPos = shipSp->getPosition().y;
    
    // create bullet in front of the ship (20px from ship X position)
    CCSprite* b = CCSprite::create("Bullet.png");
    b->setPosition( ccp(shipSp->getPosition().x + 20, yPos) );
    this->addChild(b);
    
    // add this bullet to bullet list
    bullets->addObject(b);
    
    float duration = 1.0f; // bullet speed. smaller is faster
    
    // Create the actions
    
    // this should move the bullets to the right
	CCFiniteTimeAction* actionMove = CCMoveTo::create( duration, ccp(CCDirector::sharedDirector()->getWinSize().width + b->getContentSize().width, yPos) );
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameLayer::spriteMoveFinished));
    b->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );
    b->setTag(1);   // this will help us to remove the bullet from bullet list later on
    
    // pew... pew... pew...
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("laser.mp3");
}

void GameLayer::addAsteroid(float elapsed)
{
    CCSprite *target = CCSprite::create("Asteroid.png");
    
	// Determine where to spawn the target along the Y axis
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float minY = target->getContentSize().height/2;
	float maxY = winSize.height -  target->getContentSize().height/2;
	int rangeY = (int)(maxY - minY);
	// srand( TimGetTicks() );
	int actualY = ( rand() % rangeY ) + (int)minY;
    
	// Create the target slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated
	target->setPosition( ccp(winSize.width + (target->getContentSize().width/2), actualY) );
	this->addChild(target);
    
	// Determine speed of the target
	int minDuration = (int)2.0;
	int maxDuration = (int)4.0;
	int rangeDuration = maxDuration - minDuration;
	// srand( TimGetTicks() );
	int actualDuration = ( rand() % rangeDuration ) + minDuration;
    
	// Create the actions
	CCFiniteTimeAction* actionMove = CCMoveTo::create( actualDuration, ccp(0 - target->getContentSize().width/2, actualY) );
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(GameLayer::spriteMoveFinished));
    target->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );

    // this will rotate the asteroid 5 degree every 0.1s
    CCRotateBy* actionRotate = CCRotateBy::create( 0.1f, 5.0f );
    target->runAction( CCRepeatForever::create(actionRotate) );
    
	// Add to targets array
    target->setTag(2);
    asteroids->addObject(target);
}

void GameLayer::updateGame(float elapsed)
{
    // let's check the collision
    CCArray *bulletsToDelete = new CCArray();
    
    CCObject *bulletObj;
    
    CCARRAY_FOREACH(bullets, bulletObj)
    {
        CCSprite *bulletSp = (CCSprite*) bulletObj;
        CCRect bRect = bulletSp->boundingBox();             // bullet rectangle
        
        CCObject    *asteroidObj;
        bool        hit = false;
        
        // check each bullet with all the available asteroids
        CCARRAY_FOREACH(asteroids, asteroidObj)
        {
            CCSprite *asteroidSp = (CCSprite*) asteroidObj;
            CCRect aRect = asteroidSp->boundingBox();       // asteroid rectangle
            
            // if both bullet and asteroid rectangle intersected
            // then they collide
            if(bRect.intersectsRect(aRect))
            {
                asteroids->removeObject(asteroidSp);
                this->removeChild(asteroidSp);
                hit = true;
                break;
            }
        }
        
        if(hit)
            bulletsToDelete->addObject(bulletSp);       // add to the list for removal
    }
    
    // remove all the colliding bullet
    CCARRAY_FOREACH(bulletsToDelete, bulletObj)
    {
        CCSprite *bulletSp = (CCSprite*) bulletObj;
        bullets->removeObject(bulletSp);
        this->removeChild(bulletSp);
    }
    
    // release all the object in the list
    bulletsToDelete->release();
    
    // game over if our ship is hit by asteroid
    CCObject *asteroidObj;
    CCARRAY_FOREACH(asteroids, asteroidObj)
    {
        CCSprite *asteroidSp = (CCSprite*) asteroidObj;
        
        if(shipSp->boundingBox().intersectsRect(asteroidSp->boundingBox()))
        {
            GameOverScene *gameOver = GameOverScene::create();
            CCDirector::sharedDirector()->pushScene(gameOver);
        }
    }
}

void GameLayer::spriteMoveFinished(CCNode *sender)
{
    CCSprite *sp = (CCSprite*) sender;
    
    // delete it from the list
    if(sp->getTag() == 1)
        bullets->removeObject(sp);
    else if(sp->getTag() == 2)
        asteroids->removeObject(sp);
    
    // remove the sprite from this scene
    this->removeChild(sp, true);
}

void GameLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    // this will get us the delta distance from previouse touch position
    CCPoint delta = pTouch->getDelta();
    
    // move the ship
    CCPoint shipPos = shipSp->getPosition();
    shipSp->setPosition( ccp(shipPos.x + delta.x, shipPos.y + delta.y) );
}

void GameLayer::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
