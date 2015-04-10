#include "My3DLayer.h"
#include <cmath>
#include "cocos2d.h"

using namespace cocos2d;

//实现My3DLayer类中的init方法，初始化布景
bool My3DLayer::init()
{
	//调用父类的初始化
    if ( !Layer::init() )
    {
        return false;
    }
    //加入地面Obj模型-----------------------------------------------------
    sp3Floor=Sprite3D::create("obj/floor.obj", "pic/floor.png");//创建地板
    sp3Floor->setScale(1.8f);									//设置地板大小
    sp3Floor->setPosition3D(Vec3(0,0,0));						//设置地板位置
	this->addChild(sp3Floor);									//添加地板到布景中
	this->initFounding();										//初始化看台
	this->initHero();											//初始化运动员
	this->initObject();
	this->setCameraMask((unsigned short)CameraFlag::USER1);		//设置此布景层渲染用摄像机
	auto listenerTouch = EventListenerTouchOneByOne::create();			//创建一个单点触控的监听
	listenerTouch->setSwallowTouches(true);
	listenerTouch->onTouchBegan = CC_CALLBACK_2(My3DLayer::onMyTouchBegan, this);
	listenerTouch->onTouchMoved = CC_CALLBACK_2(My3DLayer::onMyTouchMoved, this);
	listenerTouch->onTouchEnded = CC_CALLBACK_2(My3DLayer::onMyTouchEnded, this);
	listenerTouch->onTouchCancelled = CC_CALLBACK_2(My3DLayer::onMyTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouch,this);
    return true;
}
void My3DLayer::initHero()
{
	//加载Heroc3b骨骼动画模型==================================================
	spC3bHero = EffectSprite3D::create("c3b/ren.c3b");
	spC3bHero->setPosition3D(Vec3(0,0,200));
	spC3bHero->setScale(10.0f);
	spC3bHero->setGlobalZOrder(1.0f);
	this->addChild(spC3bHero);
	//*******************添加特效*********************
	Effect3DShadow *e3s=Effect3DShadow::create();	//创建影子特效
	//给物体添加影子
	spC3bHero->addEffect(e3s,-1);
	//设置绘制顺序，应比地面大，比物体小
	e3s->setGlobalZOrder(0.5f);
	//设置光源位置
	e3s->setLightPosition(Vec3(200,500,0));
	//球拍
	EffectSprite3D* spC3bLFuZi=EffectSprite3D::create("obj/bat.obj","pic/bat.png");
	spC3bLFuZi->setRotation3D(Vec3(0,60,0));
	spC3bLFuZi->setGlobalZOrder(1.0f);
	//获取需要附着物品的骨骼节点，Bip01 L Finger0是骨骼名称
	AttachNode* anRFinger = spC3bHero->getAttachNode("Bip01 R Finger0");
	//将物品附着到对应的骨骼节点
	anRFinger->addChild(spC3bLFuZi);
	//给附着物品设置影子
	Effect3DShadow *e3ss=Effect3DShadow::create();
	spC3bLFuZi->addEffect(e3ss,-1);
	e3ss->setLightPosition(Vec3(200,500,0));

	//创建并播放骨骼动画
	animationHero = Animation3D::create("c3b/ren.c3b");
	if (animationHero)
	{
		animateK = Animate3D::create(animationHero);			//英雄各种动画
		animateK->setSpeed(animateK->getSpeed()*0.9f);			//骨骼动画速度
		spC3bHero->runAction(RepeatForever::create(animateK));		//执行动作
	}
}
//创建看台
void My3DLayer::initFounding()
{
	//加载小怪兽c3b骨骼动画模型==================================================
	spObjFounding = EffectSprite3D::create("obj/rchair.obj","pic/rchair.png");
	spObjFounding->setPosition3D(Vec3(300,0,0));
	spObjFounding->setScale(5.0f);
	spObjFounding->setGlobalZOrder(1.0f);
	this->addChild(spObjFounding);

	//给物体设置影子
	Effect3DShadow *e3s=Effect3DShadow::create();
	e3s->setLightPosition(Vec3(400,500,0));
//	e3s->setGlobalZOrder(0.5f);
	spObjFounding->addEffect(e3s,-1,0.5);

}
void My3DLayer::initObject()
{
	spObj = EffectSprite3D::create("c3b/knight.c3b");
	spObj->setPosition3D(Vec3(0,0,-200));
	spObj->setScale(30.0f);
	spObj->setGlobalZOrder(1.0f);
	this->addChild(spObj);

	Effect3DShadow *e3ss=Effect3DShadow::create();
	spObj->addEffect(e3ss,-1);
	e3ss->setLightPosition(Vec3(200,500,0));

	//创建并播放骨骼动画
	Animation3D *animationObj = Animation3D::create("c3b/knight.c3b");
	if (animationObj)
	{
		Animate3D *animate = Animate3D::create(animationObj);			//英雄各种动画
		animate->setSpeed(animate->getSpeed()*0.9f);			//骨骼动画速度
		spObj->runAction(RepeatForever::create(animate));		//执行动作
	}

}
//开始触控事件的处理方法
bool My3DLayer::onMyTouchBegan(Touch *touch, Event *event)
{
	lxs=touch->getLocation().x;
	return true;
}
//触控移动事件的处理方法
void My3DLayer::onMyTouchMoved(Touch *touch, Event *event)
{
	lxm=touch->getLocation().x;
	if(lxm-lxs<-5)
	{
		degree+=4.0f;
		if(degree>=360)
		{
			degree=degree-360;
		}
		//计算当前观察角度下摄像机的位置
		float cx=sin(degree*3.1415926535898/180)*420;
		float cz=cos(degree*3.1415926535898/180)*420;
		//更新摄像机参数
		camera->setPosition3D(Vec3(cx,650,cz));
		camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));
	}else if(lxm-lxs>5){
		degree-=4.0f;
		if(degree<=-360)
		{
			degree=degree+360;
		}
		//计算当前观察角度下摄像机的位置
		float cx=sin(degree*3.1415926535898/180)*420;
		float cz=cos(degree*3.1415926535898/180)*420;
		//更新摄像机参数
		camera->setPosition3D(Vec3(cx,650,cz));
		camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));
	}
	lxs=touch->getLocation().x;
}
//触控结束事件的处理方法
void My3DLayer::onMyTouchEnded(Touch *touch, Event *event)
{
}
//触控终止事件的处理方法
void My3DLayer::onMyTouchCancelled(Touch *touch, Event *event)
{
	onMyTouchCancelled(touch, event);
}
