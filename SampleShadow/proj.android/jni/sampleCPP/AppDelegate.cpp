#include "AppDelegate.h"
#include "C3bSceneManager.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

//��ʼ������
bool AppDelegate::applicationDidFinishLaunching()
{
    //��ȡ����
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    //��������glview�����´���
    if(!glview)
    {
    	glview = GLViewImpl::create("My Game");
    }
    //���û�����GLView
    director->setOpenGLView(glview);
    //���ÿ�����ȼ��
    director->setDepthTest(true);
    //����Ŀ��ֱ���,��ķֱ��ʵ���Ļ���Զ����»��������׽��ж�ֱ�������Ӧ
    glview->setDesignResolutionSize(960,540,ResolutionPolicy::SHOW_ALL);
	//����Ϊ����ʾFPS����Ϣ
    director->setDisplayStats(false);
    //ϵͳģ��ʱ����
    director->setAnimationInterval(1.0 / 60);
    //������ӭ����
    auto scene = C3bSceneManager::createScene();
    //��������ӭ������ʾ
    director->runWithScene(scene);
    return true;
}

//����������̨ʱ���ô˷���
void AppDelegate::applicationDidEnterBackground()
{
	//ֹͣ����
    Director::getInstance()->stopAnimation();
    //����������Ļ�Ҫ��������һ����ͣ��������
    //SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

//���������ǰ̨ʱ����
void AppDelegate::applicationWillEnterForeground()
{
	//��ʼ����
    Director::getInstance()->startAnimation();
    //����������Ļ�Ҫ��������һ�俪ʼ��������
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
