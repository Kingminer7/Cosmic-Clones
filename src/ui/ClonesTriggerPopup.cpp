#include "ClonesTriggerPopup.hpp"

using namespace geode::prelude;

struct LambdaObj : CCNode {
    Function<void(Slider* slider)> callback;
    Slider* slider;
    LambdaObj(Slider* slider, Function<void(Slider* slider)> callback) : callback(std::move(callback)), slider(slider) {}

    void execute(CCObject*) {
	    callback(slider);
    }
};

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

    auto infoBtn = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_infoIcon_001.png", 1.f, [this](auto){
        // // TODO: Type this
        // FLAlertLayer::create("Help", "TODO", "ok")->show();
        SetupSequenceTriggerPopup::create(m_trigger)->show();
    });
    m_buttonMenu->addChildAtPosition(infoBtn, Anchor::TopLeft, {18, -18});

    m_spawnContainer = CCMenu::create();
    m_spawnContainer->setID("spawn-container");
    m_spawnContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_spawnContainer, Anchor::BottomLeft, {20, 20});
    m_spawnToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->m_isSpawnTriggered = !toggle->isToggled();
        m_trigger->m_isTouchTriggered = false;
        m_touchToggle->toggle(false);
        m_multiContainer->setVisible(m_trigger->m_isTouchTriggered || m_trigger->m_isSpawnTriggered);
    });
    m_spawnToggle->toggle(trigger->m_isSpawnTriggered);
    m_spawnToggle->setID("spawn-toggle");
    m_spawnContainer->addChild(m_spawnToggle);

    auto spawnLabel = CCLabelBMFont::create("Spawn\nTrigger", "bigFont.fnt");
    spawnLabel->setAnchorPoint({0, 0.5});
    spawnLabel->setID("spawn-label");
    spawnLabel->setScale(.35);
    spawnLabel->setAlignment(kCCTextAlignmentLeft);
    spawnLabel->setPositionX(20.35);
    m_spawnContainer->addChild(spawnLabel);

    m_touchContainer = CCMenu::create();
    m_touchContainer->setID("touch-container");
    m_touchContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_touchContainer, Anchor::BottomLeft, {20, 50});

    m_touchToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->m_isTouchTriggered = !toggle->isToggled();
        m_trigger->m_isSpawnTriggered = false;
        m_spawnToggle->toggle(false);
        m_multiContainer->setVisible(m_trigger->m_isTouchTriggered || m_trigger->m_isSpawnTriggered);
    });
    m_touchToggle->toggle(trigger->m_isTouchTriggered);
    m_touchToggle->setID("touch-toggle");
    m_touchContainer->addChild(m_touchToggle);

    auto touchLabel = CCLabelBMFont::create("Touch\nTrigger", "bigFont.fnt");
    touchLabel->setAnchorPoint({0, 0.5});
    touchLabel->setID("touch-label");
    touchLabel->setScale(.35);
    touchLabel->setAlignment(kCCTextAlignmentLeft);
    touchLabel->setPositionX(20.35);
    m_touchContainer->addChild(touchLabel);

    m_multiContainer = CCMenu::create();
    m_multiContainer->setID("multi-container");
    m_multiContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_multiContainer, Anchor::BottomLeft, {20, 80});

    m_multiToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->m_isMultiTriggered = !toggle->isToggled();
    });
    m_multiToggle->toggle(trigger->m_isMultiTriggered);
    m_multiToggle->setID("multi-toggle");
    m_multiToggle->setVisible(trigger->m_isTouchTriggered || trigger->m_isSpawnTriggered);
    m_multiContainer->addChild(m_multiToggle);

    auto multiLabel = CCLabelBMFont::create("Multi\nTrigger", "bigFont.fnt");
    multiLabel->setAnchorPoint({0, 0.5});
    multiLabel->setID("multi-label");
    multiLabel->setScale(.35);
    multiLabel->setAlignment(kCCTextAlignmentLeft);
    multiLabel->setPositionX(20.35);
    multiLabel->setVisible(trigger->m_isTouchTriggered || trigger->m_isSpawnTriggered);
    m_multiContainer->addChild(multiLabel);

    m_countContainer = CCMenu::create();
    m_countContainer->setID("count-container");
    m_countContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_countContainer, Anchor::Left, {75, 55});

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
    m_countContainer->addChild(m_countInput);

    auto countLeftBtn = CCMenuItemExt::createSpriteExtraWithFrameName("edit_leftBtn_001.png", 1.08, [this](auto btn) {
        m_countInput->setString(numToString(std::clamp(m_trigger->getCount() - 1, 1, 20)), true);
    });
    countLeftBtn->setID("count-left-btn");
    m_countContainer->addChildAtPosition(countLeftBtn, Anchor::Center, {-54, 0});

    auto countRightBtn = CCMenuItemExt::createSpriteExtraWithFrameName("edit_rightBtn_001.png", 1.08, [this](auto btn) {
        m_countInput->setString(numToString(std::clamp(m_trigger->getCount() + 1, 1, 20)), true);
    });
    countRightBtn->setID("count-right-btn");
    m_countContainer->addChildAtPosition(countRightBtn, Anchor::Center, {54, 0});

    auto countLabel = CCLabelBMFont::create("Clone Count", "goldFont.fnt");
    countLabel->setID("count-label");
    countLabel->setScale(.63);
    countLabel->setAnchorPoint({0.5, 0});
    m_countContainer->addChildAtPosition(countLabel, Anchor::Center, {0, 22.5});

    m_idContainer = CCMenu::create();
    m_idContainer->setID("id-container");
    m_idContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_idContainer, Anchor::Left, {75, -15});

    m_idInput = TextInput::create(70, nullptr);
    m_idInput->setID("id-input");
    m_idInput->setScale(.9);
    m_idInput->setCommonFilter(CommonFilter::Uint);
    m_idInput->setString(numToString(trigger->getControllerID()));
    m_idInput->setCallback([this](const std::string& str) {
        auto res = numFromString<int>(str);
        if (!res) return log::warn("Failed to convert contents of id '{}' to integer: {}", str, res.unwrapErr());
        m_trigger->setControllerID(std::clamp(res.unwrap(), 1, 100));
    });
    m_idContainer->addChild(m_idInput);

    auto idLeftBtn = CCMenuItemExt::createSpriteExtraWithFrameName("edit_leftBtn_001.png", 1.08, [this](auto btn) {
        m_idInput->setString(numToString(std::clamp(m_trigger->getControllerID() - 1, 1, 100)), true);
    });
    idLeftBtn->setID("id-left-btn");
    m_idContainer->addChildAtPosition(idLeftBtn, Anchor::Center, {-54, 0});

    auto idRightBtn = CCMenuItemExt::createSpriteExtraWithFrameName("edit_rightBtn_001.png", 1.08, [this](auto btn) {
        m_idInput->setString(numToString(std::clamp(m_trigger->getControllerID() + 1, 1, 100)), true);
    });
    idRightBtn->setID("id-right-btn");
    m_idContainer->addChildAtPosition(idRightBtn, Anchor::Center, {54, 0});

    auto idLabel = CCLabelBMFont::create("Controller Id", "goldFont.fnt");
    idLabel->setID("id-label");
    idLabel->setScale(.63);
    idLabel->setAnchorPoint({0.5, 0});
    m_idContainer->addChildAtPosition(idLabel, Anchor::Center, {0, 22.5});

    m_delayContainer = CCMenu::create();
    m_delayContainer->setID("delay-container");
    m_delayContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_delayContainer, Anchor::Right, {-90, 55});

    m_delaySlider = Slider::create(nullptr, nullptr);
    auto lambda = new LambdaObj(m_delaySlider, [this](auto slider) {
        m_trigger->setDelay(std::round((.25 + slider->getValue() * 19.75) * 100) / 100);
        m_delayInput->setString(numToString(m_trigger->getDelay()));
    });
    lambda->autorelease();
    m_delaySlider->m_touchLogic->m_thumb->m_pfnSelector = menu_selector(LambdaObj::execute);
    m_delaySlider->m_touchLogic->m_thumb->m_pListener = lambda;
    m_delaySlider->setUserObject("lambda", lambda);
    m_delaySlider->ignoreAnchorPointForPosition(false);
    m_delaySlider->setAnchorPoint({0, 0});
    m_delaySlider->setScale(.7f);
    m_delaySlider->setValue(m_trigger->getDelay() / 20);
    m_delaySlider->updateBar();
    m_delaySlider->setID("delay-slider");
    m_delayContainer->addChild(m_delaySlider);

    m_delayInput = TextInput::create(70, nullptr);
    m_delayInput->setID("delay-input");
    m_delayInput->setScale(.9);
    m_delayInput->setCommonFilter(CommonFilter::Float);
    m_delayInput->setString(numToString(trigger->getDelay()));
    m_delayInput->setCallback([this](const std::string& str) {
        auto res = numFromString<float>(str);
        if (!res) return log::warn("Failed to convert contents of delay '{}' to float: {}", str, res.unwrapErr());
        m_trigger->setDelay(std::round(std::clamp(res.unwrap(), 0.25f, 20.f) * 100) / 100);
        m_delaySlider->setValue(m_trigger->getDelay() / 20.f);
        m_delaySlider->updateBar();
    });
    m_delayContainer->addChildAtPosition(m_delayInput, Anchor::Center, {34.5, 30});

    auto delayLabel = CCLabelBMFont::create("Delay", "chatFont.fnt");
    delayLabel->setID("delay-label");
    delayLabel->setScale(.8);
    delayLabel->setAnchorPoint({1, .5});
    m_delayContainer->addChildAtPosition(delayLabel, Anchor::Center, {-10, 30});

    m_startDelayContainer = CCMenu::create();
    m_startDelayContainer->setID("start-delay-container");
    m_startDelayContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_startDelayContainer, Anchor::Right, {-90, -15});

    m_startDelaySlider = Slider::create(nullptr, nullptr);
    auto sdlambda = new LambdaObj(m_startDelaySlider, [this](auto slider) {
        m_trigger->setStartDelay(std::round((.25 + slider->getValue() * 19.75) * 100) / 100);
        m_startDelayInput->setString(numToString(m_trigger->getStartDelay()));
    });
    sdlambda->autorelease();
    m_startDelaySlider->m_touchLogic->m_thumb->m_pfnSelector = menu_selector(LambdaObj::execute);
    m_startDelaySlider->m_touchLogic->m_thumb->m_pListener = sdlambda;
    m_startDelaySlider->setUserObject("lambda", sdlambda);
    m_startDelaySlider->ignoreAnchorPointForPosition(false);
    m_startDelaySlider->setAnchorPoint({0, 0});
    m_startDelaySlider->setScale(.7f);
    m_startDelaySlider->setValue(m_trigger->getStartDelay() / 20);
    m_startDelaySlider->updateBar();
    m_startDelaySlider->setID("start-delay-slider");
    m_startDelayContainer->addChild(m_startDelaySlider);

    m_startDelayInput = TextInput::create(70, nullptr);
    m_startDelayInput->setID("start-delay-input");
    m_startDelayInput->setScale(.9);
    m_startDelayInput->setCommonFilter(CommonFilter::Float);
    m_startDelayInput->setString(numToString(trigger->getStartDelay()));
    m_startDelayInput->setCallback([this](const std::string& str) {
        auto res = numFromString<float>(str, 2);
        if (!res) return log::warn("Failed to convert contents of start delay '{}' to float: {}", str, res.unwrapErr());
        m_trigger->setStartDelay(std::round(std::clamp(res.unwrap(), 0.25f, 20.f) * 100) / 100);
        m_startDelaySlider->setValue(m_trigger->getStartDelay() / 20.f);
        m_startDelaySlider->updateBar();
    });
    m_startDelayContainer->addChildAtPosition(m_startDelayInput, Anchor::Center, {34.5, 30});

    auto startDelayLabel = CCLabelBMFont::create("Start Delay", "chatFont.fnt");
    startDelayLabel->setID("start-delay-label");
    startDelayLabel->setScale(.8);
    startDelayLabel->setAnchorPoint({1, .5});
    m_startDelayContainer->addChildAtPosition(startDelayLabel, Anchor::Center, {-10, 30});

    m_disabledContainer = CCMenu::create();
    m_disabledContainer->setID("disabled-container");
    m_disabledContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_disabledContainer, Anchor::BottomRight, {-75, 80});

    m_disabledToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->setDisabled(!toggle->isToggled());
    });
    m_disabledToggle->toggle(trigger->isDisabled());
    m_disabledToggle->setID("disabled-toggle");
    m_disabledContainer->addChild(m_disabledToggle);

    auto disabledLabel = CCLabelBMFont::create("Disable", "bigFont.fnt");
    disabledLabel->setAnchorPoint({0, 0.5});
    disabledLabel->setID("disabled-label");
    disabledLabel->setScale(.35);
    disabledLabel->setAlignment(kCCTextAlignmentLeft);
    disabledLabel->setPositionX(20.35);
    m_disabledContainer->addChild(disabledLabel);

    m_damageContainer = CCMenu::create();
    m_damageContainer->setID("damage-container");
    m_damageContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_damageContainer, Anchor::BottomRight, {-75, 50});

    m_damageToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->setDamaging(!toggle->isToggled());
    });
    m_damageToggle->toggle(trigger->isDamaging());
    m_damageToggle->setID("damage-toggle");
    m_damageContainer->addChild(m_damageToggle);

    auto damageLabel = CCLabelBMFont::create("Damage", "bigFont.fnt");
    damageLabel->setAnchorPoint({0, 0.5});
    damageLabel->setID("damage-label");
    damageLabel->setScale(.35);
    damageLabel->setAlignment(kCCTextAlignmentLeft);
    damageLabel->setPositionX(20.35);
    m_damageContainer->addChild(damageLabel);

    m_stopperContainer = CCMenu::create();
    m_stopperContainer->setID("stopper-container");
    m_stopperContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(m_stopperContainer, Anchor::BottomRight, {-75, 20});

    m_stopperToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->setStopper(!toggle->isToggled());
        m_countContainer->setVisible(toggle->isToggled());
        m_idContainer->setVisible(toggle->isToggled());
        m_delayContainer->setVisible(toggle->isToggled());
        m_startDelayContainer->setVisible(toggle->isToggled());
        m_disabledContainer->setVisible(toggle->isToggled());
        m_damageContainer->setVisible(toggle->isToggled());
    });
    bool stopper = trigger->isStopper();
    m_countContainer->setVisible(!stopper);
    m_idContainer->setVisible(!stopper);
    m_delayContainer->setVisible(!stopper);
    m_startDelayContainer->setVisible(!stopper);
    m_disabledContainer->setVisible(!stopper);
    m_damageContainer->setVisible(!stopper);
    m_stopperToggle->toggle(stopper);
    m_stopperToggle->setID("stopper-toggle");
    m_stopperContainer->addChild(m_stopperToggle);

    auto stopperLabel = CCLabelBMFont::create("Stop", "bigFont.fnt");
    stopperLabel->setAnchorPoint({0, 0.5});
    stopperLabel->setID("stopper-label");
    stopperLabel->setScale(.35);
    stopperLabel->setAlignment(kCCTextAlignmentLeft);
    stopperLabel->setPositionX(20.35);
    m_stopperContainer->addChild(stopperLabel);

    auto okBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK",40,0,0.8,true,"goldFont.fnt", "GJ_button_01.png",30.0), this, menu_selector(ClonesTriggerPopup::onClose));
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, {0, 24});

    return true;
}

ClonesTriggerPopup* ClonesTriggerPopup::create(CosmicClonesTrigger* trigger) {
    auto ret = new ClonesTriggerPopup();
    if (!ret->init(trigger)) return delete ret, nullptr;
    return ret->autorelease(), ret;
}
