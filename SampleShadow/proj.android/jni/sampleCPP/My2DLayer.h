#ifndef __My2DLayer_H__
#define __My2DLayer_H__

#include "cocos2d.h"
using namespace cocos2d;

class My2DLayer : public cocos2d::Layer
{
public:
	//��ʼ���ķ���
	virtual bool init();
	//�رղ˵��ص�����
    void menuCloseCallback(Ref* pSender);
    //Cocos2d-x�ж����һ���꣬����ָ�����͵Ķ���
    //��������Ӧ�ĳ�ʼ�����ͷŹ���
    CREATE_FUNC(My2DLayer);
};

#endif
