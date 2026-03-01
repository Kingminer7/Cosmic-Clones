#include "ClonesTriggerPopup.hpp"

using namespace geode::prelude;

struct LambdaObj : CCNode {
    Function<void(Slider* slider)> callback;
    Slider* slider;
    LambdaObj(Slider* slider, Function<void(Slider* slider)> callback) : slider(slider), callback(std::move(callback)) {}

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
    m_mainLayer->addChildAtPosition(multiContainer, Anchor::BottomLeft, {20, 80});

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
    m_mainLayer->addChildAtPosition(countContainer, Anchor::Left, {75, 55});

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
    m_mainLayer->addChildAtPosition(delayContainer, Anchor::Right, {-90, 55});

    m_delaySlider = Slider::create(nullptr, nullptr);
    auto lambda = new LambdaObj(m_delaySlider, [this](auto slider) {
        m_trigger->setDelay(std::round(slider->getValue() * 20 * 100) / 100);
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
    delayContainer->addChild(m_delaySlider);

    m_delayInput = TextInput::create(70, nullptr);
    m_delayInput->setID("delay-input");
    m_delayInput->setScale(.9);
    m_delayInput->setCommonFilter(CommonFilter::Float);
    m_delayInput->setString(numToString(trigger->getDelay()));
    m_delayInput->setCallback([this](const std::string& str) {
        auto res = numFromString<float>(str);
        if (!res) return log::warn("Failed to convert contents of delay '{}' to float: {}", str, res.unwrapErr());
        m_trigger->setDelay(std::round(std::clamp(res.unwrap(), 0.f, 20.f) * 100) / 100);
        m_delaySlider->setValue(m_trigger->getDelay() / 20.f);
        m_delaySlider->updateBar();
    });
    delayContainer->addChildAtPosition(m_delayInput, Anchor::Center, {34.5, 30});

    auto delayLabel = CCLabelBMFont::create("Delay", "chatFont.fnt");
    delayLabel->setID("delay-label");
    delayLabel->setScale(.8);
    delayLabel->setAnchorPoint({1, .5});
    delayContainer->addChildAtPosition(delayLabel, Anchor::Center, {-10, 30});

    auto startDelayContainer = CCMenu::create();
    startDelayContainer->setID("start-delay-container");
    startDelayContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(startDelayContainer, Anchor::Right, {-90, -15});

    m_startDelaySlider = Slider::create(nullptr, nullptr);
    auto sdlambda = new LambdaObj(m_startDelaySlider, [this](auto slider) {
        m_trigger->setStartDelay(std::round(slider->getValue() * 20 * 100) / 100);
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
    startDelayContainer->addChild(m_startDelaySlider);

    m_startDelayInput = TextInput::create(70, nullptr);
    m_startDelayInput->setID("start-delay-input");
    m_startDelayInput->setScale(.9);
    m_startDelayInput->setCommonFilter(CommonFilter::Float);
    m_startDelayInput->setString(numToString(trigger->getStartDelay()));
    m_startDelayInput->setCallback([this](const std::string& str) {
        auto res = numFromString<float>(str, 2);
        if (!res) return log::warn("Failed to convert contents of start delay '{}' to float: {}", str, res.unwrapErr());
        m_trigger->setStartDelay(std::round(std::clamp(res.unwrap(), 0.f, 20.f) * 100) / 100);
        m_startDelaySlider->setValue(m_trigger->getStartDelay() / 20.f);
        m_startDelaySlider->updateBar();
    });
    startDelayContainer->addChildAtPosition(m_startDelayInput, Anchor::Center, {34.5, 30});

    auto startDelayLabel = CCLabelBMFont::create("Start Delay", "chatFont.fnt");
    startDelayLabel->setID("start-delay-label");
    startDelayLabel->setScale(.8);
    startDelayLabel->setAnchorPoint({1, .5});
    startDelayContainer->addChildAtPosition(startDelayLabel, Anchor::Center, {-10, 30});

    auto disabledContainer = CCMenu::create();
    disabledContainer->setID("disabled-container");
    disabledContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(disabledContainer, Anchor::BottomRight, {-75, 20});

    m_disabledToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->setDisabled(!toggle->isToggled());
    });
    m_disabledToggle->toggle(trigger->isDisabled());
    m_disabledToggle->setID("disabled-toggle");
    disabledContainer->addChild(m_disabledToggle);

    m_disabledLabel = CCLabelBMFont::create("Disable", "bigFont.fnt");
    m_disabledLabel->setAnchorPoint({0, 0.5});
    m_disabledLabel->setID("disabled-label");
    m_disabledLabel->setScale(.35);
    m_disabledLabel->setAlignment(kCCTextAlignmentLeft);
    m_disabledLabel->setPositionX(20.35);
    disabledContainer->addChild(m_disabledLabel);

    auto damageContainer = CCMenu::create();
    damageContainer->setID("damage-container");
    damageContainer->setContentSize({0, 0});
    m_mainLayer->addChildAtPosition(damageContainer, Anchor::BottomRight, {-75, 50});

    m_damageToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->setDamaging(!toggle->isToggled());
    });
    m_damageToggle->toggle(trigger->isDamaging());
    m_damageToggle->setID("damage-toggle");
    damageContainer->addChild(m_damageToggle);

    m_damageLabel = CCLabelBMFont::create("Damage", "bigFont.fnt");
    m_damageLabel->setAnchorPoint({0, 0.5});
    m_damageLabel->setID("damage-label");
    m_damageLabel->setScale(.35);
    m_damageLabel->setAlignment(kCCTextAlignmentLeft);
    m_damageLabel->setPositionX(20.35);
    damageContainer->addChild(m_damageLabel);

    auto okBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK",40,0,0.8,true,"goldFont.fnt", "GJ_button_01.png",30.0), this, menu_selector(ClonesTriggerPopup::onClose));
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, {0, 24});

    return true;
}

ClonesTriggerPopup* ClonesTriggerPopup::create(CosmicClonesTrigger* trigger) {
    auto ret = new ClonesTriggerPopup();
    if (!ret->init(trigger)) return delete ret, nullptr;
    return ret->autorelease(), ret;
}
