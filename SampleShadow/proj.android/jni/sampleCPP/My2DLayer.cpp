#include "My2DLayer.h"

//USING_NS_CC;
using namespace cocos2d;

//实现My2DLayer类中的init方法，初始化布景
bool My2DLayer::init()
{
	//调用父类的初始化
    if ( !Layer::init() )
    {
        return false;
    }
    
    //获取可见区域尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //获取可见区域原点坐标
    Point origin = Director::getInstance()->getVisibleOrigin();

    //创建关闭菜单项
    auto closeItem = MenuItemImage::create
    (
		   "pic/CloseNormal.png",	//平时的图片
		   "pic/CloseSelected.png",	//选中时的图片
		   CC_CALLBACK_1(My2DLayer::menuCloseCallback, this) //点击时执行的回调方法
	);
    
    //设置关闭菜单项的位置
	closeItem->setPosition
	(
		Point
		(
			origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,	//X坐标
            origin.y + closeItem->getContentSize().height/2							//Y坐标
        )
    );

	//创建菜单对象
    auto menu = Menu::create(closeItem,(char*)NULL);
    //设置菜单位置
    menu->setPosition(Point::ZERO);
    //将菜单添加到布景中
    this->addChild(menu, 1);
    //创建一个包含"3D模型加载演示"内容的文本标签
    auto label = LabelTTF::create("3D\u7279\u6548\u5c55\u793a\uff1a\u5f71\u5b50", "fonts/FZKATJW.ttf", 40);
    //设置标签字体的颜色
    label->setFontFillColor(Color3B::YELLOW);
    //设置文本标签的位置
    label->setPosition
    (
    	Point
    	(
    		origin.x + visibleSize.width/2,
    		origin.y + visibleSize.height - label->getContentSize().height
        )
    );
    //将文本标签添加到布景中
    this->addChild(label, 1);

    return true;
}

//关闭菜单回调方法
void My2DLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}
