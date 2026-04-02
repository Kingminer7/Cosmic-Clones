#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/SequenceTriggerGameObject.hpp>

struct Style;
class CosmicClonesController;

// Property mappings
// int   count:        int   m_resetMode
// float startDelay:   float m_reset
// float delay:        float m_minInt
// bool  isDisabled:   bool  m_uniqueRemap
// int   controllerId: int   m_chanceObjects[0].m_chance
// these ones use bitwise operators
// bool  damaging:     int   m_chanceObjects[0].m_groupID (bit 0)
// bool  stopper:      int   m_chanceObjects[0].m_groupID (bit 1)
// bool  sfx:          int   m_chanceObjects[0].m_groupID (bit 2)

class $modify(CosmicClonesTrigger, SequenceTriggerGameObject)
{
    struct Fields {
        std::shared_ptr<CosmicClonesController> controller = nullptr;
    };

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

    int getControllerID() const;
    void setControllerID(int id);

    bool isStopper() const;
    void setStopper(bool stopper);

    bool isSfx() const;
    void setSfx(bool sfx);

    std::vector<Style> getStyles() const;
    void setStyles(const std::vector<Style>& styles);

    void fill10IfNeeded();

    static Style chanceObjectsToCustomStyle(const ChanceObject& first, const ChanceObject& second);
    static std::pair<ChanceObject, ChanceObject> customStyleToChanceObjects(const Style& s);

    static void updateCloneTriggerSprite(CCSprite* sprite, bool setFrame = true);
};

class $modify(CosmicClonesEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer);
    void onCreateObject(int id);
    void editObject(CCObject* sender);
    void onCreateButton(CCObject* sender);

    CCMenuItemSpriteExtra* createButton(cocos2d::CCSprite* icon, geode::Function<void(CCMenuItemSpriteExtra*)> callback);
};

class $modify(CosmicClonesEditorLayer, LevelEditorLayer) {
    struct Fields {
        bool m_creatingClone = false;
        CosmicClonesTrigger* m_createdClone = nullptr;
        CCMenuItemSpriteExtra* m_selected = nullptr;
    };
    
    GameObject* createObject(int id, cocos2d::CCPoint pos, bool noUndo);
};
