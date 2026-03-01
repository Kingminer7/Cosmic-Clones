#pragma once

#include <Geode/ui/Popup.hpp>

#include "hooks/EditorUI.hpp"

class ClonesTriggerPopup : public geode::Popup {
protected:
    bool init(CosmicClonesTrigger* trigger);

    CosmicClonesTrigger* m_trigger = nullptr;

    CCMenuItemToggler* m_spawnToggle = nullptr;
    cocos2d::CCLabelBMFont* m_spawnLabel = nullptr;
    CCMenuItemToggler* m_touchToggle = nullptr;
    cocos2d::CCLabelBMFont* m_touchLabel = nullptr;
    CCMenuItemToggler* m_multiToggle = nullptr;
    cocos2d::CCLabelBMFont* m_multiLabel = nullptr;

    geode::TextInput* m_countInput = nullptr;
    geode::TextInput* m_delayInput = nullptr;
    Slider* m_delaySlider = nullptr;
    geode::Function<void(Slider* slider)> m_delayCallback = nullptr;

    void activateCallback(CCObject* sender);
public:
    static ClonesTriggerPopup* create(CosmicClonesTrigger* trigger);
};
