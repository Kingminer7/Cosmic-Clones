#include "EditorUI.hpp"
#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>

#include "GJBaseGameLayer.hpp"
#include "../ClonesTriggerPopup.hpp"
#include "../ShaderManager.hpp"

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
            auto btn = createButton(spr, [this](auto btn) {
                if (m_fields->m_selected) {
                    m_fields->m_selected->setColor({255, 255, 255});
                    if (m_fields->m_selected == btn) {
                        m_selectedObjectIndex = 0;
                        m_fields->m_selected = nullptr;
                        return;
                    }
                }
                m_selectedObjectIndex = CLONE_ID;
                updateCreateMenu(false);
                updateGridNodeSize();
                btn->setColor({127, 127, 127});
                m_fields->m_selected = btn;
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
    auto fields = m_fields.self();
    if (id != CLONE_ID) return EditorUI::onCreateObject(id);

    id = TRIGGER_ID;
    fields->m_creatingClone = true;
    EditorUI::onCreateObject(id);
    fields->m_createdClone->setupCloneTrigger(true);
}

GameObject* CosmicClonesEditorUI::createObject(const int id, CCPoint pos) {
    auto ret = EditorUI::createObject(id, std::move(pos));
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
            auto popup = ClonesTriggerPopup::create(modify_cast<CosmicClonesTrigger*>(m_selectedObject));
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
    if (m_fields->m_selected) {
        m_fields->m_selected->setColor({255, 255, 255});
        m_fields->m_selected = nullptr;
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
        log::debug("Clone Trigger:\n"
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
            getStyles());
	if (isDisabled()) return;
        auto cLayer = static_cast<CosmicClonesGJBGL*>(layer);
        cLayer->updateFromTrigger(this);
        cLayer->startClones();
        return;
    }
    SequenceTriggerGameObject::triggerObject(layer, uniqueID, remapKeys);
}

void CosmicClonesTrigger::setupCloneTrigger(bool initial)
{
    setUserFlag("clones-trigger"_spr);
    m_sequenceMode = CLONE_ID;
    updateCloneTriggerSprite(this);

    if (initial) {
        setCount(3);
        setStartDelay(1.75);
        setDelay(1);
        setDamaging(true);
        setDisabled(false);
        setStyles({"Cosmic Mario\n(SMG 1)"});
	m_isMultiTriggered = false;
    }
}

int CosmicClonesTrigger::getCount() const {
    return m_resetMode;
}

void CosmicClonesTrigger::setCount(const int count) {
    m_resetMode = count;
}

float CosmicClonesTrigger::getStartDelay() const {
    return m_reset;
}

void CosmicClonesTrigger::setStartDelay(const float startDelay) {
    m_reset = startDelay;
}

float CosmicClonesTrigger::getDelay() const {
    return m_minInt;
}

void CosmicClonesTrigger::setDelay(const float delay) {
    m_minInt = delay;
}

bool CosmicClonesTrigger::isDamaging() const {
    return m_sequenceTotalCount;
}

void CosmicClonesTrigger::setDamaging(const bool damaging) {
    m_sequenceTotalCount = damaging;
}

bool CosmicClonesTrigger::isDisabled() const {
    return m_uniqueRemap;
}

void CosmicClonesTrigger::setDisabled(const bool disabled) {
    m_uniqueRemap = disabled;
}

std::vector<std::string> CosmicClonesTrigger::getStyles() const {
    std::vector<std::string> ret;
    for (auto obj : m_chanceObjects){
        if (obj.m_groupID != -42) continue;
        ret.push_back(intToStyle(obj.m_chance));
    }
    return ret;
}

void CosmicClonesTrigger::setStyles(const std::vector<std::string>& styles) {
    if (m_chanceObjects.size() > 5) {
        m_chanceObjects.erase(m_chanceObjects.begin() + 5, m_chanceObjects.end());
    } else if (m_chanceObjects.size() < 5) {
        for (int i = 0; m_chanceObjects.size() < 5; i++) {
            // filler
            m_chanceObjects.push_back(ChanceObject(-1, -1));
        }
    }
    for (int i = 0; i < styles.size(); i++) {
        const auto style = styleToInt(styles[i]);
        // 42 is the number of galaxies in smg1
        m_chanceObjects.push_back(ChanceObject(-42, style));
    }
}

std::string CosmicClonesTrigger::intToStyle(int i) {
    switch (i) {
        case 0:
            return "Cosmic Mario\n(SMG 1)";
        case 1:
            return "Cosmic Clone\n(SMG 2)";
        case 2:
            return "Badeline Chaser\n(Celeste)";
        case 3:
            return "Custom";
        default:
            return "Cosmic Mario\n(SMG 1)";
    }
}

int CosmicClonesTrigger::styleToInt(std::string s) {
    if (s == "Cosmic Clone\n(SMG 2)") return 1;
    if (s == "Badeline Chaser\n(Celeste)") return 2;
    if (s == "Custom") return 3;
    return 0;
}
