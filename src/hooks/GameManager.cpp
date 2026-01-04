#include "GameManager.hpp"

#include "GJBaseGameLayer.hpp"

void CosmicClonesGameManager::applicationWillEnterForeground() {
    GameManager::applicationWillEnterForeground();
    if (!PlayLayer::get()) return;
    auto bgl = reinterpret_cast<CosmicClonesGJBGL *>(PlayLayer::get());
    for (auto clone : bgl->m_fields->m_clones) {
        // clone->getP1Sprite()->resetRenderTexture();
        // clone->getP2Sprite()->resetRenderTexture();
    }
}
