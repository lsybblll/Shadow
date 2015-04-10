#include "C3bSceneManager.h"
#include "My3DLayer.h"
#include "My2DLayer.h"

//USING_NS_CC;
using namespace cocos2d;

//实现C3bSceneManager类中的createScene方法
Scene* C3bSceneManager::createScene()
{
    //创建一个场景对象
    auto scene = Scene::create();

    //获取可见区域尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //创建摄像机
    Camera* camera = Camera::createPerspective
    (
    	55, 										//摄像机视角 40-60之间是合理值
    	visibleSize.width/visibleSize.height, 		//视口长宽比
    	1, 											//near
    	2000										//far
    );
    camera->setCameraFlag(CameraFlag::USER1);		//设置摄像机编号标志
    camera->setPosition3D(Vec3(0,650,420));			//设置摄像机位置
    camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));		//设置摄像机目标点以及up向量
    scene->addChild(camera);						//将摄像机添加到场景中

    //创建一个3D布景对象
    My3DLayer* layer3D = My3DLayer::create();
    layer3D->camera=camera;
    //将3D布景添加到场景中
    scene->addChild(layer3D);

    //创建一个2D布景对象
    My2DLayer* layer2D = My2DLayer::create();
    //将2D布景添加到场景中
    scene->addChild(layer2D);

    //返回场景
    return scene;
}
