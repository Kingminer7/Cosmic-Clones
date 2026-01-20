#pragma once

#include <Geode/loader/SettingV3.hpp>

struct Style {
  Style() {};
  Style(std::string type) : type(type) {}
  Style(std::string type, cocos2d::ccColor3B col1, cocos2d::ccColor3B col2, bool useGlow, cocos2d::ccColor3B glow) : type(type), col1(col1), col2(col2), useGlow(useGlow), glow(glow) {}
  std::string type = "Cosmic Mario\n(SMG 1)";
  cocos2d::ccColor3B col1 = cocos2d::ccWHITE;
  cocos2d::ccColor3B col2 = cocos2d::ccWHITE;
  bool useGlow = false;
  cocos2d::ccColor3B glow = cocos2d::ccWHITE;

  bool operator==(const Style& other) const {
    if (type != other.type) return false;
    if (type != "Custom") return true;
    return col1 == other.col1 && col2 == other.col2 && useGlow == other.useGlow && (!useGlow || glow == other.glow);
  }

  bool operator!=(const Style& other) const {
    return !(*this == other);
  }
};

template <>
struct matjson::Serialize<Style> {
    static geode::Result<Style> fromJson(const Value& value) {
        GEODE_UNWRAP_INTO(std::string type, value["type"].asString());
        if (type == "Custom") {
            GEODE_UNWRAP_INTO(cocos2d::ccColor3B col1, value["col1"].as<cocos2d::ccColor3B>());
            GEODE_UNWRAP_INTO(cocos2d::ccColor3B col2, value["col2"].as<cocos2d::ccColor3B>());
            GEODE_UNWRAP_INTO(bool useGlow, value["use-glow"].as<bool>());
            GEODE_UNWRAP_INTO(cocos2d::ccColor3B glow, value["glow"].as<cocos2d::ccColor3B>());
            return geode::Ok(Style { type, col1, col2, useGlow, glow });
        }
        return geode::Ok(Style {type});
    }
    static Value toJson(const Style& style) {
        if (style.type == "Custom") return makeObject({
            { "type", style.type },
            { "col1", style.col1 },
            { "col2", style.col2 },
            { "use-glow", style.useGlow },
            { "glow", style.glow },
        });
        return makeObject({{"type", style.type}});
    }
};

class CloneStyleSetting : public geode::SettingBaseValueV3<std::vector<Style>> {
protected:
    std::vector<Style> m_styles = {};

public:
    static geode::Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json);
    geode::SettingNodeV3* createNode(float width) override;
};

template <>
struct geode::SettingTypeForValueType<std::vector<Style>> {
  using SettingType = CloneStyleSetting;
};

class StyleNode;

class CloneStyleSettingNode : public geode::SettingValueNodeV3<CloneStyleSetting> {
protected:
  bool init(std::shared_ptr<CloneStyleSetting> setting, float width);
  void updateState(CCNode* invoker) override;

  std::vector<StyleNode*> m_nodes;
  CCMenuItemSpriteExtra* m_addBtn = nullptr;
public:
  void setNodeValue(StyleNode* node, Style value);
  void remove(int id);

  static CloneStyleSettingNode* create(std::shared_ptr<CloneStyleSetting> setting, float width);
};


class StyleNode : public cocos2d::CCMenu {
protected:
  bool init(CloneStyleSettingNode* setting, Style value);

  static std::vector<Style> m_styles;
  Style m_style;
  cocos2d::CCLabelBMFont* m_label = nullptr;
  CloneStyleSettingNode* m_setting = nullptr;
  CCMenu* m_customMenu = nullptr;

public:
  void updateState(Style style);
  Style getStyle() { return m_style; }

  static StyleNode* create(CloneStyleSettingNode* setting, Style value);
};