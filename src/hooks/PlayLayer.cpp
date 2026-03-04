#include "PlayLayer.hpp"
#include "GJBaseGameLayer.hpp"
#include "../internal/CosmicClonesController.hpp"
#include "../Utils.hpp"
#include <ninxout.options_api/include/API.hpp>
#include <cvolton.level-id-api/include/EditorIDs.hpp>

inline bool isEnabledForLevel(GJGameLevel* level) {
    if (level->m_levelType == GJLevelType::Editor) {
        return Mod::get()->getSavedValue<bool>(fmt::format("enable-in-editor-{}", EditorIDs::getID(level)));
    }
    return Mod::get()->getSavedValue<bool>(fmt::format("enable-in-{}", level->m_levelID));
}

void CosmicClonesPlayLayer::resetLevel() {
    PlayLayer::resetLevel();
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    if (!fields->m_controller) return;
    fields->m_autoClones = getSettingFast<"enabled", bool>() || isEnabledForLevel(m_level);

    // wow one of my first actual explaining comments
    // basically in practice mode, it'd be better if the clones reset to where they were at said checkpoint
    // so we don't reset them.
    // we also give the player a second of immunity in case of spawn trap
    if (m_isPracticeMode) {
        fields->m_p1Immunity = 240; // 1s immunity (except for stinky tps bypass)
        fields->m_p2Immunity = 240;

        if (fields->m_autoClones && !fields->m_controller->isStopped()) fields->m_controller->softReset(m_gameState.m_currentProgress);
    } else {
        if (!fields->m_controller->isStopped()) fields->m_controller->stop(true);
        for (const auto& [id, cont] : fields->m_triggerControllers){
            if (!cont->isStopped()) {
                cont->stop(true);
                if (cont->getOffset() <= m_gameState.m_currentProgress) cont->start();
            }
        }

        fields->m_controller->loadConfigFromSettings();
        if (fields->m_autoClones) fields->m_controller->start();
    }
}

void CosmicClonesPlayLayer::fullReset() {
    PlayLayer::fullReset();
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    if (!fields->m_controller->isStopped()) fields->m_controller->stop(true);
    for (const auto& [id, cont] : fields->m_triggerControllers){
        if (!cont->isStopped()) cont->stop(true);
    }

    fields->m_autoClones = getSettingFast<"enabled", bool>() || isEnabledForLevel(m_level);
    fields->m_controller->loadConfigFromSettings();
    if (fields->m_autoClones) fields->m_controller->start();
}

void CosmicClonesPlayLayer::setupHasCompleted() {
    PlayLayer::setupHasCompleted();
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    fields->m_autoClones = getSettingFast<"enabled", bool>() || isEnabledForLevel(m_level);
    fields->m_controller = CosmicClonesController::createWithSettings(bgl);
    if (fields->m_autoClones) fields->m_controller->start();
}

void CosmicClonesPlayLayer::levelComplete() {
    PlayLayer::levelComplete();
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(this)->m_fields.self();
    if (fields->m_controller->isStopped()) return;
    fields->m_controller->stop();
    for (const auto& [id, cont] : fields->m_triggerControllers){
        if (!cont->isStopped()) cont->stop();
    }
}

$on_mod(Loaded) {
    OptionsAPI::addPreLevelSetting<bool>(
        "Clones in This Level",
        "clones-in-level"_spr,
        [](GJGameLevel* level) {
            std::string key;
            if (level->m_levelType != GJLevelType::Editor) {
                key = fmt::format("enable-in-{}", level->m_levelID);
            } else {
                key = fmt::format("enable-in-editor-{}", EditorIDs::getID(level));
            }
            Mod::get()->setSavedValue<bool>(key, !Mod::get()->getSavedValue<bool>(key, false));
        },
        [](GJGameLevel* level) {
            return isEnabledForLevel(level);
        },
        "If clones should spawn from the start in this level. No effect if \"Clones in Every Level\" is enabled."
    );
}