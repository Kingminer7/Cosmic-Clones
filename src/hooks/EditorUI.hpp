#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/SequenceTriggerGameObject.hpp>

class $modify(CosmicClonesTrigger, SequenceTriggerGameObject) {
    void customObjectSetup(gd::vector<gd::string>& values, gd::vector<void*>& exists);
    void triggerObject(GJBaseGameLayer* layer, int uniqueID, gd::vector<int> const* remapKeys);

    void setupCloneTrigger(bool initial = false);

    int getCount() const;
    void setCount(int count);

    float getStartDelay() const;
    void setStartDelay(float startDelay);

    float getDelay() const;
    void setDelay(float delay);

    bool isDamaging() const;
    void setDamaging(bool damaging);

    bool isDisabled() const;
    void setDisabled(bool disabled);

    std::vector<std::string> getStyles() const;
    void setStyles(const std::vector<std::string>& styles);

    static std::string intToStyle(int i);
    static int styleToInt(std::string s);

    static void updateCloneTriggerSprite(CCSprite* sprite, bool setFrame = true);
};

class $modify(CosmicClonesEditorUI, EditorUI) {
    struct Fields {
        bool m_creatingClone = false;
        CosmicClonesTrigger* m_createdClone = nullptr;
        CCMenuItemSpriteExtra* m_selected = nullptr;
    };

    bool init(LevelEditorLayer* editorLayer);
    void onCreateObject(int id);
    GameObject* createObject(int id, cocos2d::CCPoint pos);
    void editObject(CCObject* sender);
    void onCreateButton(CCObject* sender);

    CCMenuItemSpriteExtra* createButton(cocos2d::CCSprite* icon, geode::Function<void(CCMenuItemSpriteExtra*)> callback);
};