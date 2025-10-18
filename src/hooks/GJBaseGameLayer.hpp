#pragma once

#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../CosmicClone.hpp"
#include "../ShaderManager.hpp"

class $modify(CosmicClonesGJBGL, GJBaseGameLayer) {
    struct Fields {
        std::vector<std::shared_ptr<CosmicClone>> m_clones;
        std::map<int, Snapshot> m_snapshots;
        std::vector<int> m_sfxIds;

        float m_initialDelay = 0.f;
        float m_delay;
        int m_count;
        int m_offset = 0;
        int m_p1Immunity = 0;
        int m_p2Immunity = 0;

        bool m_stopped = false;
        bool m_p1Jump = false;
        bool m_p2Jump = false;
        bool m_p1Frozen = false;
        bool m_p2Frozen = false;
        bool m_enabled = geode::Mod::get()->getSettingValue<bool>("enabled");

        cocos2d::CCRenderTexture* m_renderTex;
        cocos2d::CCLayer* m_renderLayer;

        CosmicSprite* m_sprite;
    };

    void processCommands(float dt);
    void visit();
};
