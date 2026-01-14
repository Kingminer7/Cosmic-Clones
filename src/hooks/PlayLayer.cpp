#include "PlayLayer.hpp"
#include "GJBaseGameLayer.hpp"
#include "../CosmicClone.hpp"
#include "../Utils.hpp"

void CosmicClonesPlayLayer::resetLevel() {
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    auto tick = m_gameState.m_currentProgress - fields->m_offset;
    fields->m_stopped = false;
    if (!fields->m_enabled) {
        PlayLayer::resetLevel();
        if (bgl->updateSettings(fields)) fields->m_offset = m_gameState.m_currentProgress;
        return;
    }
    // wow one of my first actual explaining comments
    // basically in practice mode, it'd be better if the clones reset to where they were at said checkpoint
    // so we don't reset them.
    // we also give the player a second of immunity in case of spawn trap
    if (m_isPracticeMode) {
        PlayLayer::resetLevel();
        fields->m_p1Immunity = 240; // 1s immunity (except for stinky tps bypass)
        fields->m_p2Immunity = 240;
        std::vector<std::shared_ptr<CosmicClone>> toRem;
        for (auto clone = fields->m_clones.begin(); clone != fields->m_clones.end();) {
            if (clone->get()->getDelay() > tick) {
                clone->get()->remove();
                fields->m_clones.erase(clone);
            } else {
                ++clone;
            }
        }
        erase_if(fields->m_snapshots, [tick](const std::pair<const int, Snapshot>& time) {
            return time.first > tick;
        });
    } else {
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
        PlayLayer::resetLevel();
        if (!bgl->updateSettings(fields)) return;
        fields->m_offset = m_gameState.m_currentProgress;
    }
}

void CosmicClonesPlayLayer::fullReset() {
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    if (!fields->m_enabled) {
        PlayLayer::fullReset();
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
    PlayLayer::fullReset();
    if (!bgl->updateSettings(fields)) return;
    fields->m_offset = m_gameState.m_currentProgress;
}

void CosmicClonesPlayLayer::setupHasCompleted() {
    auto bgl = reinterpret_cast<CosmicClonesGJBGL*>(this);
    auto fields = bgl->m_fields.self();
    if (!bgl->updateSettings(fields)) return PlayLayer::setupHasCompleted();
    fields->m_stopped = false;
    PlayLayer::setupHasCompleted();
    fields->m_offset = m_gameState.m_currentProgress;
}

void CosmicClonesPlayLayer::levelComplete() {
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(this)->m_fields.self();
    PlayLayer::levelComplete();
    if (!fields->m_enabled) return;
    bool hasSfxed = false;
    for (const auto& clone : fields->m_clones) {
        if (!hasSfxed) {
            hasSfxed = true;
            if (getSettingFast<"sfx", bool>()) clone->playSFX(CosmicCloneSFXType::Die);
        }
        clone->getP1()->playerDestroyed(false);
        clone->getP2()->playerDestroyed(false);
    }
    fields->m_stopped = true;
}
