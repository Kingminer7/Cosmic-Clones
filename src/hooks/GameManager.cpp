#include "GameManager.hpp"

#include "GJBaseGameLayer.hpp"

void CosmicClonesGameManager::applicationWillEnterForeground() {
    GameManager::applicationWillEnterForeground();
    if (!PlayLayer::get()) return;
    auto bgl = reinterpret_cast<CosmicClonesGJBGL *>(PlayLayer::get());
    bgl->setupRenderTexture();
}
