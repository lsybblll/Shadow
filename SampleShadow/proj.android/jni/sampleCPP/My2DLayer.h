#ifndef __My2DLayer_H__
#define __My2DLayer_H__

#include "cocos2d.h"
using namespace cocos2d;

class My2DLayer : public cocos2d::Layer
{
public:
	//初始化的方法
	virtual bool init();
	//关闭菜单回调方法
    void menuCloseCallback(Ref* pSender);
    //Cocos2d-x中定义的一个宏，创建指定类型的对象
    //并做好相应的初始化与释放工作
    CREATE_FUNC(My2DLayer);
};

#endif
