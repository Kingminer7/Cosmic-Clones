#pragma once

#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../CosmicClone.hpp"

class $modify(CosmicClonesGJBGL, GJBaseGameLayer) {
    struct Fields {
        std::vector<std::shared_ptr<CosmicClone>> m_clones;
        std::map<int, Snapshot> m_snapshots;
        std::vector<int> m_sfxIds;

        int m_offset = 0;
        int m_p1Immunity = 0;
        int m_p2Immunity = 0;

        bool m_stopped = false;
        bool m_p1Jump = false;
        bool m_p2Jump = false;
        bool m_p1Frozen = false;
        bool m_p2Frozen = false;

        bool m_enabled;
        float m_initialDelay;
        float m_delay;
        int m_count;
    };

    void processCommands(float dt, bool half, bool last);
    bool updateSettings(Fields* fields);
    bool init();
};
