#include "Effect3D.h"

//排序算法，根据tuple第一个元素的大小排序
static int tuple_sort( const std::tuple<ssize_t,Effect3D*,CustomCommand> &tuple1, const std::tuple<ssize_t,Effect3D*,CustomCommand> &tuple2 )
{
	return std::get<0>(tuple1) < std::get<0>(tuple2);
}
//回调方法用于与shader进行通信
static void MatrixPalleteCallBack( GLProgram* glProgram, Uniform* uniform, int paletteSize, const float* palette)
{
	//向shader传递数据
	glUniform4fv( uniform->location, (GLsizei)paletteSize, (const float*)palette );
}
/************************************************************************************
 *							Effect3D			3D效果基类							*
 ************************************************************************************/
//3D着色器绑定
void Effect3D::bindShader(EffectSprite3D *sprite)
{
	//获取精灵属性
	auto mesh = sprite->getMesh();
//	auto mesh=sprite->getMeshByIndex(1);
	//数据偏移
	long offset = 0;
	//获取精灵顶点属性信息
	auto attributeCount = mesh->getMeshVertexAttribCount();
	//遍历顶点数据集合
	for (auto k = 0; k < attributeCount; k++)
	{
		//得到顶点数据
		auto meshvertexattrib = mesh->getMeshVertexAttribute(k);
		//将顶点数据绑定给着色器
		_glProgramState->setVertexAttribPointer(s_attributeNames[meshvertexattrib.vertexAttrib],
				meshvertexattrib.size,					//数据大小
				meshvertexattrib.type,					//数据类型
				GL_FALSE,								//是否标准化
				mesh->getVertexSizeInBytes(),			//步长
				(GLvoid*)offset);						//位置指针
		offset += meshvertexattrib.attribSizeBytes;
	}
}
//设置绘制顺序
void Effect3D::setGlobalZOrder(float globalOrder)
{
	if(_globalOrder!=globalOrder)
	{
		_globalOrder=globalOrder;
	}
}
/************************************************************************************
 *						Effect3DSprite3D			3D特效精灵						*
 ************************************************************************************/
//创建方法
EffectSprite3D* EffectSprite3D::create(const std::string &model, const std::string &texture)
{
	//new精灵
    auto sprite = new (std::nothrow) EffectSprite3D();
    //初始化并判断是否成功
    if (sprite && sprite->initWithFile(model))
    {
    	//设置自动释放
        sprite->autorelease();
        //设置纹理贴图
        if(texture.size() > 0)
            sprite->setTexture(texture);
        return sprite;
    }
    //删除对象
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
//创建方法
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
//析构函数
EffectSprite3D::~EffectSprite3D()
{
	for(auto effect : _effects)
	{
    	//引用计数-1，置指针为NULL
    	CC_SAFE_RELEASE_NULL(std::get<1>(effect));
	}
	//引用计数-1
	CC_SAFE_RELEASE(_defaultEffect);
}
//添加3D特效
void EffectSprite3D::addEffect(Effect3D* effect, ssize_t order, float globalZOrder /* = 0 */)
{
	if(nullptr == effect)
		return;
	//指针引用+1
	effect->retain();
	//设置精灵着色器
	effect->setTarget(this);
	//将特效加入vector
	_effects.push_back(std::make_tuple(order,effect,CustomCommand()));
	//排序
	std::sort(std::begin(_effects), std::end(_effects), tuple_sort);
}
//绑定绘制方法
void EffectSprite3D::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
	//遍历3D效果vector，绑定绘制方法
	for(auto &effect : _effects)
	{
		if(std::get<0>(effect) >=0)
			break;
        CustomCommand &cc = std::get<2>(effect);
        //全局绘制顺序
        cc.init(std::get<1>(effect)->getGlobalZOrder());
        //设置绘制方法
        cc.func = CC_CALLBACK_0(Effect3D::draw,std::get<1>(effect),transform);
        //添加命令
        renderer->addCommand(&cc);
	}
	//判断本身是否有特效，无特效调用父类draw
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
	//遍历3D效果vector，绑定绘制方法
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
 *						Effect3DShadow				影子特效							*
 ************************************************************************************/
//无骨骼
const std::string Effect3DShadow::_vertShaderFile = "shader/shadow.vert";
const std::string Effect3DShadow::_fragShaderFile = "shader/shadow.frag";
const std::string Effect3DShadow::_keyInGLProgramCache = "Effect3DLibrary_Shadow";
//含骨骼
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
//设置光源位置
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
    	//为了影子效果，不打开深度检测，需要手动设置后GlobalZOrder
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


