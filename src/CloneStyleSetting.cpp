#include "CloneStyleSetting.hpp"

Result<std::shared_ptr<SettingV3> > CloneStyleSetting::parse(std::string const &key, std::string const &modID, matjson::Value const &json) {
    auto res = std::make_shared<CloneStyleSetting>();
    auto root = checkJson(json, "CloneStyleSetting");
    res->parseBaseProperties(key, modID, root);
    root.has("styles").into(res->m_styles);
    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

SettingNodeV3* CloneStyleSetting::createNode(float width) {
    return CloneStyleSettingNode::create(std::static_pointer_cast<CloneStyleSetting>(shared_from_this()), width);
}

void CloneStyleSettingNode::updateState(CCNode *invoker) {

    SettingValueNodeV3::updateState(invoker);

    // auto shouldEnable = this->getSetting()->shouldEnable();
    //
    // for (auto toggle : m_toggles) {
    //
    //     toggle->toggle(toggle->getTag() == static_cast<int>(this->getValue()));
    //
    //     toggle->setEnabled(shouldEnable);
    //     toggle->setCascadeColorEnabled(true);
    //     toggle->setCascadeOpacityEnabled(true);
    //     toggle->setOpacity(shouldEnable ? 255 : 155);
    //     toggle->setColor(shouldEnable ? ccWHITE : ccGRAY);
    // }
}

bool CloneStyleSettingNode::init(std::shared_ptr<CloneStyleSetting> setting, float width)  {
    if (!SettingValueNodeV3::init(setting, width))
        return false;

    auto bm = getButtonMenu();
    for (auto value : getValue()) {
        auto node = CCMenu::create();
        node->ignoreAnchorPointForPosition(false);
        node->setAnchorPoint({.5f, .5f});
        node->setContentSize({120, 30});
        auto lab = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
        lab->limitLabelWidth(80.f, .7f, .05f);
        auto lArrow = CCMenuItemExt::createSpriteExtraWithFrameName("navArrowBtn_001.png", .4f, [](auto) {

        });
        static_cast<CCSprite*>(lArrow->getNormalImage())->setFlipX(true);
        auto rArrow = CCMenuItemExt::createSpriteExtraWithFrameName("navArrowBtn_001.png", .4f, [](auto) {

        });

        node->addChildAtPosition(lab, Anchor::Center, {0.f, 0.f});
        node->addChildAtPosition(lArrow, Anchor::Left, {5.f, 0.f});
        node->addChildAtPosition(rArrow, Anchor::Right, {-5.f, 0.f});
        m_nodes.insert({value, node});
        bm->addChild(node);
    }


    bm->setContentWidth(100);
    bm->setContentHeight(m_nodes.size() * 30.f);
    bm->setLayout(ColumnLayout::create());

    this->updateState(nullptr);

    return true;
}

CloneStyleSettingNode* CloneStyleSettingNode::create(std::shared_ptr<CloneStyleSetting> setting, float width) {
    auto ret = new CloneStyleSettingNode();
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

$execute {
    (void)Mod::get()->registerCustomSettingType("clone-style", &CloneStyleSetting::parse);
}