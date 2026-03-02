#include <unordered_map>
#include "LevelEditorLayer.hpp"
#include "GJBaseGameLayer.hpp"
#include "../Utils.hpp"
#include "../internal/CosmicClonesController.hpp"

bool CosmicClonesLevelEditorLayer::init(GJGameLevel* level, bool noUI) {
    if (!LevelEditorLayer::init(level, noUI)) return false;
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    fields->m_autoClones = getSettingFast<"enabled", bool>();
    fields->m_controller = CosmicClonesController::createWithSettings(bgl);
    return true;
}

void CosmicClonesLevelEditorLayer::onPlaytest() {
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    LevelEditorLayer::onPlaytest();
    fields->m_autoClones = getSettingFast<"enabled", bool>() && getSettingFast<"editor", bool>();
    if (!fields->m_autoClones) return;
    fields->m_controller->loadConfigFromSettings();
    fields->m_controller->start();
    for (auto [id, cont] : fields->m_triggerControllers){
        cont->loadConfigFromTrigger();
    }
}

void CosmicClonesLevelEditorLayer::onStopPlaytest() {
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    LevelEditorLayer::onStopPlaytest();
    geode::log::info("Stopping");
    if (!fields->m_controller->isStopped()) fields->m_controller->stop(true);
    for (auto [id, cont] : fields->m_triggerControllers){
        if (!cont->isStopped()) cont->stop(true);
        geode::log::info("stopping a controller");
    }
}
