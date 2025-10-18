#include "ShaderManager.hpp"

#include <utility>

#include "Utils.hpp"
#include "hooks/GJBaseGameLayer.hpp"

using namespace geode::prelude;

ShaderManager& ShaderManager::get() {
    static ShaderManager instance;
    return instance;
}

CCGLProgram* ShaderManager::getCosmicShader() {
    auto cache = CCShaderCache::sharedShaderCache();
    auto shader = cache->programForKey("cosmic"_spr);
    if (shader) return shader;
    auto vsh = Mod::get()->getResourcesDir() / "cosmic.vsh";
    auto fsh = Mod::get()->getResourcesDir() / "cosmic.fsh";

    if (!std::filesystem::exists(vsh)) {
        log::error("Vertex shader doesn't exist.");
        return nullptr;
    }

    if (!std::filesystem::exists(fsh)) {
        log::error("Fragment shader doesn't exist.");
        return nullptr;
    }

    shader = new CCGLProgram;

    if (!shader->initWithVertexShaderFilename(string::pathToString(vsh.c_str()).c_str(), string::pathToString(fsh.c_str()).c_str())) {
        log::error("Shader failed to load!");
        log::error("{}", shader->fragmentShaderLog()); // probably going to crash anyway
        delete shader;
        return nullptr;
    }

    shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    shader->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

    shader->link();
    shader->updateUniforms();

    shader->setUniformLocationWith1i(shader->getUniformLocationForName("CC_Texture1"), 1);
    shader->setUniformLocationWith1i(shader->getUniformLocationForName("CC_Texture2"), 2);
    shader->setUniformLocationWith1i(shader->getUniformLocationForName("CC_Texture3"), 3);

    CCShaderCache::sharedShaderCache()->addProgram(shader, "cosmic"_spr);

    return shader;
}

CosmicSprite* CosmicSprite::create(CCTexture2D* base) {
    auto ret = new CosmicSprite;
    if (ret->init(base)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool CosmicSprite::init(CCTexture2D* base) {
    if (!CCSprite::initWithTexture(base)) return false;
    m_cosmicTex = CCTextureCache::get()->addImage("cosmic.png"_spr, false)->getName();
    m_normalTex = CCTextureCache::get()->addImage("normal.png"_spr, false)->getName();
    m_overlayTex = CCTextureCache::get()->addImage("star.png"_spr, false)->getName();

    updateStyle(getSettingFast<"style", std::string>());

    scheduleUpdate();

    return true;
}

void CosmicSprite::updateStyle(std::string style) {
    m_style = std::move(style);
    if (m_style == "Cosmic Mario (SMG 1)") setShaderProgram(ShaderManager::get().getCosmicShader());
    else setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
}

$on_mod(Loaded) {
    listenForSettingChanges("style", [](std::string style){
        if (auto pl = reinterpret_cast<CosmicClonesGJBGL*>(PlayLayer::get())) {
            auto fields = pl->m_fields.self();
            fields->m_sprite->updateStyle(style);
            for (auto clone : fields->m_clones) {
                clone->updateStyle(style);
            }
        }
    });
}

// taken from icon ninja
unsigned int* getNumberOfDraws() {
    // compiler yells at me if i use #elifdef :broken_heart:
    // thanks to jasmine for ALL of these
#if defined(GEODE_IS_MACOS)
    static_assert(GEODE_COMP_GD_VERSION == 22074, "Please update macOS offsets");
    return reinterpret_cast<unsigned int*>(geode::base::get() + GEODE_ARM_MAC(0x8b0f60) GEODE_INTEL_MAC(0x98bf30));
#elif defined(GEODE_IS_IOS)
    static_assert(GEODE_COMP_GD_VERSION == 22074, "Please update iOS offsets");
    return reinterpret_cast<unsigned int*>(geode::base::get() + 0x8791d0);
#else
    return &g_uNumberOfDraws;
#endif
}

void CosmicSprite::update(float dt) {
    m_time += dt;
}


void CosmicSprite::draw() {
    if (m_style != "Cosmic Mario (SMG 1)") return CCSprite::draw();
    CC_NODE_DRAW_SETUP();

    cocos2d::ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

    // note https://discord.com/channels/911701438269386882/911702535373475870/1421852242998329504
    // but for this to work in a batch node (or our fake batch node) we need to
    // use ccglbindtexture2dn instead of manual opengl for texture to get set
    // properly on every sprite after
    cocos2d::ccGLBindTexture2DN(0, m_pobTexture->getName());
    cocos2d::ccGLBindTexture2DN(1, m_cosmicTex);
    cocos2d::ccGLBindTexture2DN(2, m_normalTex);
    cocos2d::ccGLBindTexture2DN(3, m_overlayTex);

    GLint timeLoc = m_pShaderProgram->getUniformLocationForName("u_time");
    m_pShaderProgram->setUniformLocationWith1f(timeLoc, m_time);

    auto size = CCDirector::sharedDirector()->getVisibleSize();
    GLint sizeLoc = m_pShaderProgram->getUniformLocationForName("u_screenSize");
    m_pShaderProgram->setUniformLocationWith2f(sizeLoc, size.width, size.height);

    cocos2d::ccGLEnableVertexAttribs(cocos2d::kCCVertexAttribFlag_PosColorTex);

    // vertex
    int diff = offsetof(cocos2d::ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(cocos2d::kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(m_sQuad.bl), (void*)((uintptr_t)&m_sQuad + diff));

    // texCoods
    diff = offsetof(cocos2d::ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(cocos2d::kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(m_sQuad.bl), (void*)((uintptr_t)&m_sQuad + diff));

    // color
    diff = offsetof(cocos2d::ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(cocos2d::kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(m_sQuad.bl), (void*)((uintptr_t)&m_sQuad + diff));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    *getNumberOfDraws() += 1;
}