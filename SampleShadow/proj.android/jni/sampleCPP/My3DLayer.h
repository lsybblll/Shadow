#ifndef __My3DLayer_H__
#define __My3DLayer_H__

#include "cocos2d.h"
#include "Effect3D.h"

using namespace cocos2d;

class My3DLayer : public cocos2d::Layer
{
public :
	//摄像机
	Camera* camera;
	Sprite3D* sp3Floor;//地面obj

	Animation3D* animationGuaiShou;//怪兽骨骼动画
	Animate3D* animateKan;//怪兽骨骼动画动作
	Animation3D* animationHero;//英雄骨骼动画
	Animate3D* animateK;//英雄骨骼动画动作

	EffectSprite3D* spObjFounding;//看台
	EffectSprite3D* spC3bHero;//英雄c3b
	EffectSprite3D* spObj;//英雄c3b


	//摄像机转场角度
	float degree=0;

	float lxs;//起点的x
	float lxm;//移动时的x

	float startAngle=0;
	//初始化的方法
	virtual bool init();
	void initFounding();
	void initHero();
	void initObject();

    bool onMyTouchBegan(Touch *touch, Event *event);		//触控开始事件的回调方法
	void onMyTouchMoved(Touch *touch, Event *event);		//触控点移动事件的回调方法
	void onMyTouchEnded(Touch *touch, Event *event);		//触控结束事件的回调方法
	void onMyTouchCancelled(Touch *touch, Event *event);	//触控终止事件的回调方法

    //Cocos2d-x中定义的一个宏，创建指定类型的对象
    //并做好相应的初始化与释放工作
    CREATE_FUNC(My3DLayer);
};

#endif
