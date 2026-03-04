#pragma once

#include <Geode/ui/Button.hpp>
#include <Geode/ui/Popup.hpp>

#include "CloneStyleSetting.hpp"
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
    cocos2d::CCMenu* m_sfxContainer = nullptr;

    CCMenuItemToggler* m_spawnToggle = nullptr;
    CCMenuItemToggler* m_touchToggle = nullptr;
    CCMenuItemToggler* m_multiToggle = nullptr;

    CCMenuItemToggler* m_disabledToggle = nullptr;
    CCMenuItemToggler* m_damageToggle = nullptr;
    CCMenuItemToggler* m_stopperToggle = nullptr;
    CCMenuItemToggler* m_sfxToggle = nullptr;

    geode::TextInput* m_countInput = nullptr;
    geode::TextInput* m_idInput = nullptr;
    geode::TextInput* m_delayInput = nullptr;
    Slider* m_delaySlider = nullptr;
    geode::TextInput* m_startDelayInput = nullptr;
    Slider* m_startDelaySlider = nullptr;

    CCLayer* m_page1 = nullptr;
    CCLayer* m_page2 = nullptr;

    geode::ScrollLayer* m_styleScroll = nullptr;
    geode::Button* m_newBtn = nullptr; 
public:
    void onClose(CCObject* ) override;

    geode::ScrollLayer* getScroll() const;

    geode::Button* getNewBtn() const;

    static ClonesTriggerPopup* create(CosmicClonesTrigger* trigger);
};


class StyleNode : public cocos2d::CCMenu {
protected:
    bool init(ClonesTriggerPopup* popup, Style value);

    static std::vector<Style> m_styles;
    Style m_style;
    cocos2d::CCLabelBMFont* m_label = nullptr;
    ClonesTriggerPopup* m_popup = nullptr;
    CCMenu* m_customMenu = nullptr;

    SimplePlayer* m_preview = nullptr;
public:
    void updateState(const Style& style);
    const Style& getStyle() { return m_style; }

    ClonesTriggerPopup* getPopup() const;

    static StyleNode* create(ClonesTriggerPopup* popup, const Style& value);
};