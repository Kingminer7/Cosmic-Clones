#pragma once

#include <Geode/ui/Popup.hpp>

#include "../hooks/EditorUI.hpp"

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

    CCMenuItemToggler* m_disabledToggle = nullptr;
    cocos2d::CCLabelBMFont* m_disabledLabel = nullptr;
    CCMenuItemToggler* m_damageToggle = nullptr;
    cocos2d::CCLabelBMFont* m_damageLabel = nullptr;


    geode::TextInput* m_countInput = nullptr;
    geode::TextInput* m_delayInput = nullptr;
    Slider* m_delaySlider = nullptr;
    geode::TextInput* m_startDelayInput = nullptr;
    Slider* m_startDelaySlider = nullptr;
public:
    static ClonesTriggerPopup* create(CosmicClonesTrigger* trigger);
};
