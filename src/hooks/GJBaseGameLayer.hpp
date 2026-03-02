#pragma once

#include <Geode/modify/GJBaseGameLayer.hpp>

#ifdef GEODE_IS_MACOS
#define toHook() processQueuedButtons(float dt, bool clear)
#define orig() processQueuedButtons(dt, clear)
#else
#define toHook() processCommands(float dt, bool half, bool last)
#define orig() processCommands(dt, half, last)
#endif

struct CosmicClonesTrigger;
class CosmicClonesController;

class $modify(CosmicClonesGJBGL, GJBaseGameLayer)
{
    struct Fields {
        unsigned int m_p1Immunity = 0;
        unsigned int m_p2Immunity = 0;

        bool m_p1Jump = false;
        bool m_p2Jump = false;
        bool m_p1Frozen = false;
        bool m_p2Frozen = false;

		bool m_autoClones = false;
		std::shared_ptr<CosmicClonesController> m_controller;
        std::unordered_map<int, std::shared_ptr<CosmicClonesController>> m_triggerControllers;
    };

    void toHook();
};
