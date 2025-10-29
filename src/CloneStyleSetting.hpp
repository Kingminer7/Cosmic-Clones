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

class CloneStyleSettingNode : public SettingValueNodeV3<CloneStyleSetting> {
protected:
    bool init(std::shared_ptr<CloneStyleSetting> setting, float width);
    void updateState(CCNode* invoker) override;

    std::map<std::string, CCMenu*> m_nodes;
public:
    static CloneStyleSettingNode* create(std::shared_ptr<CloneStyleSetting> setting, float width);
};