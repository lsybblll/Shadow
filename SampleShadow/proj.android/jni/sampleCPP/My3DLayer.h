#ifndef __My3DLayer_H__
#define __My3DLayer_H__

#include "cocos2d.h"
#include "Effect3D.h"

using namespace cocos2d;

class My3DLayer : public cocos2d::Layer
{
public :
	//�����
	Camera* camera;
	Sprite3D* sp3Floor;//����obj

	Animation3D* animationGuaiShou;//���޹�������
	Animate3D* animateKan;//���޹�����������
	Animation3D* animationHero;//Ӣ�۹�������
	Animate3D* animateK;//Ӣ�۹�����������

	EffectSprite3D* spObjFounding;//��̨
	EffectSprite3D* spC3bHero;//Ӣ��c3b
	EffectSprite3D* spObj;//Ӣ��c3b


	//�����ת���Ƕ�
	float degree=0;

	float lxs;//����x
	float lxm;//�ƶ�ʱ��x

	float startAngle=0;
	//��ʼ���ķ���
	virtual bool init();
	void initFounding();
	void initHero();
	void initObject();

    bool onMyTouchBegan(Touch *touch, Event *event);		//���ؿ�ʼ�¼��Ļص�����
	void onMyTouchMoved(Touch *touch, Event *event);		//���ص��ƶ��¼��Ļص�����
	void onMyTouchEnded(Touch *touch, Event *event);		//���ؽ����¼��Ļص�����
	void onMyTouchCancelled(Touch *touch, Event *event);	//������ֹ�¼��Ļص�����

    //Cocos2d-x�ж����һ���꣬����ָ�����͵Ķ���
    //��������Ӧ�ĳ�ʼ�����ͷŹ���
    CREATE_FUNC(My3DLayer);
};

#endif
