#pragma once

#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../internal/CosmicClone.hpp"

struct CosmicClonesTrigger;
class $modify(CosmicClonesGJBGL, GJBaseGameLayer)
{
    struct Fields {
        unsigned int m_p1Immunity = 0;
        unsigned int m_p2Immunity = 0;

        bool m_p1Jump = false;
        bool m_p2Jump = false;
        bool m_p1Frozen = false;
        bool m_p2Frozen = false;
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
