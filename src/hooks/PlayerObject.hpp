#pragma once

#include <Geode/modify/PlayerObject.hpp>
class $modify(CosmicClonesPlayerObject, PlayerObject) {
    void incrementJumps();
    void enablePlayerControls();
    void disablePlayerControls();
};
