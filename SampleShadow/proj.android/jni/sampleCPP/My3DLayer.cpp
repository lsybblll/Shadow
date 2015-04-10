#include "My3DLayer.h"
#include <cmath>
#include "cocos2d.h"

using namespace cocos2d;

//ʵ��My3DLayer���е�init��������ʼ������
bool My3DLayer::init()
{
	//���ø���ĳ�ʼ��
    if ( !Layer::init() )
    {
        return false;
    }
    //�������Objģ��-----------------------------------------------------
    sp3Floor=Sprite3D::create("obj/floor.obj", "pic/floor.png");//�����ذ�
    sp3Floor->setScale(1.8f);									//���õذ��С
    sp3Floor->setPosition3D(Vec3(0,0,0));						//���õذ�λ��
	this->addChild(sp3Floor);									//��ӵذ嵽������
	this->initFounding();										//��ʼ����̨
	this->initHero();											//��ʼ���˶�Ա
	this->initObject();
	this->setCameraMask((unsigned short)CameraFlag::USER1);		//���ô˲�������Ⱦ�������
	auto listenerTouch = EventListenerTouchOneByOne::create();			//����һ�����㴥�صļ���
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
	//����Heroc3b��������ģ��==================================================
	spC3bHero = EffectSprite3D::create("c3b/ren.c3b");
	spC3bHero->setPosition3D(Vec3(0,0,200));
	spC3bHero->setScale(10.0f);
	spC3bHero->setGlobalZOrder(1.0f);
	this->addChild(spC3bHero);
	//*******************�����Ч*********************
	Effect3DShadow *e3s=Effect3DShadow::create();	//����Ӱ����Ч
	//���������Ӱ��
	spC3bHero->addEffect(e3s,-1);
	//���û���˳��Ӧ�ȵ���󣬱�����С
	e3s->setGlobalZOrder(0.5f);
	//���ù�Դλ��
	e3s->setLightPosition(Vec3(200,500,0));
	//����
	EffectSprite3D* spC3bLFuZi=EffectSprite3D::create("obj/bat.obj","pic/bat.png");
	spC3bLFuZi->setRotation3D(Vec3(0,60,0));
	spC3bLFuZi->setGlobalZOrder(1.0f);
	//��ȡ��Ҫ������Ʒ�Ĺ����ڵ㣬Bip01 L Finger0�ǹ�������
	AttachNode* anRFinger = spC3bHero->getAttachNode("Bip01 R Finger0");
	//����Ʒ���ŵ���Ӧ�Ĺ����ڵ�
	anRFinger->addChild(spC3bLFuZi);
	//��������Ʒ����Ӱ��
	Effect3DShadow *e3ss=Effect3DShadow::create();
	spC3bLFuZi->addEffect(e3ss,-1);
	e3ss->setLightPosition(Vec3(200,500,0));

	//���������Ź�������
	animationHero = Animation3D::create("c3b/ren.c3b");
	if (animationHero)
	{
		animateK = Animate3D::create(animationHero);			//Ӣ�۸��ֶ���
		animateK->setSpeed(animateK->getSpeed()*0.9f);			//���������ٶ�
		spC3bHero->runAction(RepeatForever::create(animateK));		//ִ�ж���
	}
}
//������̨
void My3DLayer::initFounding()
{
	//����С����c3b��������ģ��==================================================
	spObjFounding = EffectSprite3D::create("obj/rchair.obj","pic/rchair.png");
	spObjFounding->setPosition3D(Vec3(300,0,0));
	spObjFounding->setScale(5.0f);
	spObjFounding->setGlobalZOrder(1.0f);
	this->addChild(spObjFounding);

	//����������Ӱ��
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

	//���������Ź�������
	Animation3D *animationObj = Animation3D::create("c3b/knight.c3b");
	if (animationObj)
	{
		Animate3D *animate = Animate3D::create(animationObj);			//Ӣ�۸��ֶ���
		animate->setSpeed(animate->getSpeed()*0.9f);			//���������ٶ�
		spObj->runAction(RepeatForever::create(animate));		//ִ�ж���
	}

}
//��ʼ�����¼��Ĵ�����
bool My3DLayer::onMyTouchBegan(Touch *touch, Event *event)
{
	lxs=touch->getLocation().x;
	return true;
}
//�����ƶ��¼��Ĵ�����
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
		//���㵱ǰ�۲�Ƕ����������λ��
		float cx=sin(degree*3.1415926535898/180)*420;
		float cz=cos(degree*3.1415926535898/180)*420;
		//�������������
		camera->setPosition3D(Vec3(cx,650,cz));
		camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));
	}else if(lxm-lxs>5){
		degree-=4.0f;
		if(degree<=-360)
		{
			degree=degree+360;
		}
		//���㵱ǰ�۲�Ƕ����������λ��
		float cx=sin(degree*3.1415926535898/180)*420;
		float cz=cos(degree*3.1415926535898/180)*420;
		//�������������
		camera->setPosition3D(Vec3(cx,650,cz));
		camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));
	}
	lxs=touch->getLocation().x;
}
//���ؽ����¼��Ĵ�����
void My3DLayer::onMyTouchEnded(Touch *touch, Event *event)
{
}
//������ֹ�¼��Ĵ�����
void My3DLayer::onMyTouchCancelled(Touch *touch, Event *event)
{
	onMyTouchCancelled(touch, event);
}
