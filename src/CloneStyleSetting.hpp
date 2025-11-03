#pragma once

#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

class CloneStyleSetting : public SettingBaseValueV3<std::vector<std::string>> {
protected:
    std::vector<std::string> m_styles = {};
public:
    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json);
    SettingNodeV3* createNode(float width) override;
};

template <>
struct geode::SettingTypeForValueType<std::vector<std::string>> {
    using SettingType = CloneStyleSetting;
};

class StyleNode;

class CloneStyleSettingNode : public SettingValueNodeV3<CloneStyleSetting> {
protected:
    bool init(std::shared_ptr<CloneStyleSetting> setting, float width);
    void updateState(CCNode* invoker) override;

    std::vector<StyleNode*> m_nodes;
    CCMenuItemSpriteExtra* m_addBtn = nullptr;
public:
    void setNodeValue(StyleNode* node, std::string value);
    void remove(int id);

    static CloneStyleSettingNode* create(std::shared_ptr<CloneStyleSetting> setting, float width);
};


class StyleNode : public CCMenu {
protected:
    bool init(CloneStyleSettingNode* setting, std::string value);

    static std::vector<std::string> m_styles;
    CCLabelBMFont* m_label = nullptr;
    CloneStyleSettingNode* m_setting = nullptr;
public:
    void updateState(std::string value);

    static StyleNode* create(CloneStyleSettingNode* setting, std::string value);
};