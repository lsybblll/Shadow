#include "C3bSceneManager.h"
#include "My3DLayer.h"
#include "My2DLayer.h"

//USING_NS_CC;
using namespace cocos2d;

//ʵ��C3bSceneManager���е�createScene����
Scene* C3bSceneManager::createScene()
{
    //����һ����������
    auto scene = Scene::create();

    //��ȡ�ɼ�����ߴ�
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //���������
    Camera* camera = Camera::createPerspective
    (
    	55, 										//������ӽ� 40-60֮���Ǻ���ֵ
    	visibleSize.width/visibleSize.height, 		//�ӿڳ����
    	1, 											//near
    	2000										//far
    );
    camera->setCameraFlag(CameraFlag::USER1);		//�����������ű�־
    camera->setPosition3D(Vec3(0,650,420));			//���������λ��
    camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));		//���������Ŀ����Լ�up����
    scene->addChild(camera);						//���������ӵ�������

    //����һ��3D��������
    My3DLayer* layer3D = My3DLayer::create();
    layer3D->camera=camera;
    //��3D������ӵ�������
    scene->addChild(layer3D);

    //����һ��2D��������
    My2DLayer* layer2D = My2DLayer::create();
    //��2D������ӵ�������
    scene->addChild(layer2D);

    //���س���
    return scene;
}
