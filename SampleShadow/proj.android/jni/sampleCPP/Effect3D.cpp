#include "Effect3D.h"

//�����㷨������tuple��һ��Ԫ�صĴ�С����
static int tuple_sort( const std::tuple<ssize_t,Effect3D*,CustomCommand> &tuple1, const std::tuple<ssize_t,Effect3D*,CustomCommand> &tuple2 )
{
	return std::get<0>(tuple1) < std::get<0>(tuple2);
}
//�ص�����������shader����ͨ��
static void MatrixPalleteCallBack( GLProgram* glProgram, Uniform* uniform, int paletteSize, const float* palette)
{
	//��shader��������
	glUniform4fv( uniform->location, (GLsizei)paletteSize, (const float*)palette );
}
/************************************************************************************
 *							Effect3D			3DЧ������							*
 ************************************************************************************/
//3D��ɫ����
void Effect3D::bindShader(EffectSprite3D *sprite)
{
	//��ȡ��������
	auto mesh = sprite->getMesh();
//	auto mesh=sprite->getMeshByIndex(1);
	//����ƫ��
	long offset = 0;
	//��ȡ���鶥��������Ϣ
	auto attributeCount = mesh->getMeshVertexAttribCount();
	//�����������ݼ���
	for (auto k = 0; k < attributeCount; k++)
	{
		//�õ���������
		auto meshvertexattrib = mesh->getMeshVertexAttribute(k);
		//���������ݰ󶨸���ɫ��
		_glProgramState->setVertexAttribPointer(s_attributeNames[meshvertexattrib.vertexAttrib],
				meshvertexattrib.size,					//���ݴ�С
				meshvertexattrib.type,					//��������
				GL_FALSE,								//�Ƿ��׼��
				mesh->getVertexSizeInBytes(),			//����
				(GLvoid*)offset);						//λ��ָ��
		offset += meshvertexattrib.attribSizeBytes;
	}
}
//���û���˳��
void Effect3D::setGlobalZOrder(float globalOrder)
{
	if(_globalOrder!=globalOrder)
	{
		_globalOrder=globalOrder;
	}
}
/************************************************************************************
 *						Effect3DSprite3D			3D��Ч����						*
 ************************************************************************************/
//��������
EffectSprite3D* EffectSprite3D::create(const std::string &model, const std::string &texture)
{
	//new����
    auto sprite = new (std::nothrow) EffectSprite3D();
    //��ʼ�����ж��Ƿ�ɹ�
    if (sprite && sprite->initWithFile(model))
    {
    	//�����Զ��ͷ�
        sprite->autorelease();
        //����������ͼ
        if(texture.size() > 0)
            sprite->setTexture(texture);
        return sprite;
    }
    //ɾ������
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
//��������
EffectSprite3D* EffectSprite3D::create(const std::string &model)
{
    auto sprite = new (std::nothrow) EffectSprite3D();
    if (sprite && sprite->initWithFile(model))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
//��������
EffectSprite3D::~EffectSprite3D()
{
	for(auto effect : _effects)
	{
    	//���ü���-1����ָ��ΪNULL
    	CC_SAFE_RELEASE_NULL(std::get<1>(effect));
	}
	//���ü���-1
	CC_SAFE_RELEASE(_defaultEffect);
}
//���3D��Ч
void EffectSprite3D::addEffect(Effect3D* effect, ssize_t order, float globalZOrder /* = 0 */)
{
	if(nullptr == effect)
		return;
	//ָ������+1
	effect->retain();
	//���þ�����ɫ��
	effect->setTarget(this);
	//����Ч����vector
	_effects.push_back(std::make_tuple(order,effect,CustomCommand()));
	//����
	std::sort(std::begin(_effects), std::end(_effects), tuple_sort);
}
//�󶨻��Ʒ���
void EffectSprite3D::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
	//����3DЧ��vector���󶨻��Ʒ���
	for(auto &effect : _effects)
	{
		if(std::get<0>(effect) >=0)
			break;
        CustomCommand &cc = std::get<2>(effect);
        //ȫ�ֻ���˳��
        cc.init(std::get<1>(effect)->getGlobalZOrder());
        //���û��Ʒ���
        cc.func = CC_CALLBACK_0(Effect3D::draw,std::get<1>(effect),transform);
        //�������
        renderer->addCommand(&cc);
	}
	//�жϱ����Ƿ�����Ч������Ч���ø���draw
	if(!_defaultEffect)
	{
		Sprite3D::draw(renderer, transform, flags);
	}
	else
	{
		_command.init(_globalZOrder);
		_command.func = CC_CALLBACK_0(Effect3D::draw, _defaultEffect, transform);
		renderer->addCommand(&_command);
    }
	//����3DЧ��vector���󶨻��Ʒ���
	for(auto &effect : _effects)
	{
		if(std::get<0>(effect) <=0)
			continue;
		CustomCommand &cc = std::get<2>(effect);
		cc.init(std::get<1>(effect)->getGlobalZOrder());
		cc.func = CC_CALLBACK_0(Effect3D::draw,std::get<1>(effect),transform);
		renderer->addCommand(&cc);
    }
}
/************************************************************************************
 *						Effect3DShadow				Ӱ����Ч							*
 ************************************************************************************/
//�޹���
const std::string Effect3DShadow::_vertShaderFile = "shader/shadow.vert";
const std::string Effect3DShadow::_fragShaderFile = "shader/shadow.frag";
const std::string Effect3DShadow::_keyInGLProgramCache = "Effect3DLibrary_Shadow";
//������
const std::string Effect3DShadow::_vertSkinnedShaderFile = "shader/shadowSkin.vert";
const std::string Effect3DShadow::_fragSkinnedShaderFile = "shader/shadowSkin.frag";
const std::string Effect3DShadow::_keySkinnedInGLProgramCache = "Effect3DLibrary_Shadow";


Effect3DShadow::Effect3DShadow()
: _lightPosition(0, 500, 0)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _backToForegroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                          [this](EventCustom*)
                                                          {
                                                              auto glProgram = _glProgramState->getGLProgram();
                                                              glProgram->reset();
                                                              glProgram->initWithFilenames(_vertShaderFile, _fragShaderFile);
                                                              glProgram->link();
                                                              glProgram->updateUniforms();
                                                          }
                                                          );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backToForegroundListener, -1);
#endif
}

Effect3DShadow::~Effect3DShadow()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundListener);
#endif
}

Effect3DShadow* Effect3DShadow::create()
{
	Effect3DShadow* effect = new (std::nothrow) Effect3DShadow();
    if(effect && effect->init())
    {
        effect->autorelease();
        return effect;
    }
    else
    {
        CC_SAFE_DELETE(effect);
        return nullptr;
    }
}

bool Effect3DShadow::init()
{
    return true;
}
//���ù�Դλ��
void Effect3DShadow::setLightPosition(const Vec3& position)
{
    if(_lightPosition != position)
    {
    	_lightPosition = position;
        if(_glProgramState)
            _glProgramState->setUniformVec3("u_LightLocation", _lightPosition);
    }
}

void Effect3DShadow::setTarget(EffectSprite3D *sprite)
{
    CCASSERT(nullptr != sprite && nullptr != sprite->getMesh(),"Error: Setting a null pointer or a null mesh EffectSprite3D to Effect3D");

    if(sprite != _sprite)
    {
        GLProgram* glprogram;
        if(!sprite->getMesh()->getSkin())
            glprogram = GLProgram::createWithFilenames(_vertShaderFile, _fragShaderFile);
        else
            glprogram = GLProgram::createWithFilenames(_vertSkinnedShaderFile, _fragSkinnedShaderFile);

        _glProgramState = GLProgramState::create(glprogram);

        _glProgramState->retain();
        _glProgramState->setUniformVec3("u_LightLocation", _lightPosition);

        _sprite = sprite;

        bindShader(sprite);
    }
}

void Effect3DShadow::draw(const Mat4 &transform)
{
    //draw
    if(_sprite && _sprite->getMesh())
    {
    	//Ϊ��Ӱ��Ч����������ȼ�⣬��Ҫ�ֶ����ú�GlobalZOrder
    	GL::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    	_glProgramState->setUniformVec3("u_LightLocation", _lightPosition);

        CCLOG("------------------------%zd",_sprite->getMeshCount());
        int count=_sprite->getMeshCount();
        for(int i=0;i<count;i++){
        	auto mesh = _sprite->getMeshByIndex(i);
        	auto skin = mesh->getSkin();
        	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
			if(_sprite && skin)
			{
				auto function = std::bind(MatrixPalleteCallBack, std::placeholders::_1, std::placeholders::_2,
										  skin->getMatrixPaletteSize(), (float*)skin->getMatrixPalette());
				_glProgramState->setUniformCallback("u_matrixPalette", function);
			}

			if(_sprite)
				_glProgramState->apply(transform);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());

			glDrawElements(mesh->getPrimitiveType(), (GLsizei)mesh->getIndexCount(), mesh->getIndexFormat(), 0);
			CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, mesh->getIndexCount());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}


