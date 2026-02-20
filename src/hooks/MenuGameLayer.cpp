#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

#include "../ShaderManager.hpp"
#include "../CosmicClone.hpp"

#include "Geode/utils/random.hpp"
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
