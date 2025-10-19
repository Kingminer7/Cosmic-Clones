#include "PlayLayer.hpp"
#include "GJBaseGameLayer.hpp"
#include "../CosmicClone.hpp"
#include "../Utils.hpp"

void CosmicClonesPlayLayer::resetLevel() {
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(this)->m_fields.self();
    if (!fields->m_enabled) return PlayLayer::resetLevel();
    auto tick = m_gameState.m_currentProgress - fields->m_offset;
    fields->m_stopped = false;
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
                auto p1 = clone->get()->getP1();
                p1->toggleGhostEffect(GhostType::Disabled);
                p1->removeFromParent();

                auto p2 = clone->get()->getP2();
                p2->toggleGhostEffect(GhostType::Disabled);
                p2->removeFromParent();
                fields->m_clones.erase(clone);
            } else {
                ++clone;
            }
        }
        erase_if(fields->m_snapshots, [this, tick](std::pair<const int, Snapshot> time) {
            return time.first > tick;
        });
    } else {
        for (auto clone : fields->m_clones) {
            auto p1 = clone->getP1();
            p1->toggleGhostEffect(GhostType::Disabled);
            p1->removeFromParent();

            auto p2 = clone->getP2();
            p2->toggleGhostEffect(GhostType::Disabled);
            p2->removeFromParent();
        }
        fields->m_clones.clear();
        fields->m_snapshots.clear();

        fields->m_count = getSettingFast<"clones", int>();
        fields->m_delay = getSettingFast<"delay", float>();
        PlayLayer::resetLevel();
        fields->m_offset = m_gameState.m_currentProgress;
        fields->m_initialDelay = getSettingFast<"spawn-delay", float>();
        fields->m_sfxIds.erase(
            std::remove_if(fields->m_sfxIds.begin(), fields->m_sfxIds.end(), [](int channel) {
                return FMODAudioEngine::get()->m_stoppedChannels.find(channel) != FMODAudioEngine::get()->m_stoppedChannels.end();
            }), fields->m_sfxIds.end());
        for (auto channel : fields->m_sfxIds) {
            FMODAudioEngine::get()->stopChannel(channel);
        }
    }
}

void CosmicClonesPlayLayer::setupHasCompleted() {
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(this)->m_fields.self();
    if (!m_isPlatformer && !getSettingFast<"normal-mode", bool>()) fields->m_enabled = false;
    if (!fields->m_enabled) return PlayLayer::setupHasCompleted();
    fields->m_count = getSettingFast<"clones", int>();
    fields->m_delay = getSettingFast<"delay", float>();
    fields->m_stopped = false;
    PlayLayer::setupHasCompleted();
    fields->m_offset = m_gameState.m_currentProgress;
    fields->m_initialDelay = getSettingFast<"spawn-delay", float>();
    auto ws = cocos2d::CCDirector::get()->getWinSize();
    fields->m_renderTex = cocos2d::CCRenderTexture::create(ws.width, ws.height);
    fields->m_renderTex->setPosition(ws / 2);
    fields->m_renderLayer = CCLayer::create();
    fields->m_renderLayer->setContentSize({0, 0});
    fields->m_renderLayer->setScale(m_objectLayer->getScale());
    fields->m_renderLayer->setPosition(m_objectLayer->getPosition());
    fields->m_renderTex->addChild(fields->m_renderLayer);
    fields->m_renderTex->setVisible(false);
    fields->m_sprite = CosmicSprite::create(fields->m_renderTex->m_pTexture);
    fields->m_sprite->setFlipY(true);
    fields->m_sprite->setScale(1 / m_objectLayer->getScale());
    fields->m_sprite->addChild(fields->m_renderTex);
    m_objectLayer->addChild(fields->m_sprite, m_player1->getZOrder());
    fields->m_sprite->setPosition(fields->m_sprite->getContentSize() / 2 - m_objectLayer->getPosition() / m_objectLayer->getScale());
}

void CosmicClonesPlayLayer::levelComplete() {
    auto fields = reinterpret_cast<CosmicClonesGJBGL*>(this)->m_fields.self();
    PlayLayer::levelComplete();
    if (!fields->m_enabled) return;
    bool hasSfxed = false;
    for (const auto& clone : fields->m_clones) {
        if (!hasSfxed) {
            hasSfxed = true;
            clone->playSFX(CosmicCloneSFXType::Die);
        }
        clone->getP1()->playerDestroyed(false);
        clone->getP2()->playerDestroyed(false);
    }
    fields->m_stopped = true;
}
