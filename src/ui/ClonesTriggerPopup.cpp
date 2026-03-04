#include "ClonesTriggerPopup.hpp"

#include "../internal/ShaderManager.hpp"

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

    m_page1 = CCLayer::create();
    m_page1->setID("page-1");
    m_page1->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChildAtPosition(m_page1, Anchor::Center);

    m_page2 = CCLayer::create();
    m_page2->setID("page-2");
    m_page2->setVisible(false);
    m_page2->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChildAtPosition(m_page2, Anchor::Center);

    static_cast<CopySizeLayout*>(m_mainLayer->getLayout())->add(m_page1);
    static_cast<CopySizeLayout*>(m_mainLayer->getLayout())->add(m_page2);
    m_mainLayer->updateLayout();

    auto left = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_arrow_01_001.png", .7f, [this](auto){
	    m_page1->setVisible(!m_page1->isVisible());
	    m_page2->setVisible(!m_page2->isVisible());
    });
    left->setID("left-button");
    m_buttonMenu->addChildAtPosition(left, Anchor::Left, {-15, 0});
    
    auto right = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_arrow_01_001.png", .7f, [this](auto){
	    m_page1->setVisible(!m_page1->isVisible());
	    m_page2->setVisible(!m_page2->isVisible());
    });
    static_cast<CCSprite*>(right->getNormalImage())->setFlipX(true);
    right->setID("right-button");
    m_buttonMenu->addChildAtPosition(right, Anchor::Right, {15, 0});

    auto infoBtn = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_infoIcon_001.png", 1.f, [this](auto){
        // // TODO: Type this
        // FLAlertLayer::create("Help", "TODO", "ok")->show();
        SetupSequenceTriggerPopup::create(m_trigger)->show();
    });
    infoBtn->setID("info-button");
    m_buttonMenu->addChildAtPosition(infoBtn, Anchor::TopLeft, {18, -18});

    m_spawnContainer = CCMenu::create();
    m_spawnContainer->setID("spawn-container");
    m_spawnContainer->setContentSize({0, 0});
    m_page1->addChildAtPosition(m_spawnContainer, Anchor::BottomLeft, {20, 20});
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
    m_page1->addChildAtPosition(m_touchContainer, Anchor::BottomLeft, {20, 50});

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
    m_multiContainer->setVisible(m_trigger->m_isTouchTriggered || m_trigger->m_isSpawnTriggered);
    m_page1->addChildAtPosition(m_multiContainer, Anchor::BottomLeft, {20, 80});

    m_multiToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->m_isMultiTriggered = !toggle->isToggled();
    });
    m_multiToggle->toggle(trigger->m_isMultiTriggered);
    m_multiToggle->setID("multi-toggle");
    m_multiContainer->addChild(m_multiToggle);

    auto multiLabel = CCLabelBMFont::create("Multi\nTrigger", "bigFont.fnt");
    multiLabel->setAnchorPoint({0, 0.5});
    multiLabel->setID("multi-label");
    multiLabel->setScale(.35);
    multiLabel->setAlignment(kCCTextAlignmentLeft);
    multiLabel->setPositionX(20.35);
    m_multiContainer->addChild(multiLabel);

    m_countContainer = CCMenu::create();
    m_countContainer->setID("count-container");
    m_countContainer->setContentSize({0, 0});
    m_page1->addChildAtPosition(m_countContainer, Anchor::Left, {75, 55});

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
    m_page1->addChildAtPosition(m_idContainer, Anchor::Left, {75, -15});

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
    m_page1->addChildAtPosition(m_delayContainer, Anchor::Right, {-90, 55});

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
    m_page1->addChildAtPosition(m_startDelayContainer, Anchor::Right, {-90, -15});

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
    m_page1->addChildAtPosition(m_disabledContainer, Anchor::BottomRight, {-75, 80});

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
    m_page1->addChildAtPosition(m_damageContainer, Anchor::BottomRight, {-75, 50});

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
    m_page1->addChildAtPosition(m_stopperContainer, Anchor::BottomRight, {-75, 20});

    m_stopperToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this, left, right](CCMenuItemToggler* toggle) {
        bool stopper = !toggle->isToggled();
        m_trigger->setStopper(stopper);
        m_countContainer->setVisible(!stopper);
        m_delayContainer->setVisible(!stopper);
        m_startDelayContainer->setVisible(!stopper);
        m_disabledContainer->setVisible(!stopper);
        m_damageContainer->setVisible(!stopper);
        left->setVisible(!stopper);
        right->setVisible(!stopper);
    });
    bool stopper = trigger->isStopper();
    m_countContainer->setVisible(!stopper);
    m_delayContainer->setVisible(!stopper);
    m_startDelayContainer->setVisible(!stopper);
    m_disabledContainer->setVisible(!stopper);
    m_damageContainer->setVisible(!stopper);
    left->setVisible(!stopper);
    right->setVisible(!stopper);
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


    m_styleScroll = ScrollLayer::create({155, 180});
    m_styleScroll->ignoreAnchorPointForPosition(false);
    m_styleScroll->setID("style-scroll");
    m_page2->addChildAtPosition(m_styleScroll, Anchor::Center, {0, 10});

    float y = 0;
    for (const auto& style : trigger->getStyles()) {
        auto node = StyleNode::create(this, style);
        node->updateState(style);
        y += node->getContentHeight();
        m_styleScroll->m_contentLayer->addChild(node);
    }
    m_styleScroll->m_contentLayer->setContentHeight(std::max(y, m_styleScroll->getContentHeight()));
    m_styleScroll->m_contentLayer->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->ignoreInvisibleChildren(false)->setAutoScale(false)->setCrossAxisOverflow(false));
    m_styleScroll->scrollToTop();

    m_sfxContainer = CCMenu::create();
    m_sfxContainer->setID("sfx-container");
    m_sfxContainer->setContentSize({0, 0});
    m_page2->addChildAtPosition(m_sfxContainer, Anchor::BottomRight, {-75, 20});

    m_sfxToggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this](CCMenuItemToggler* toggle) {
        m_trigger->setSfx(!toggle->isToggled());
    });
    m_sfxToggle->toggle(trigger->isSfx());
    m_sfxToggle->setID("sfx-toggle");
    m_sfxContainer->addChild(m_sfxToggle);

    auto sfxLabel = CCLabelBMFont::create("SFX", "bigFont.fnt");
    sfxLabel->setAnchorPoint({0, 0.5});
    sfxLabel->setID("sfx-label");
    sfxLabel->setScale(.35);
    sfxLabel->setAlignment(kCCTextAlignmentLeft);
    sfxLabel->setPositionX(20.35);
    m_sfxContainer->addChild(sfxLabel);

    auto okBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK",40,0,0.8,true,"goldFont.fnt", "GJ_button_01.png",30.0), this, menu_selector(ClonesTriggerPopup::onClose));
    okBtn->setID("ok-button");
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, {0, 24});

    return true;
}

void ClonesTriggerPopup::onClose(CCObject *sender) {
    std::vector<Style> styles;
    for (const auto child : m_styleScroll->m_contentLayer->getChildrenExt<StyleNode*>()) {
        styles.push_back(child->getStyle());
    }
    m_trigger->setStyles(styles);
    Popup::onClose(sender);
}

ScrollLayer* ClonesTriggerPopup::getScroll() const {
    return m_styleScroll;
}

ClonesTriggerPopup* ClonesTriggerPopup::create(CosmicClonesTrigger* trigger) {
    auto ret = new ClonesTriggerPopup();
    if (!ret->init(trigger)) return delete ret, nullptr;
    return ret->autorelease(), ret;
}

std::vector<Style> StyleNode::m_styles = {
    {"Cosmic Mario\n(SMG 1)"},
    {"Cosmic Clone\n(SMG 2)"},
    {"Badeline Chaser\n(Celeste)"},
    {"Custom"}
};

ClonesTriggerPopup* StyleNode::getPopup() const {
    return m_popup;
}

bool StyleNode::init(ClonesTriggerPopup* popup, Style value) {
    if (!CCMenu::init()) return false;
    setID("clone-style-node");
    m_popup = popup;
    m_style = std::move(value);
    ignoreAnchorPointForPosition(false);
    setAnchorPoint({.5f, .5f});
    setContentSize({155, m_style.type == "Custom" ? 120.f : 30.f});
    m_label = CCLabelBMFont::create(value.type.c_str(), "bigFont.fnt");
    m_label->setScale(75.f / m_label->getContentWidth());
    m_label->setAlignment(kCCTextAlignmentCenter);
    m_label->setID("display-label");
    auto lArrow = CCMenuItemExt::createSpriteExtraWithFrameName("navArrowBtn_001.png", .4f, [this](auto) {
        auto it = std::ranges::find(m_styles, m_style);
        if (it == m_styles.begin() || it == m_styles.end()) it = m_styles.end() - 1;
        else --it;
        updateState(*it);
    });
    static_cast<CCSprite*>(lArrow->getNormalImage())->setFlipX(true);
    lArrow->setID("left-arrow-button");
    auto rArrow = CCMenuItemExt::createSpriteExtraWithFrameName("navArrowBtn_001.png", .4f, [this, value](auto) {
        auto it = std::ranges::find(m_styles, m_style);
        if (it == m_styles.end() - 1 || it == m_styles.end()) it = m_styles.begin();
        else ++it;
        updateState(*it);
    });
    rArrow->setID("right-arrow-button");

    auto del = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_deleteIcon_001.png", .7f, [this](auto) {
        removeFromParent();
        auto scroll = m_popup->getScroll();
        scroll->m_contentLayer->setContentHeight(std::max(scroll->m_contentLayer->getContentHeight() - getContentHeight(), scroll->getContentHeight()));
        scroll->m_contentLayer->updateLayout();
    });
    del->setID("delete-button");

    m_preview = SimplePlayer::create(1);
    m_preview->setID("preview");
    m_preview->setScale(5.f / 6);
    addChildAtPosition(m_preview, Anchor::TopLeft, {15, -15});

    addChildAtPosition(m_label, Anchor::Top, {5.f, -15.f});
    addChildAtPosition(lArrow, Anchor::TopLeft, {35.f, -15.f});
    addChildAtPosition(rArrow, Anchor::TopRight, {-25.f, -15.f});
    addChildAtPosition(del, Anchor::TopRight, {-10.f, -15.f});

    m_customMenu = CCMenu::create();
    m_customMenu->setContentSize({155, 90});
    m_customMenu->setAnchorPoint({.5f, .5f});
    m_customMenu->ignoreAnchorPointForPosition(false);

    auto col1Lab = CCLabelBMFont::create("Color 1", "bigFont.fnt");
    col1Lab->setAnchorPoint({0.f, .5f});
    col1Lab->setContentSize({120, 90.f});
    col1Lab->setScale(.4f);
    m_customMenu->addChildAtPosition(col1Lab, Anchor::Center, {-45, 30});

    auto col1Spr = ColorChannelSprite::create();
    col1Spr->setColor(m_style.col1);
    col1Spr->setScale(.7f);
    auto col1Btn = CCMenuItemExt::createSpriteExtra(col1Spr, [this](auto) {
        auto popup = ColorPickPopup::create(m_style.col1);
        popup->setCallback([this](ccColor4B const& color) {
            m_style.col1 = ccColor3B(color.r, color.g, color.b);
            updateState(m_style);
        });
        popup->show();
    });
    col1Btn->setID("col1");
    m_customMenu->addChildAtPosition(col1Btn, Anchor::Center, {62.5, 30});

    auto col2Lab = CCLabelBMFont::create("Color 2", "bigFont.fnt");
    col2Lab->setAnchorPoint({0.f, .5f});
    col2Lab->setContentSize({120, 90.f});
    col2Lab->setScale(.4f);
    m_customMenu->addChildAtPosition(col2Lab, Anchor::Center, {-45, 0});

    auto col2Spr = ColorChannelSprite::create();
    col2Spr->setColor(m_style.col2);
    col2Spr->setScale(.7f);
    auto col2Btn = CCMenuItemExt::createSpriteExtra(col2Spr, [this](auto) {
        auto popup = ColorPickPopup::create(m_style.col2);
        popup->setCallback([this](ccColor4B const& color) {
            m_style.col2 = ccColor3B(color.r, color.g, color.b);
            updateState(m_style);
        });
        popup->show();
    });
    col2Btn->setID("col2");
    m_customMenu->addChildAtPosition(col2Btn, Anchor::Center, {62.5, 0});

    auto glowLab = CCLabelBMFont::create("Glow", "bigFont.fnt");
    glowLab->setAnchorPoint({0.f, .5f});
    glowLab->setContentSize({120, 90.f});
    glowLab->setScale(.4f);
    m_customMenu->addChildAtPosition(glowLab, Anchor::Center, {-45, -30});

    auto glowSpr = ColorChannelSprite::create();
    glowSpr->setColor(m_style.glow);
    glowSpr->setScale(.7f);
    auto glowBtn = CCMenuItemExt::createSpriteExtra(glowSpr, [this](auto) {
        auto popup = ColorPickPopup::create(m_style.glow);
        popup->setCallback([this](ccColor4B const& color) {
            m_style.glow = ccColor3B(color.r, color.g, color.b);
            updateState(m_style);
        });
        popup->show();
    });
    glowBtn->setID("glow");
    m_customMenu->addChildAtPosition(glowBtn, Anchor::Center, {62.5, -30});

    auto toggle = CCMenuItemExt::createTogglerWithStandardSprites(.7f, [this, glowBtn](auto) {
        queueInMainThread([this] {
            m_style.useGlow = !m_style.useGlow;
            updateState(m_style);
        });
    });
    toggle->setCascadeOpacityEnabled(true);
    glowBtn->setEnabled(m_style.useGlow);
    toggle->setID("glow-toggler");
    m_customMenu->addChildAtPosition(toggle, Anchor::Center, {32.5, -30});

    addChildAtPosition(m_customMenu, Anchor::Center, {0, -15.f});
    return true;
}

void StyleNode::updateState(const Style& style) {
    if (style.type == "Custom" && m_style.type != "Custom") {
        auto cl = m_popup->getScroll()->m_contentLayer;
        cl->setPositionY(cl->getPositionY() - 90);
        cl->setContentHeight(cl->getContentHeight() + 90);
    } else if (m_style.type == "Custom" && style.type != "Custom") {
        auto cl = m_popup->getScroll()->m_contentLayer;
        cl->setPositionY(cl->getPositionY() + 90);
        cl->setContentHeight(cl->getContentHeight() - 90);
    }
    m_style = std::move(style);
    m_label->setString(m_style.type.c_str());
    if (auto width = m_label->getContentWidth(); width > 0.001f) m_label->setScale(std::min(80.f / width, .45f));
    else m_label->setScale(.001f);
    setContentHeight(m_style.type == "Custom" ? 120.f : 30.f);
    m_customMenu->setVisible(m_style.type == "Custom");

    m_customMenu->getChildByID("col1")->getChildByType<ColorChannelSprite*>(0)->setColor(m_style.col1);
    m_customMenu->getChildByID("col2")->getChildByType<ColorChannelSprite*>(0)->setColor(m_style.col2);
    auto glowBtn = m_customMenu->getChildByID("glow");
    glowBtn->getChildByType<ColorChannelSprite*>(0)->setColor(m_style.glow);
    typeinfo_cast<CCMenuItemSpriteExtra*>(glowBtn)->setEnabled(m_style.useGlow);
    typeinfo_cast<CCMenuItemToggler*>(m_customMenu->getChildByID("glow-toggler"))->toggle(m_style.useGlow);
    updateLayout();

    m_preview->setColors(style.getColor1(), style.getColor2());
    if (style.isGlowEnabled()) m_preview->setGlowOutline(style.getGlowColor());
    else m_preview->disableGlowOutline();
    if (style.type == "Cosmic Mario\n(SMG 1)") {
        const auto shader = ShaderManager::get().getCosmicShader();
        m_preview->getChildByIndex(0)->setShaderProgram(shader);
        for (const auto child : m_preview->getChildByIndex(0)->getChildrenExt()) child->setShaderProgram(shader);
    }
    else {
        const auto shader = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor);
        m_preview->getChildByIndex(0)->setShaderProgram(shader);
        for (const auto child : m_preview->getChildByIndex(0)->getChildrenExt()) child->setShaderProgram(shader);
    }
    auto scroll = m_popup->getScroll();
    scroll->m_contentLayer->updateLayout();
}

StyleNode* StyleNode::create(ClonesTriggerPopup* popup, const Style& value) {
    auto ret = new StyleNode();
    if (ret->init(popup, value)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}