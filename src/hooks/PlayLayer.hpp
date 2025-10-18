#pragma once

#include <Geode/modify/PlayLayer.hpp>

class $modify(CosmicClonesPlayLayer, PlayLayer) {
    void resetLevel();
    void setupHasCompleted();
    void levelComplete();
};