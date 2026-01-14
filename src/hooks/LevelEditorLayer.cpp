#include <unordered_map>
#include "LevelEditorLayer.hpp"
#include "GJBaseGameLayer.hpp"
#include "../CosmicClone.hpp"

void CosmicClonesLevelEditorLayer::onPlaytest() {
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    if (!bgl->updateSettings(fields)) return LevelEditorLayer::onPlaytest();
    fields->m_stopped = false;
    LevelEditorLayer::onPlaytest();
    fields->m_offset = m_gameState.m_currentProgress;
}

void CosmicClonesLevelEditorLayer::onStopPlaytest() {
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    if (!fields->m_enabled) {
        LevelEditorLayer::onStopPlaytest();
        if (bgl->updateSettings(fields)) fields->m_offset = m_gameState.m_currentProgress;
        return;
    }
    for (const auto& clone : fields->m_clones) {
        clone->remove();
    }
    fields->m_clones.clear();
    fields->m_snapshots.clear();
    std::erase_if(fields->m_sfxIds, [](int channel) {
        return FMODAudioEngine::get()->m_stoppedChannels.find(channel) != FMODAudioEngine::get()->m_stoppedChannels.end();
    });
    for (auto channel : fields->m_sfxIds) {
        FMODAudioEngine::get()->stopChannel(channel);
    }
    if (!bgl->updateSettings(fields)) return;
    LevelEditorLayer::onStopPlaytest();
}
