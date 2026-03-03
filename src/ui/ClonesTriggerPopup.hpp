#pragma once

#include <Geode/ui/Popup.hpp>

#include "../hooks/EditorUI.hpp"

class ClonesTriggerPopup : public geode::Popup {
protected:
    bool init(CosmicClonesTrigger* trigger);

    CosmicClonesTrigger* m_trigger = nullptr;

    cocos2d::CCMenu* m_spawnContainer = nullptr;
    cocos2d::CCMenu* m_touchContainer = nullptr;
    cocos2d::CCMenu* m_multiContainer = nullptr;
    cocos2d::CCMenu* m_countContainer = nullptr;
    cocos2d::CCMenu* m_idContainer = nullptr;
    cocos2d::CCMenu* m_delayContainer = nullptr;
    cocos2d::CCMenu* m_startDelayContainer = nullptr;
    cocos2d::CCMenu* m_disabledContainer = nullptr;
    cocos2d::CCMenu* m_damageContainer = nullptr;
    cocos2d::CCMenu* m_stopperContainer = nullptr;

    CCMenuItemToggler* m_spawnToggle = nullptr;
    CCMenuItemToggler* m_touchToggle = nullptr;
    CCMenuItemToggler* m_multiToggle = nullptr;

    CCMenuItemToggler* m_disabledToggle = nullptr;
    CCMenuItemToggler* m_damageToggle = nullptr;
    CCMenuItemToggler* m_stopperToggle = nullptr;

    geode::TextInput* m_countInput = nullptr;
    geode::TextInput* m_idInput = nullptr;
    geode::TextInput* m_delayInput = nullptr;
    Slider* m_delaySlider = nullptr;
    geode::TextInput* m_startDelayInput = nullptr;
    Slider* m_startDelaySlider = nullptr;
public:
    static ClonesTriggerPopup* create(CosmicClonesTrigger* trigger);
};
