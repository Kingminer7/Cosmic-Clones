#pragma once

#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../CosmicClone.hpp"

struct CosmicClonesTrigger;
class $modify(CosmicClonesGJBGL, GJBaseGameLayer)
{
    struct Fields {
        std::vector<std::shared_ptr<CosmicClone>> m_clones;
        std::map<unsigned int, Snapshot> m_snapshots;
        std::vector<int> m_sfxIds;

        unsigned int m_offset = 0;
        unsigned int m_p1Immunity = 0;
        unsigned int m_p2Immunity = 0;

        bool m_startsImmediately = false;
        bool m_stopped = true;

        bool m_p1Jump = false;
        bool m_p2Jump = false;
        bool m_p1Frozen = false;
        bool m_p2Frozen = false;

        bool m_enabled;
        unsigned int m_count;
        float m_initialDelay;
        float m_delay;
        bool m_damage = true;
    };

    #ifndef GEODE_IS_MACOS
    void processCommands(float dt, bool half, bool last);
    #else
    void processQueuedButtons(float dt, bool clear);
    #endif
    
    bool updateSettings(Fields* fields);
    void updateFromTrigger(const CosmicClonesTrigger* trigger);
    bool init();
    void startClones();
};
