#include "CosmicClonesController.hpp"

#include <arc/time/Sleep.hpp>

#include "../hooks/GJBaseGameLayer.hpp"

using namespace geode::prelude;

bool CosmicClonesController::start() {
    if (!m_stopped) {
        log::error("Could not start cosmic clones controller: this controller is already running!");
        return false;
    }
    if (!m_bgl) {
        log::error("Could not start cosmic clones controller: GJBGL doesn't exist!");
        return false;
    }

    m_stopped = false;
    m_startOffset = m_bgl->m_gameState.m_currentProgress;
    return true;
}

void CosmicClonesController::stop(bool immediate) {
    if (m_stopped) return log::error("Could not stop cosmic clones controller: this controller is already stopped!");
    bool hasSfxed = false;
    if (!immediate) {
        for (const auto& clone : m_clones) {
            if (!hasSfxed) {
                hasSfxed = true;
                if (m_sfx) clone->playSFX(CosmicCloneSFXType::Die);
            }
            clone->getP1()->playerDestroyed(false);
            clone->getP2()->playerDestroyed(false);
        }
        async::spawn(arc::sleep(asp::Duration::fromSecs(1)), [this] {
            cleanup();
        });
    } else {
        cleanup();
    }
    m_stopped = true;
}

void CosmicClonesController::cleanup() {
    for (const auto& clone : m_clones) {
        clone->remove();
    }
    m_clones.clear();
    m_snapshots.clear();
    for (const auto channel : m_sfxIds) {
        FMODAudioEngine::get()->stopChannel(channel);
    }
    m_sfxIds.clear();
}

void CosmicClonesController::tick() {

}

CosmicClonesController* CosmicClonesController::createWithSettings(CosmicClonesGJBGL* bgl) {
    auto ret = new CosmicClonesController(bgl);
    ret->loadConfigFromSettings();
    return ret;
}

CosmicClonesController* CosmicClonesController::createFromTrigger(CosmicClonesGJBGL* bgl, CosmicClonesTrigger* trigger) {
    auto ret = new CosmicClonesController(bgl);
    ret->loadConfigFromTrigger(trigger);
    return ret;
}

CosmicClonesController::CosmicClonesController(CosmicClonesGJBGL* bgl) : m_bgl(bgl) {}
