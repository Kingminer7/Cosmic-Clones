#include "GJBaseGameLayer.hpp"

#include <Geode/Geode.hpp>

#include "../internal/CosmicClonesController.hpp"

void CosmicClonesGJBGL::toHook() {
    auto fields = m_fields.self();

    fields->m_controller->tick(m_gameState.m_currentProgress);
    for (auto [id, cont] : fields->m_triggerControllers) {
        cont->tick(m_gameState.m_currentProgress);
    }

    GJBaseGameLayer::orig();
}