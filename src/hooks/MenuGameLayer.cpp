#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/utils/random.hpp>

//#include <undefined0.icon_ninja/include/events.hpp>

#include "../internal/ShaderManager.hpp"
#include "../internal/CosmicClone.hpp"

using namespace geode::prelude;

class $modify(ClonesMenuGameLayer, MenuGameLayer) {
    struct Fields {
        bool m_wasCloneLast = false;
    };

    void resetPlayer() {
        MenuGameLayer::resetPlayer();

        if (random::generate(0, 1000) == 309) {
            CosmicClone::updateShaderForPlayer(m_playerObject, ShaderManager::get().getCosmicShader(), true);
            m_fields->m_wasCloneLast = true;
        } else if (m_fields->m_wasCloneLast) {
            CosmicClone::updateShaderForPlayer(m_playerObject, CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor), true);
            m_fields->m_wasCloneLast = false;
        }
    }
};

$on_mod(Loaded) {
    /* ninja::NewPlayerEvent().listen([](PlayerObject* player){
        if (random::generate(0, 1000) == 309) {
            CosmicClone::updateShaderForPlayer(player, ShaderManager::get().getCosmicShader(), true);
        }
    }).leak(); */
}
