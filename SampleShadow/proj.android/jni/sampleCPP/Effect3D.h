#ifndef __Effect3D_H__
#define __Effect3D_H__

#include "cocos2d.h"
USING_NS_CC;

class EffectSprite3D;	//声明类

//3D效果基类，声明了需要重写的方法以及相关属性
class Effect3D : public Ref
{
public:
	//3D特效的绘制方法，需要调用OpenGL方法进行重写
	virtual void draw(const Mat4 &transform) = 0;
	//3D着色器初始化操作，需要重写
	virtual void setTarget(EffectSprite3D *sprite) = 0;
	//3D着色器绑定
	void bindShader(EffectSprite3D *sprite);
	//设置绘制顺序
	void setGlobalZOrder(float globalOrder);
	//获取绘制顺序
	float getGlobalZOrder() {return _globalOrder;}
protected:
	//构造、析构函数
	Effect3D():_glProgramState(nullptr), _globalOrder(0), _sprite(nullptr){}
	virtual ~Effect3D()
	{
		//安全释放着色器
		CC_SAFE_RELEASE(_glProgramState);
	}
	//绘制顺序
	float _globalOrder;
	//3D特效着色器
	GLProgramState* _glProgramState;
	//实现3D特效的精灵的引用
	EffectSprite3D* _sprite;
};

//3D特效精灵，可以添加3D效果
class EffectSprite3D : public Sprite3D
{
public:
	//3D特效精灵创建方法
	static EffectSprite3D* create(const std::string& model, const std::string& texture);
	static EffectSprite3D* create(const std::string& model);
	//添加3D特效（不覆盖本身效果）
	void addEffect(Effect3D* effect, ssize_t order, float globalZOrder = 0);
	//重写CCNode.h方法，用于将绘制方法绑定
	virtual void draw(Renderer *renderer, const Mat4 &transform,uint32_t flags) override;
protected:
	//构造、析构方法
	EffectSprite3D():_defaultEffect(nullptr){}
	virtual ~EffectSprite3D();
	//3D特效集合vector，保存所有特效，并按元数据第一个参数排序
	std::vector<std::tuple<ssize_t,Effect3D*,CustomCommand>> _effects;
	//本身效果
	Effect3D* _defaultEffect;
	//自定义命令，用于绑定自己重写的draw方法
	CustomCommand _command;
};
//3D特效——影子
class Effect3DShadow: public Effect3D
{
public:
	//创建方法
	static Effect3DShadow* create();
	//设置产生影子的光源的位置
	void setLightPosition(const Vec3& position);
	//实现基类的绘制及设置着色器的方法
    virtual void draw(const Mat4 &transform) override;
    virtual void setTarget(EffectSprite3D *sprite) override;
protected:
    //构造、析构函数
    Effect3DShadow();
    virtual ~Effect3DShadow();
    //初始化函数，可以为空
    bool init();
    //光源位置
    Vec3 _lightPosition;
protected:
	//响应EVENT_RENDERER_RECREATED进行重新初始化（在安卓、WP8、WINDOWS_RT下起效）
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		EventListenerCustom* _backToForegroundListener;
	#endif
protected:
    //记录着色器路径，无骨骼
    static const std::string _vertShaderFile;
    static const std::string _fragShaderFile;
    static const std::string _keyInGLProgramCache;
    //有骨骼
    static const std::string _vertSkinnedShaderFile;
    static const std::string _fragSkinnedShaderFile;
    static const std::string _keySkinnedInGLProgramCache;
};
#endif
