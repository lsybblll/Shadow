#ifndef __Effect3D_H__
#define __Effect3D_H__

#include "cocos2d.h"
USING_NS_CC;

class EffectSprite3D;	//������

//3DЧ�����࣬��������Ҫ��д�ķ����Լ��������
class Effect3D : public Ref
{
public:
	//3D��Ч�Ļ��Ʒ�������Ҫ����OpenGL����������д
	virtual void draw(const Mat4 &transform) = 0;
	//3D��ɫ����ʼ����������Ҫ��д
	virtual void setTarget(EffectSprite3D *sprite) = 0;
	//3D��ɫ����
	void bindShader(EffectSprite3D *sprite);
	//���û���˳��
	void setGlobalZOrder(float globalOrder);
	//��ȡ����˳��
	float getGlobalZOrder() {return _globalOrder;}
protected:
	//���졢��������
	Effect3D():_glProgramState(nullptr), _globalOrder(0), _sprite(nullptr){}
	virtual ~Effect3D()
	{
		//��ȫ�ͷ���ɫ��
		CC_SAFE_RELEASE(_glProgramState);
	}
	//����˳��
	float _globalOrder;
	//3D��Ч��ɫ��
	GLProgramState* _glProgramState;
	//ʵ��3D��Ч�ľ��������
	EffectSprite3D* _sprite;
};

//3D��Ч���飬�������3DЧ��
class EffectSprite3D : public Sprite3D
{
public:
	//3D��Ч���鴴������
	static EffectSprite3D* create(const std::string& model, const std::string& texture);
	static EffectSprite3D* create(const std::string& model);
	//���3D��Ч�������Ǳ���Ч����
	void addEffect(Effect3D* effect, ssize_t order, float globalZOrder = 0);
	//��дCCNode.h���������ڽ����Ʒ�����
	virtual void draw(Renderer *renderer, const Mat4 &transform,uint32_t flags) override;
protected:
	//���졢��������
	EffectSprite3D():_defaultEffect(nullptr){}
	virtual ~EffectSprite3D();
	//3D��Ч����vector������������Ч������Ԫ���ݵ�һ����������
	std::vector<std::tuple<ssize_t,Effect3D*,CustomCommand>> _effects;
	//����Ч��
	Effect3D* _defaultEffect;
	//�Զ���������ڰ��Լ���д��draw����
	CustomCommand _command;
};
//3D��Ч����Ӱ��
class Effect3DShadow: public Effect3D
{
public:
	//��������
	static Effect3DShadow* create();
	//���ò���Ӱ�ӵĹ�Դ��λ��
	void setLightPosition(const Vec3& position);
	//ʵ�ֻ���Ļ��Ƽ�������ɫ���ķ���
    virtual void draw(const Mat4 &transform) override;
    virtual void setTarget(EffectSprite3D *sprite) override;
protected:
    //���졢��������
    Effect3DShadow();
    virtual ~Effect3DShadow();
    //��ʼ������������Ϊ��
    bool init();
    //��Դλ��
    Vec3 _lightPosition;
protected:
	//��ӦEVENT_RENDERER_RECREATED�������³�ʼ�����ڰ�׿��WP8��WINDOWS_RT����Ч��
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		EventListenerCustom* _backToForegroundListener;
	#endif
protected:
    //��¼��ɫ��·�����޹���
    static const std::string _vertShaderFile;
    static const std::string _fragShaderFile;
    static const std::string _keyInGLProgramCache;
    //�й���
    static const std::string _vertSkinnedShaderFile;
    static const std::string _fragSkinnedShaderFile;
    static const std::string _keySkinnedInGLProgramCache;
};
#endif
