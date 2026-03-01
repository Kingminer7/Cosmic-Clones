#include "ClonesTriggerPopup.hpp"

using namespace geode::prelude;

bool ClonesTriggerPopup::init(CosmicClonesTrigger* trigger) {
    if (!Popup::init(320, 260)) return false;
    m_trigger = trigger;
    setTitle("Cosmic Clones Trigger");
    static_cast<AnchorLayoutOptions*>(m_title->getLayoutOptions())->setOffset({0, -16});
    m_title->setScale(0.5f);
    m_title->setFntFile("bigFont.fnt");
    m_title->setID("title");
    m_bgSprite->setID("background");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_closeBtn->removeFromParent();
    setID("ClonesTriggerPopup");
    m_mainLayer->updateLayout();

    // 18, 18

    auto infoBtn = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_infoIcon_001.png", 1.f, [](auto){
        FLAlertLayer::create("Help", "TODO", "OK")->show();
    });
    m_buttonMenu->addChildAtPosition(infoBtn, Anchor::TopLeft, {18, -18});

    auto spawnContainer = CCMenu::create();
    spawnContainer->setID("spawn-container");
    spawnContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(spawnContainer, Anchor::BottomLeft, {20, 20});
    m_spawnToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->m_isSpawnTriggered = !toggle->isToggled();
        m_trigger->m_isTouchTriggered = false;
        m_touchToggle->toggle(false);
        m_multiToggle->setVisible(m_trigger->m_isTouchTriggered || m_trigger->m_isSpawnTriggered);
        m_multiLabel->setVisible(m_trigger->m_isTouchTriggered || m_trigger->m_isSpawnTriggered);
    });
    m_spawnToggle->toggle(trigger->m_isSpawnTriggered);
    m_spawnToggle->setID("spawn-toggle");
    spawnContainer->addChild(m_spawnToggle);

    m_spawnLabel = CCLabelBMFont::create("Spawn\nTrigger", "bigFont.fnt");
    m_spawnLabel->setAnchorPoint({0, 0.5});
    m_spawnLabel->setID("spawn-label");
    m_spawnLabel->setScale(.35);
    m_spawnLabel->setAlignment(kCCTextAlignmentLeft);
    m_spawnLabel->setPositionX(20.35);
    spawnContainer->addChild(m_spawnLabel);

    auto touchContainer = CCMenu::create();
    touchContainer->setID("touch-container");
    touchContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(touchContainer, Anchor::BottomLeft, {20, 50});

    m_touchToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->m_isTouchTriggered = !toggle->isToggled();
        m_trigger->m_isSpawnTriggered = false;
        m_spawnToggle->toggle(false);
        m_multiToggle->setVisible(m_trigger->m_isTouchTriggered || m_trigger->m_isSpawnTriggered);
        m_multiLabel->setVisible(m_trigger->m_isTouchTriggered || m_trigger->m_isSpawnTriggered);
    });
    m_touchToggle->toggle(trigger->m_isTouchTriggered);
    m_touchToggle->setID("touch-toggle");
    touchContainer->addChild(m_touchToggle);

    m_touchLabel = CCLabelBMFont::create("Touch\nTrigger", "bigFont.fnt");
    m_touchLabel->setAnchorPoint({0, 0.5});
    m_touchLabel->setID("touch-label");
    m_touchLabel->setScale(.35);
    m_touchLabel->setAlignment(kCCTextAlignmentLeft);
    m_touchLabel->setPositionX(20.35);
    touchContainer->addChild(m_touchLabel);

    auto multiContainer = CCMenu::create();
    multiContainer->setID("multi-container");
    multiContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(multiContainer, Anchor::BottomRight, {-75, 50});

    m_multiToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->m_isMultiTriggered = !toggle->isToggled();
    });
    m_multiToggle->toggle(trigger->m_isMultiTriggered);
    m_multiToggle->setID("multi-toggle");
    m_multiToggle->setVisible(trigger->m_isTouchTriggered || trigger->m_isSpawnTriggered);
    multiContainer->addChild(m_multiToggle);

    m_multiLabel = CCLabelBMFont::create("Multi\nTrigger", "bigFont.fnt");
    m_multiLabel->setAnchorPoint({0, 0.5});
    m_multiLabel->setID("multi-label");
    m_multiLabel->setScale(.35);
    m_multiLabel->setAlignment(kCCTextAlignmentLeft);
    m_multiLabel->setPositionX(20.35);
    m_multiLabel->setVisible(trigger->m_isTouchTriggered || trigger->m_isSpawnTriggered);
    multiContainer->addChild(m_multiLabel);

    auto countContainer = CCMenu::create();
    countContainer->setID("count-container");
    countContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(countContainer, Anchor::TopLeft, {75, -75});

    m_countInput = TextInput::create(70, nullptr);
    m_countInput->setID("count-input");
    m_countInput->setScale(.9);
    m_countInput->setCommonFilter(CommonFilter::Uint);
    m_countInput->setString(numToString(trigger->getCount()));
    m_countInput->setCallback([this](const std::string& str) {
        auto res = numFromString<int>(str);
        if (!res) return log::warn("Failed to convert contents of count '{}' to integer: {}", str, res.unwrapErr());
        m_trigger->setCount(std::clamp(res.unwrap(), 1, 20));
    });
    countContainer->addChild(m_countInput);

    auto countLeftBtn = CCMenuItemExt::createSpriteExtraWithFrameName("edit_leftBtn_001.png", 1.08, [this](auto btn) {
        m_countInput->setString(numToString(std::clamp(m_trigger->getCount() - 1, 1, 20)), true);
    });
    countLeftBtn->setID("count-left-btn");
    countContainer->addChildAtPosition(countLeftBtn, Anchor::Center, {-54, 0});

    auto countRightBtn = CCMenuItemExt::createSpriteExtraWithFrameName("edit_rightBtn_001.png", 1.08, [this](auto btn) {
        m_countInput->setString(numToString(std::clamp(m_trigger->getCount() + 1, 1, 20)), true);
    });
    countRightBtn->setID("count-right-btn");
    countContainer->addChildAtPosition(countRightBtn, Anchor::Center, {54, 0});

    auto countLabel = CCLabelBMFont::create("Clone Count", "goldFont.fnt");
    countLabel->setID("count-label");
    countLabel->setScale(.63);
    countLabel->setAnchorPoint({0.5, 0});
    countContainer->addChildAtPosition(countLabel, Anchor::Center, {0, 22.5});

    auto delayContainer = CCMenu::create();
    delayContainer->setID("delay-container");
    delayContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(delayContainer, Anchor::TopRight, {-75, -80});

    m_delaySlider = Slider::create(this, menu_selector(ClonesTriggerPopup::activateCallback));
    m_delaySlider->ignoreAnchorPointForPosition(false);
    m_delaySlider->setAnchorPoint({0, 0});
    m_delaySlider->setScale(.7f);
    m_delayCallback = [this](auto slider) {
        m_trigger->setCount(slider->getValue() * 20);
        m_delayInput->setString(numToString(m_trigger->getCount()));
    };
    m_delaySlider->setValue(m_trigger->getDelay() / 20);
    m_delaySlider->updateBar();
    delayContainer->addChild(m_delaySlider);

    m_delayInput = TextInput::create(70, nullptr);
    m_delayInput->setID("delay-input");
    m_delayInput->setScale(.9);
    m_delayInput->setCommonFilter(CommonFilter::Float);
    m_delayInput->setString(numToString(trigger->getCount()));
    m_delayInput->setCallback([this](const std::string& str) {
        auto res = numFromString<float>(str);
        if (!res) return log::warn("Failed to convert contents of count '{}' to float: {}", str, res.unwrapErr());
        m_trigger->setDelay(std::clamp(res.unwrap(), 0.f, 20.f));
        m_delaySlider->setValue(m_trigger->getDelay() / 20);
        m_delaySlider->updateBar();
    });
    delayContainer->addChildAtPosition(m_delayInput, Anchor::Center, {44.5, 30});

    auto delayLabel = CCLabelBMFont::create("Delay", "goldFont.fnt");
    delayLabel->setID("delay-label");
    delayLabel->setScale(.63);
    delayLabel->setAnchorPoint({1, .5});
    delayContainer->addChildAtPosition(delayLabel, Anchor::Center, {0, 30});

    // input +25
    // label 0 with anchor 1, .5

    auto okBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK",40,0,0.8,true,"goldFont.fnt", "GJ_button_01.png",30.0), this, menu_selector(ClonesTriggerPopup::onClose));
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, {0, 24});

    return true;
}

void ClonesTriggerPopup::activateCallback(CCObject* sender) {
    if (sender == m_delaySlider) m_delayCallback(m_delaySlider);
}

ClonesTriggerPopup* ClonesTriggerPopup::create(CosmicClonesTrigger* trigger) {
    auto ret = new ClonesTriggerPopup();
    if (!ret->init(trigger)) return delete ret, nullptr;
    return ret->autorelease(), ret;
}
