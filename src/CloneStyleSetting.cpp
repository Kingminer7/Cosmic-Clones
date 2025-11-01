#include "CloneStyleSetting.hpp"

std::vector<std::string> StyleNode::m_styles = {
    "Cosmic Mario\n(SMG 1)",
    "Cosmic Clones\n(SMG 2)",
    "Badeline Chaser\n(Celeste)",
    "Custom"
};

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
    auto val = getValue();
    auto bm = getButtonMenu();
    if (m_nodes.size() < val.size()) {
        for (size_t i = m_nodes.size(); i < val.size(); ++i) {
            auto node = StyleNode::create(this, val[i]);
            node->setTag(static_cast<int>(i));
            node->getChildByID("delete-button")->setVisible(val.size() > 1);
            m_nodes.push_back(node);
            bm->addChild(node);
        }
    }
    for (size_t i = 0; i < m_nodes.size();) {
        auto node = m_nodes[i];
        if (i >= val.size()) {
            node->removeFromParent();
            m_nodes.erase(m_nodes.begin() + i);
        } else {
            node->setTag(static_cast<int>(i));
            node->getChildByID("delete-button")->setVisible(val.size() > 1);
            node->updateState(val[i]);
            ++i;
        }
    }

    auto h = m_nodes.size() * 30.f + 30.f;
    bm->setContentHeight(h);
    setContentHeight(h);
    bm->updateLayout();
}

bool CloneStyleSettingNode::init(std::shared_ptr<CloneStyleSetting> setting, float width)  {
    if (!SettingValueNodeV3::init(setting, width))
        return false;

    auto bm = getButtonMenu();
    auto addBtn = CCMenuItemExt::createSpriteExtraWithFrameName("edit_addCBtn_001.png", .5f, [this](auto btn) {
        auto val = getValue();
        val.push_back("Cosmic Mario\n(SMG 1)");
        setValue(val, btn);
    });
    addBtn->setContentSize({30.f, 30.f});
    auto img = addBtn->getNormalImage();
    // img->retain();
    img->removeFromParent();
    addBtn->addChildAtPosition(img, Anchor::Center);
    // img->release();
    bm->addChild(addBtn);
    int i = 0;
    auto val = getValue();
    for (auto value : val) {
        auto node = StyleNode::create(this, value);
        node->setTag(i);
        node->getChildByID("delete-button")->setVisible(val.size() > 1);
        m_nodes.push_back(node);
        bm->addChild(node);
        i++;
    }

    auto h = m_nodes.size() * 30.f + 30.f;
    bm->setContentHeight(h);
    setContentHeight(h);
    bm->setContentWidth(100);
    bm->setLayout(ColumnLayout::create()->setAxisReverse(true)->setGap(0.f));

    this->updateState(nullptr);

    return true;
}

void CloneStyleSettingNode::setNodeValue(StyleNode* node, std::string value) {
    auto val = getValue();
    val[node->getTag()] = value;
    setValue(val, node);
}

void CloneStyleSettingNode::remove(int id) {
    auto val = getValue();
    if (id < 0 || id >= static_cast<int>(val.size()))
        return;
    val.erase(val.begin() + id);
    setValue(val, this);
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

bool StyleNode::init(CloneStyleSettingNode* setting, std::string value) {
    if (!CCMenu::init()) return false;
    setID("clone-style-node");
    m_setting = setting;
    ignoreAnchorPointForPosition(false);
    setAnchorPoint({.5f, .5f});
    setContentSize({120, 30});
    m_label = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
    m_label->setScale(75.f / m_label->getContentWidth());
    m_label->setAlignment(kCCTextAlignmentCenter);
    m_label->setID("display-label");
    auto lArrow = CCMenuItemExt::createSpriteExtraWithFrameName("navArrowBtn_001.png", .4f, [this, value](auto) {
        auto it = std::find(m_styles.begin(), m_styles.end(), m_label->getString());
        if (it == m_styles.begin() || it == m_styles.end())
            it = m_styles.end() - 1;
        else
            --it;
        auto val = *it;
        m_setting->setNodeValue(this, val);
    });
    static_cast<CCSprite*>(lArrow->getNormalImage())->setFlipX(true);
    lArrow->setID("left-arrow-button");
    auto rArrow = CCMenuItemExt::createSpriteExtraWithFrameName("navArrowBtn_001.png", .4f, [this, value](auto) {
        auto it = std::find(m_styles.begin(), m_styles.end(), m_label->getString());
        if (it == m_styles.end() - 1 || it == m_styles.end())
            it = m_styles.begin();
        else
            ++it;
        auto val = *it;
        m_setting->setNodeValue(this, val);
    });
    rArrow->setID("right-arrow-button");

    auto del = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_deleteIcon_001.png", .7f, [this, value](auto) {
        m_setting->remove(getTag());
    });
    del->setID("delete-button");

    addChildAtPosition(m_label, Anchor::Center, {-7.5f, 0.f});
    addChildAtPosition(lArrow, Anchor::Left, {5.f, 0.f});
    addChildAtPosition(rArrow, Anchor::Right, {-20.f, 0.f});
    addChildAtPosition(del, Anchor::Right, {-5.f, 0.f});
    return true;
}

void StyleNode::updateState(std::string value) {
    m_label->setString(value.c_str());
    if (auto width = m_label->getContentWidth(); width > 0.001f)
        m_label->setScale(std::min(80.f / width, .45f));
    else
        m_label->setScale(.001f);
}

StyleNode* StyleNode::create(CloneStyleSettingNode* setting, std::string value) {
    auto ret = new StyleNode();
    if (ret->init(setting, value)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

$execute {
    (void)Mod::get()->registerCustomSettingType("clone-style", &CloneStyleSetting::parse);
}