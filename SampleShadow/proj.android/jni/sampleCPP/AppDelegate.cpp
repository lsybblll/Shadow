#include "AppDelegate.h"
#include "C3bSceneManager.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

//初始化方法
bool AppDelegate::applicationDidFinishLaunching()
{
    //获取导演
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    //若不存在glview则重新创建
    if(!glview)
    {
    	glview = GLViewImpl::create("My Game");
    }
    //设置绘制用GLView
    director->setOpenGLView(glview);
    //设置开启深度检测
    director->setDepthTest(true);
    //设置目标分辨率,别的分辨率的屏幕将自动上下或左右留白进行多分辨率自适应
    glview->setDesignResolutionSize(960,540,ResolutionPolicy::SHOW_ALL);
	//设置为不显示FPS等信息
    director->setDisplayStats(false);
    //系统模拟时间间隔
    director->setAnimationInterval(1.0 / 60);
    //创建欢迎场景
    auto scene = C3bSceneManager::createScene();
    //跌换到欢迎场景显示
    director->runWithScene(scene);
    return true;
}

//当程序进入后台时调用此方法
void AppDelegate::applicationDidEnterBackground()
{
	//停止动画
    Director::getInstance()->stopAnimation();
    //如果有声音的话要调用下面一句暂停声音播放
    //SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

//当程序进入前台时调用
void AppDelegate::applicationWillEnterForeground()
{
	//开始动画
    Director::getInstance()->startAnimation();
    //如果有声音的话要调用下面一句开始声音播放
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
