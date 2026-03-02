#pragma once

#include <Geode/modify/LevelEditorLayer.hpp>

class $modify(CosmicClonesLevelEditorLayer, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool noUI);
    void onPlaytest();
    void onStopPlaytest();
};
