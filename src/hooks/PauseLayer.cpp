#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

#include "../ShaderManager.hpp"
using namespace geode::prelude;

class $modify(ClonesPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto spr = CosmicSprite::create(CCSprite::create("cube_1.png"_spr));
        spr->setRotation(55);
        spr->getNode()->setPosition(spr->getContentSize().width / 2, spr->getContentSize().height / 2);
        spr->updateStyle(Style{});

        auto btn = CCMenuItemExt::createSpriteExtra(CircleButtonSprite::create(spr, CircleBaseColor::Green, CircleBaseSize::MediumAlt), [](auto) {
           openSettingsPopup(Mod::get(), true);
        });
        spr->setScale(.75);
        auto menu = getChildByID("left-button-menu");
        menu->addChild(btn);
        menu->updateLayout();

    }
};