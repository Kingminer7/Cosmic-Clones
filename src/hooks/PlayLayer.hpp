#pragma once

#include <Geode/modify/PlayLayer.hpp>

class $modify(CosmicClonesPlayLayer, PlayLayer) {
    void resetLevel();
    void fullReset();
    void setupHasCompleted();
    void levelComplete();
};
