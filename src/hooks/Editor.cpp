#include "Editor.hpp"

#include <Geode/Geode.hpp>

#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>

#include "Gameplay.hpp"
#include "../Utils.hpp"
#include "../internal/CosmicClonesController.hpp"
#include "../internal/ShaderManager.hpp"
#include "../ui/ClonesTriggerPopup.hpp"

// sequence trigger
#define TRIGGER_ID 3607

// i dont think we'll ever get 11 million objects
// if we do then we'll figure this out then
// this is a very specific reference
#define CLONE_ID 11012007

using namespace geode::prelude;

bool CosmicClonesEditorUI::init(LevelEditorLayer* editorLayer)
{
    if (!EditorUI::init(editorLayer)) return false;

    alpha::editor_tabs::addTab("clones-tab"_spr, alpha::editor_tabs::BUILD,
        [this] {
            std::vector<Ref<CCNode>> nodes;
            auto spr = CCSprite::createWithSpriteFrameName("triggerMain.png"_spr);
            CosmicClonesTrigger::updateCloneTriggerSprite(spr);
            auto fields = static_cast<CosmicClonesEditorLayer*>(m_editorLayer)->m_fields.self();
            auto btn = createButton(spr, [this, fields](auto btn) {
                if (fields->m_selected) {
                    fields->m_selected->setColor({255, 255, 255});
                    if (fields->m_selected == btn) {
                        m_selectedObjectIndex = 0;
                        fields->m_selected = nullptr;
                        return;
                    }
                }
                m_selectedObjectIndex = CLONE_ID;
                updateCreateMenu(false);
                updateGridNodeSize();
                btn->setColor({127, 127, 127});
                fields->m_selected = btn;
            });
            nodes.emplace_back(btn);
            return alpha::editor_tabs::createEditButtonBar(nodes);
        }, [] {
            auto spr = CCSprite::createWithSpriteFrameName("cube_1.png"_spr);
            spr->setShaderProgram(ShaderManager::get().getCosmicShader());
            return spr;
        },
        [] (bool state, auto tab) { // do something when the tab is entered and exited

        },
        [] (int rows, int cols, auto tab) { // do something when the tab is reloaded

        });

    return true;
}

void CosmicClonesEditorUI::onCreateObject(int id) {
    auto fields = static_cast<CosmicClonesEditorLayer*>(m_editorLayer)->m_fields.self();
    if (id != CLONE_ID) return EditorUI::onCreateObject(id);

    id = TRIGGER_ID;
    fields->m_creatingClone = true;
    EditorUI::onCreateObject(id);
    if (fields->m_creatingClone) {
        log::error("Failed to create clone trigger for some reason! Something has gone horribly wrong.");
        fields->m_creatingClone = false;
        return;
    }
    fields->m_createdClone->setupCloneTrigger(true);
}

GameObject* CosmicClonesEditorLayer::createObject(const int id, CCPoint pos, bool noUndo) {
    auto ret = LevelEditorLayer::createObject(id, std::move(pos), noUndo);
    auto eui = static_cast<CosmicClonesEditorUI*>(m_editorUI);
    auto fields = m_fields.self();
    if (fields->m_creatingClone) {
        fields->m_createdClone = modify_cast<CosmicClonesTrigger*>(ret);
        fields->m_creatingClone = false;
    }
    return ret;
}

void CosmicClonesEditorUI::editObject(CCObject* sender) {
    if (m_selectedObject && m_selectedObject->m_objectID == TRIGGER_ID) {
        if (m_selectedObject->getUserFlag("clones-trigger"_spr)) {
            auto popup = ClonesTriggerPopup::create(static_cast<CosmicClonesTrigger*>(m_selectedObject));
            popup->m_noElasticity = true;
            popup->show();
            return;
        }
    }
    EditorUI::editObject(sender);
}

void CosmicClonesTrigger::updateCloneTriggerSprite(CCSprite* sprite, bool setFrame) {
    if (setFrame) sprite->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("triggerMain.png"_spr));
    if (!sprite->getChildByID("outline"_spr)) {
        auto spr = CCSprite::createWithSpriteFrameName("triggerOutline.png"_spr);
        spr->setID("outline"_spr);
        spr->setZOrder(1);
        sprite->addChildAtPosition(spr, Anchor::Center, {0, 0}, false);
    }
    if (!sprite->getChildByID("inner"_spr)) {
        auto spr = CCSprite::createWithSpriteFrameName("triggerInner.png"_spr);
        spr->setID("inner"_spr);
        spr->setShaderProgram(ShaderManager::get().getCosmicShader());
        sprite->addChildAtPosition(spr, Anchor::Center, {0, 0}, false);
    }
}

CCMenuItemSpriteExtra* CosmicClonesEditorUI::createButton(CCSprite* icon, Function<void(CCMenuItemSpriteExtra*)> callback) {
    auto spr = ButtonSprite::create(icon, 0x20, 0, 32.0f, 1.0f, false, "GJ_button_04.png", true);
    icon->setScale(32 / spr->getContentHeight());
    spr->setCascadeColorEnabled(true);
    auto ret = CCMenuItemExt::createSpriteExtra(spr, std::move(callback));
    ret->setScale(0.9f);
    ret->m_baseScale = 0.9f;
    icon->setPositionY(icon->getPositionY() - 2);

    return ret;
}

void CosmicClonesEditorUI::onCreateButton(CCObject* sender) {
    auto fields = static_cast<CosmicClonesEditorLayer*>(m_editorLayer)->m_fields.self();
    if (fields->m_selected) {
        fields->m_selected->setColor({255, 255, 255});
        fields->m_selected = nullptr;
    }
    EditorUI::onCreateButton(sender);
}


void CosmicClonesTrigger::customObjectSetup(gd::vector<gd::string>& values, gd::vector<void*>& exists)
{
    SequenceTriggerGameObject::customObjectSetup(values, exists);
    if (m_sequenceMode == CLONE_ID) {
        setupCloneTrigger();
    }
}

void CosmicClonesTrigger::triggerObject(GJBaseGameLayer* layer, int uniqueID, gd::vector<int> const* remapKeys) {

    if (getUserFlag("clones-trigger"_spr)) {
        log::dev("Clone Trigger:\n"
              "Count: {}\n"
              "Start Delay: {}\n"
              "Delay: {}\n"
              "Damage Enabled: {}\n"
              "Disabled: {}\n"
              "Styles: {}",
        getCount(),
        getStartDelay(),
        getDelay(),
        isDamaging(),
        isDisabled(),
        matjson::Value(getStyles()).dump());
	    if (isDisabled()) return;
        auto bgl = static_cast<CosmicClonesGJBGL*>(layer);
        auto bglFields = bgl->m_fields.self();
        auto id = getControllerID();
        auto it = bglFields->m_triggerControllers.find(id);
        if (isStopper()) {
            if (it == bglFields->m_triggerControllers.end() || it->second == nullptr) return;
            if (it->second->isStopped()) return;
            it->second->stop();
            return;
        }
        if (it == bglFields->m_triggerControllers.end() || it->second == nullptr) {
            auto controller = CosmicClonesController::createFromTrigger(bgl, this);
            bglFields->m_triggerControllers[id] = controller;
            controller->loadConfigFromTrigger(this);
            controller->start();
        } else {
            if (!it->second->isStopped()) return;
            it->second->loadConfigFromTrigger(this);
            it->second->start();
        }
        return;
    }
    SequenceTriggerGameObject::triggerObject(layer, uniqueID, remapKeys);
}

void CosmicClonesTrigger::setupCloneTrigger(bool initial) {
    setUserFlag("clones-trigger"_spr);
    m_sequenceMode = CLONE_ID;
    updateCloneTriggerSprite(this);

    if (initial) {
        setCount(3);
        setStartDelay(1.75);
        setDelay(1);
        setDamaging(true);
        setDisabled(false);
        setStopper(false);
	    setSfx(true);
        setStyles({{"Cosmic Mario\n(SMG 1)"}});
	    m_isMultiTriggered = false;
    }
}

int CosmicClonesTrigger::getCount() const {
    return -m_resetMode;
}

void CosmicClonesTrigger::setCount(const int count) {
    m_resetMode = -count;
}

float CosmicClonesTrigger::getStartDelay() const {
    return -m_reset;
}

void CosmicClonesTrigger::setStartDelay(const float startDelay) {
    m_reset = -startDelay;
}

float CosmicClonesTrigger::getDelay() const {
    return -m_minInt;
}

void CosmicClonesTrigger::setDelay(const float delay) {
    m_minInt = -delay;
}

bool CosmicClonesTrigger::isDamaging() const {
    if (m_chanceObjects.size() < 1) return true;
    return (m_chanceObjects[0].m_groupID & 1 << 0) != 0;
}

void CosmicClonesTrigger::setDamaging(const bool damaging) {
    fill10IfNeeded();

    if (damaging) m_chanceObjects[0].m_groupID |= 1 << 0;
    else m_chanceObjects[0].m_groupID &= ~(1 << 0);
}

bool CosmicClonesTrigger::isStopper() const {
    if (m_chanceObjects.size() < 1) return false;
    return m_chanceObjects[0].m_groupID & 1 << 1;
}

void CosmicClonesTrigger::setStopper(const bool stopper) {
    fill10IfNeeded();

    if (stopper) m_chanceObjects[0].m_groupID |= (1 << 1);
    else m_chanceObjects[0].m_groupID &= ~(1 << 1);
}

bool CosmicClonesTrigger::isSfx() const {
    if (m_chanceObjects.size() < 1) return false;
    return m_chanceObjects[0].m_groupID & 1 << 2;
}

void CosmicClonesTrigger::setSfx(const bool sfx) {
    fill10IfNeeded();

    if (sfx) m_chanceObjects[0].m_groupID |= (1 << 2);
    else m_chanceObjects[0].m_groupID &= ~(1 << 2);
}

bool CosmicClonesTrigger::isDisabled() const {
    return m_uniqueRemap;
}

void CosmicClonesTrigger::setDisabled(const bool disabled) {
    m_uniqueRemap = disabled;
}

int CosmicClonesTrigger::getControllerID() const {
    return -m_chanceObjects[0].m_chance;
}

void CosmicClonesTrigger::setControllerID(const int id) {
    m_chanceObjects[0].m_chance = -id;
}

inline ccColor3B colorFromInt(int i) {
    return {
        static_cast<GLubyte>(i >> 16 & 0xFF),
        static_cast<GLubyte>(i >> 8 & 0xFF),
        static_cast<GLubyte>(i & 0xFF)
    };
}

inline int colorToInt(const ccColor3B color) {
    return color.r << 16 | color.g << 8 | color.b;
}

// Chance object mapping
// m_groupID: type
// m_chance: (custom) style color 1
// Second object (when type == "Custom")
// m_groupID: (custom) style color 2
// m_chance: (custom) glow

std::vector<Style> CosmicClonesTrigger::getStyles() const {
    std::vector<Style> ret;
    if (10 >= m_chanceObjects.size()) return getSettingFast<"styles", std::vector<Style>>();
    for (int i = 10; i < m_chanceObjects.size();) {
        auto co = m_chanceObjects[i];
        if (co.m_groupID == -1) ret.push_back({"Cosmic Mario\n(SMG 1)"});
        else if (co.m_groupID == -2) ret.push_back({"Cosmic Clone\n(SMG 2)"});
        else if (co.m_groupID == -3) ret.push_back({"Badeline Chaser\n(Celeste)"});
        else if (co.m_groupID == -4) {
            ret.push_back(chanceObjectsToCustomStyle(co, i + 1 == m_chanceObjects.size() ? ChanceObject{} : m_chanceObjects[i  + 1]));
            i++;
        }
        i++;
    }
    if (ret.empty()) return getSettingFast<"styles", std::vector<Style>>();
    return ret;
}

void CosmicClonesTrigger::setStyles(const std::vector<Style>& styles) {
    fill10IfNeeded();
    if (m_chanceObjects.size() > 10) {
        m_chanceObjects.erase(m_chanceObjects.begin() + 10, m_chanceObjects.end());
    }
    for (int i = 0; i < styles.size(); i++) {
        if (styles[i].type == "Cosmic Mario\n(SMG 1)") m_chanceObjects.emplace_back(ChanceObject{-1, -1});
        else if (styles[i].type == "Cosmic Clone\n(SMG 2)") m_chanceObjects.emplace_back(ChanceObject{-2, -1});
        else if (styles[i].type == "Badeline Chaser\n(Celeste)") m_chanceObjects.emplace_back(ChanceObject{-3, -1});
        else if (styles[i].type == "Custom") {
            const auto& [first, second] = customStyleToChanceObjects(styles[i]);
            m_chanceObjects.emplace_back(first);
            m_chanceObjects.emplace_back(second);
        }
    }
}

void CosmicClonesTrigger::fill10IfNeeded() {
    if (m_chanceObjects.size() < 10) {
        for (int i = 0; m_chanceObjects.size() < 10; i++) {
            // filler in case i add more data
            m_chanceObjects.emplace_back(-1, -1);
        }
    }
}

Style CosmicClonesTrigger::chanceObjectsToCustomStyle(const ChanceObject& first, const ChanceObject& second) {
    auto col1 = colorFromInt(-first.m_chance);
    auto col2 = colorFromInt(-second.m_groupID);
    bool glowOn = -second.m_chance >> 24 & 1;
    auto glow = colorFromInt(-second.m_chance);
    return {"Custom", col1, col2, glowOn, glow};
}

std::pair<ChanceObject, ChanceObject> CosmicClonesTrigger::customStyleToChanceObjects(const Style& s) {
    const auto col1 = colorToInt(s.col1);
    const auto col2 = colorToInt(s.col2);
    const auto glow = colorToInt(s.glow) | s.useGlow << 24;
    return {ChanceObject{-4, -col1}, ChanceObject{-col2, -glow}};
}
