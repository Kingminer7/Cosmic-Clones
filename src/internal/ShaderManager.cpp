#include "ShaderManager.hpp"

#include "Geode/loader/SettingV3.hpp"
#include "../Utils.hpp"
#include "../hooks/GJBaseGameLayer.hpp"
#include "Shaders.hpp"
#include "../ui/CloneStyleSetting.hpp"

using namespace geode::prelude;

ShaderManager& ShaderManager::get() {
    static ShaderManager instance;
    return instance;
}

CCGLProgram* ShaderManager::getCosmicShader() const {
    return m_shader;
}

void ShaderManager::setup() {
    m_hasSetup = true;
    m_shader = new CCGLProgram;
    if (!m_shader->initWithVertexShaderByteArray(g_cosmicVsh, g_cosmicFsh)) {
        log::error("Cosmic shader failed to load!\n{}\n{}", m_shader->fragmentShaderLog(), m_shader->vertexShaderLog()); // probably going to crash anyway
        delete m_shader;
        return;
    }

    m_shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    m_shader->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    m_shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

    m_shader->link();
    m_shader->updateUniforms();

    m_shader->setUniformLocationWith1i(m_shader->getUniformLocationForName("CC_Texture1"), 1);
    m_shader->setUniformLocationWith1i(m_shader->getUniformLocationForName("CC_Texture2"), 2);
    m_shader->setUniformLocationWith1i(m_shader->getUniformLocationForName("CC_Texture3"), 3);

    CCShaderCache::sharedShaderCache()->addProgram(m_shader, "cosmic"_spr);

    m_glCosmic = CCTextureCache::get()->addImage("cosmic.png"_spr, false)->getName();
    m_glNormal = CCTextureCache::get()->addImage("normal.png"_spr, false)->getName();
    m_glOverlay = CCTextureCache::get()->addImage("star.png"_spr, false)->getName();
    m_glTime = m_shader->getUniformLocationForName("u_time");
    m_glScreenSize = m_shader->getUniformLocationForName("u_screenSize");
}

void ShaderManager::update(const float dt) {
    if (!m_hasSetup) setup();
    if (!m_shader) return;
    m_shader->use();
    m_shader->setUniformsForBuiltins();

    ccGLBindTexture2DN(1, m_glCosmic);
    ccGLBindTexture2DN(2, m_glNormal);
    ccGLBindTexture2DN(3, m_glOverlay);

    m_time += dt;
    m_shader->setUniformLocationWith1f(m_glTime, m_time);

    auto size = CCDirector::sharedDirector()->getVisibleSize();
    m_shader->setUniformLocationWith2f(m_glScreenSize, size.width, size.height);
}

$on_mod(Loaded) {
    CCScheduler::get()->scheduleUpdateForTarget(&ShaderManager::get(), 0, false);

    listenForAllSettingChanges([](auto k, auto s) {
        if (auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(GJBaseGameLayer::get())) {
            //auto fields = bgl->m_fields.self();
            //int i = 0;
            // if (!fields->m_enabled) return;
            // if (!bgl->updateSettings(fields)) {
            //     for (const auto& clone : fields->m_clones) {
            //         clone->remove();
            //     }
            //     fields->m_clones.clear();
            //     fields->m_snapshots.clear();
            //     std::erase_if(fields->m_sfxIds, [](int channel) {
            //         return FMODAudioEngine::get()->m_stoppedChannels.find(channel) != FMODAudioEngine::get()->m_stoppedChannels.end();
            //     });
            //     for (auto channel : fields->m_sfxIds) {
            //         FMODAudioEngine::get()->stopChannel(channel);
            //     }
            // } else {
            //     const auto& styles = getSettingFast<"styles", std::vector<Style>>();
            //     for (const auto& clone : fields->m_clones) {
            //         if (clone == nullptr) continue;
            //         clone->updateStyle(styles[i % styles.size()]);
            //         i++;
            //     }
            // }
        }
    });
}
