#include "My2DLayer.h"

//USING_NS_CC;
using namespace cocos2d;

//ʵ��My2DLayer���е�init��������ʼ������
bool My2DLayer::init()
{
	//���ø���ĳ�ʼ��
    if ( !Layer::init() )
    {
        return false;
    }
    
    //��ȡ�ɼ�����ߴ�
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //��ȡ�ɼ�����ԭ������
    Point origin = Director::getInstance()->getVisibleOrigin();

    //�����رղ˵���
    auto closeItem = MenuItemImage::create
    (
		   "pic/CloseNormal.png",	//ƽʱ��ͼƬ
		   "pic/CloseSelected.png",	//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_1(My2DLayer::menuCloseCallback, this) //���ʱִ�еĻص�����
	);
    
    //���ùرղ˵����λ��
	closeItem->setPosition
	(
		Point
		(
			origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,	//X����
            origin.y + closeItem->getContentSize().height/2							//Y����
        )
    );

	//�����˵�����
    auto menu = Menu::create(closeItem,(char*)NULL);
    //���ò˵�λ��
    menu->setPosition(Point::ZERO);
    //���˵���ӵ�������
    this->addChild(menu, 1);
    //����һ������"3Dģ�ͼ�����ʾ"���ݵ��ı���ǩ
    auto label = LabelTTF::create("3D\u7279\u6548\u5c55\u793a\uff1a\u5f71\u5b50", "fonts/FZKATJW.ttf", 40);
    //���ñ�ǩ�������ɫ
    label->setFontFillColor(Color3B::YELLOW);
    //�����ı���ǩ��λ��
    label->setPosition
    (
    	Point
    	(
    		origin.x + visibleSize.width/2,
    		origin.y + visibleSize.height - label->getContentSize().height
        )
    );
    //���ı���ǩ��ӵ�������
    this->addChild(label, 1);

    return true;
}

//�رղ˵��ص�����
void My2DLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}
