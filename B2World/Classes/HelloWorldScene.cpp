#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    world = new b2World(b2Vec2(0, -10));
    //设置监听碰撞的listener
    world ->SetContactListener(this);
    
    addRect();
    addGround();
    scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt){
    world ->Step(dt, 8, 3);
    Sprite *s;
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
//        if (b->GetType() == b2_dynamicBody) {
            if (b->GetUserData()) {
                s = (Sprite*)b->GetUserData();
                s ->setPosition(b->GetPosition().x*RATIO, b->GetPosition().y*RATIO);
            }
//        }
    }
}


void HelloWorld::addRect(){
    
    //config box2d
    //构建body类型，位置
    b2BodyDef def;
    def.position = b2Vec2(3, 1.5);
    
    //动态物体
    def.type = b2_dynamicBody;
    def.linearVelocity = b2Vec2(0, 10);
    
/*  漂浮物体
    def.type = b2_kinematicBody;
    def.linearVelocity = b2Vec2(1, 0);
*/
   
    
    //根据类型创建body
    b2Body *body = world ->CreateBody(&def);
    
    //定义body形状
    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(0.5, 0.5);
    
    //定义body属性
    b2FixtureDef bodyFixtureDef;
    bodyFixtureDef.density = 1;
    bodyFixtureDef.friction = 0.3;
    bodyFixtureDef.shape =&bodyShape;
    //给body添加属性
    body ->CreateFixture(&bodyFixtureDef);
    

    //config shape
    auto s = Sprite::create();
    s->setTextureRect(Rect(0, 0, 0.5*2*RATIO, 0.5*2*RATIO));
    addChild(s);
    
    body ->SetUserData(s);
    
}

void HelloWorld::addGround(){
    //定义body类型，位置
    b2BodyDef def;
    def.type = b2_staticBody;
    def.position = b2Vec2(0, 0);
    //根据类型创建body
    groundBody = world ->CreateBody(&def);
    
    //定义body形状
    b2PolygonShape groundShape;
    groundShape.SetAsBox(400/RATIO, 0.5);
    
    //定义body属性
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.3;
    fixtureDef.shape =&groundShape;
    //给body添加属性
    groundBody ->CreateFixture(&fixtureDef);
    
}


void HelloWorld::BeginContact(b2Contact *contact){
    if (contact ->GetFixtureA()->GetBody() == groundBody||contact->GetFixtureB()->GetBody() ==groundBody) {
        log("something hit the floor");
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
